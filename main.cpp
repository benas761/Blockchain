#include "header.h"

const int maxBalance = 10000, maxTrans = 1000, userNum=10, transNum=1000, blockSize = 25; // blocksize<=transNum
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
        transaction t(users[a].publicKey, users[b].publicKey, rand()%(transNum-100)+10); // money go from 10 to 1000
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
        int trLoc = rand()%transNum;
        b.transactions.push_back(transactions[trLoc]);
        transactions.erase(transactions.begin()+trLoc); // these 3 lines took me 2 hours to write...
    }
    b.merkelHash = merkleRoot(b.transactions);
    /*block b(previousHash, transactions, merkelHash, version)
    b.previousHash;
    b.version;*/
    return(b);
}

std::string blockToString(block b){
    std::string x = std::to_string(b.difficulty)+b.merkelHash+std::to_string(b.nonce);
    x += b.previousHash+std::to_string(b.timestamp)+std::to_string(b.version);
    return(x);
}

void mineBlock(std::vector<transaction> &tr, user users[]){
    block b = makeBlock(tr);
    std::string comp, zeros; zeros.assign(b.difficulty, '0');
    while(true){
        std::string currHash = badHash(blockToString(b));
        comp = currHash;
        comp.resize(b.difficulty);
        if(comp == zeros) {
            // add the block to the chain
            b.previousHash = badHash(blockToString(blockchain.back()));
            blockchain.push_back(b);
            // execute transactions
            for(auto it = blockchain.back().transactions.begin(); it!=blockchain.back().transactions.end(); it++){
                std::string all = (*it).inKey + (*it).outKey + std::to_string((*it).sum);
                for(int i=0; i<userNum; i++)
                if(badHash(all) == (*it).hashID){
                    // find both users
                    int user1 = -1, user2 = -1;
                    for(int i = 0; user1 == -1 || user2 == -1 || i<userNum; i++){
                        if(users[i].publicKey==(*it).inKey) user1 = i;
                        else if(users[i].publicKey==(*it).outKey) user2 = i;
                        if(i-1==userNum) std::cout << "problem.\n";
                    }
                    if(users[user1].balance>=(*it).sum){
                        users[user1].balance -= (*it).sum;
                        users[user2].balance += (*it).sum;
                    }
                    else (*it).isValid = false; //tr.push_back(*it); - instead of just putting it back to transaction list, it just invalidates it
                }
                else (*it).isValid = false;
            }
            break;
        }
        else b.nonce++;
    }
}

int main()
{
    srand(time(NULL));
    block genesisBlock;
    blockchain.push_back(genesisBlock);
    user users[userNum];
    generateUsers(userNum, users);
    std::vector<transaction> transactions = generateTransactions(transNum, users);
    mineBlock(transactions, users);
    return 0;
}
