#include "header.h"

const int userNum=10,  transNum=50, blockSize = 25;

class user{
public:
    std::string name, publicKey;
    double balance;
    user(std::string nm, std::string pk, int b) {
        name=nm; publicKey=pk; balance=b;
    }
    user(){}
    operator=(user b){
        name=b.name; publicKey=b.publicKey; balance=b.balance;
    }
};

class transaction{
public:
    std::string hashID, inKey, outKey;
    int sum;

    bool isValid = true;
    transaction(std::string inPublicKey, std::string outPublicKey, int sum){
        inKey = inPublicKey;
        outKey = outPublicKey;
        this->sum = sum;
        std::string all = inPublicKey + outPublicKey + std::to_string(this->sum);
        hashID = badHash(all); /// bad! you stupid piece of shit, learn what public keys are already...
    }
    transaction() {}
    operator=(transaction b) {
        inKey = b.inKey;
        outKey = b.outKey;
        sum = b.sum;
        std::string all = b.inKey + b.outKey + std::to_string(b.sum);
        hashID = badHash(all);
    }
};

class block{
public:
    block() {
        previousHash="";
        merkelHash=""; /// something needs to happen here
        nonce=0;  /// same
    }
    block(std::string previousHash) {

    }
    std::string previousHash, merkelHash;
    int version, difficulty = 3;
    unsigned long long int nonce=0;
    std::vector<transaction> transactions;
    int timestamp = time(0);
};

void generateUsers(const int n, user users[]) {
    for(int i=0; i<n; i++) {
        int b = rand()%9900+100;
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
        transaction t(users[a].publicKey, users[b].publicKey, rand()%9990+10); // money go from 10 to 1000
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

void mineBlock(std::string prevHash, std::vector<transaction> &tr){
    block b = makeBlock(tr);
    std::string comp, zeros; zeros.assign(b.difficulty, '0');
    while(true){
        std::string currHash = badHash(std::to_string(b.difficulty)+b.merkelHash+std::to_string(b.nonce)+b.previousHash+std::to_string(b.timestamp)+std::to_string(b.version));
        comp = currHash;
        comp.resize(b.difficulty);
        if(comp == zeros) {
            break;
        }
        else b.nonce++;
    }
}

int main()
{
    block genesisBlock;
    user users[userNum];
    std::vector<transaction> transactions = generateTransactions(transNum, users);
    generateUsers(userNum, users);
    mineBlock(badHash("a"), transactions);
    return 0;
}
