#include "header.h"

const int userNum=10, transNum=100;

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
        nonce="";  /// same
    }
    block(std::string temp) {

    }
    std::string previousHash, merkelHash, nonce;
    int version, difficulty = 2; /// change timestamp type later
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

void generateTransactions(const int n, transaction tr[], user users[]) {
    for(int i=0; i<n; i++) {
        int a = rand()%userNum, b=rand()%userNum;
        while(b==a && userNum>1) b = rand()%userNum; // to make sure they don't send money between the same user
        transaction t(users[a].publicKey, users[b].publicKey, rand()%9990+10); // money go from 10 to 1000
        tr[i] = t;
    }
}

int main()
{
    block genesisBlock;
    user users[userNum];
    transaction transactions[transNum];
    generateUsers(userNum, users);
    generateTransactions(transNum, transactions, users);
    return 0;
}
