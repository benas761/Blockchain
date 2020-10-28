#include "header.h"

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
    std::string hashID, inKey, outKey;
    int sum;
public:
    bool isValid = true;
    transaction(std::string inPublicKey, std::string outPublicKey, int sum){
        inKey = inPublicKey;
        outKey = outPublicKey;
        this->sum = sum;
        std::string all = inPublicKey + outPublicKey + std::to_string(this->sum);
        ///hashID = hash(all);
        hashID = all; // bad!
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
private:
    std::string previousHash, merkelHash, nonce;
    int version, difficulty = 2; /// change timestamp type later
    std::vector<transaction> transactions;
    int timestamp = time(0);
};

void generateUsers(int n, user users[]) {
    for(int i=0; i<n; i++) {
        int b = rand()%9900+100;
        user t("name"+std::to_string(i), badHash("name"+std::to_string(i)+std::to_string(b)), b);
        users[i]=t;
    }
}

void genereateTransactions(int n) {
    for(int i=0; i<n; i++) {

    }
}

int main()
{
    block genesisBlock;
    user users[10];
    generateUsers(10, users);
    for(int i=0; i<10; i++) std::cout << users[i].publicKey << std::endl;
    return 0;
}
