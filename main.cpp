#include "header.h"

const int maxBalance = 1000000, maxTrans = 100, userNum=1000, transNum=10000, blockSize = 100; // blocksize<=transNum
std::vector<block> blockchain;

transaction::transaction(std::string inPublicKey, std::string outPublicKey, int sum){
    inKey = inPublicKey;
    outKey = outPublicKey;
    this->sum = sum;
    std::string all = inPublicKey + outPublicKey + std::to_string(this->sum);
    hashID = badHash(all);
}

transaction::operator=(transaction b) {
    inKey = b.inKey;
    outKey = b.outKey;
    sum = b.sum;
    std::string all = b.inKey + b.outKey + std::to_string(b.sum);
    hashID = badHash(all);
}

void generateUsers(const int n, user users[]) {
    for(int i=0; i<n; i++) {
        int b = rand()%(maxBalance-100)+100;
        std::string name = "name"+std::to_string(i);
        user t(name, badHash(name), b);
        users[i]=t;
    }
}

std::vector<transaction> generateTransactions(const int n, user users[]) {
    std::vector<transaction> tr;
    for(int i=0; i<n; i++) {
        int a = rand()%userNum, b=rand()%userNum;
        while(b==a && userNum>1) b = rand()%userNum; // to make sure they don't send money between the same user
        transaction t(users[a].publicKey, users[b].publicKey, rand()%(transNum-1)+1); // money go from 1 to maxTrans
        t.userIn=users+a;
        t.userOut=users+b;
        tr.push_back(t);
    }
    return tr;
}

std::string merkleRoot(std::vector<transaction> tr) {
    std::vector<std::string> hashes, prevHashes;
    int s = tr.size();
    for(auto it=tr.begin(); it!=tr.end(); it++) prevHashes.push_back((*it).hashID);
    while(s>1){
        for(int i=0; i<s; i++) if(i%2==1)
            hashes.push_back(badHash(prevHashes[i]+prevHashes[i-1]));
        if(s%2!=0) {
            hashes.push_back(prevHashes.back());
            s=s/2+1;
        }
        else s/=2;
        prevHashes = hashes;
        hashes.clear();
    }
    return(prevHashes.back());
}

block makeBlock(std::vector<transaction> &transactions){
    block b;
    for(int i=0; i<blockSize; i++){
        auto trLoc = transactions.begin() + rand()%(transactions.size()-1);
        b.transactions.push_back(*trLoc);
        b.transPtr.push_back(trLoc);
        //transactions.erase(trLoc, trLoc+blockSize/10); /// takes every 10 transactions, much faster to erase, though still the biggest time waste of all
    }
    b.merkelHash = merkleRoot(b.transactions);
    return(b);
}

std::string blockToString(block b, unsigned long long int nonce = -1){
    if(nonce==-1) nonce=b.nonce;
    std::string x = std::to_string(b.difficulty)+b.merkelHash+std::to_string(nonce);
    x += b.previousHash+std::to_string(b.timestamp)+std::to_string(b.version);
    return(x);
}

block mineBlock(std::vector<transaction> &tr, user users[], bool &blockMined){
    block b = makeBlock(tr);
    std::string zeros; zeros.assign(b.difficulty, '0');
    for(int i = 0; !blockMined; i++){
        //std::cout << currThread << std::endl;
        std::string currHash = badHash(blockToString(b, i));
        std::string comp = currHash;
        comp.resize(b.difficulty);
        //std::cout << comp << " " << zeros << " " << i << std::endl;
        if(comp == zeros) {
            // set nonce
            b.nonce = i;
            return b;
        }
    }
}

void addBlock(block b, std::vector<transaction> &transactions, user users[]) {
    // add the block to the chain
    b.previousHash = badHash(blockToString(blockchain.back()));
    blockchain.push_back(b);
    // execute transactions
    int trDiff=0;
    auto it = blockchain.back().transactions.begin();
    for(int i=0; i<blockchain.back().transactions.size(); i++){
        std::string all = (*it).inKey + (*it).outKey + std::to_string((*it).sum);
        if(badHash(all) == (*it).hashID){
            if((*it).userIn->balance>=(*it).sum){
                (*it).userIn->balance -= (*it).sum;
                (*it).userOut->balance += (*it).sum;
            }
            else (*it).isValid = false; //tr.push_back(*it); - instead of just putting it back to transaction list, it just invalidates it
        }
        else (*it).isValid = false;
        std::iter_swap(blockchain.back().transPtr[i], transactions.end()-trDiff-1);
        trDiff++;
        it++;
    }
    transactions.erase(transactions.end()-trDiff, transactions.end());
}

int main()
{
    srand(time(NULL));
    blockchain.push_back(block());
    user users[userNum];
    generateUsers(userNum, users);
    std::vector<transaction> transactions = generateTransactions(transNum, users);
    double sumsBefore[userNum];
    for(int i=0; i<userNum; i++) sumsBefore[i]=users[i].balance;

    std::cout << "Thread Nonce Transactions_left\n";
    while(transactions.size()>blockSize) {
        // The block mining competition
        bool blockMined = false;
        const int threadNum = omp_get_max_threads();
        block potentialBlocks[threadNum];
        clock_t timestamp[threadNum]; for(int i=0; i<threadNum; i++) timestamp[i]=std::numeric_limits<clock_t>::max();
        #pragma omp parallel
        {
            int currThread = omp_get_thread_num();
            potentialBlocks[currThread] = mineBlock(transactions, users, blockMined);
            if(!blockMined) {
                timestamp[currThread] = clock();
                blockMined = true;
            }
        }
        int winnerIndex = std::distance(timestamp, std::min_element(timestamp, timestamp+threadNum));
        addBlock(potentialBlocks[winnerIndex], transactions, users);
        std::cout <<std::setw(6)<< winnerIndex <<" "<<std::setw(5)<< blockchain.back().nonce <<" "<< transactions.size() <<std::endl;
    }

    std::cout << "Every 100th user's balance, before and after:\n";
    for(int i=0; i<userNum; i+=100) {
        std::cout << std::setw(6) << sumsBefore[i] << std::setw(6) << users[i].balance << std::endl;
    }
    return 0;
}
