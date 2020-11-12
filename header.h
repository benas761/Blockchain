#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <chrono>
#include <time.h>
#include <omp.h>
#include <limits>

void midHash(std::string& chr32, long long int x[]);
std::string badHash(std::string in);

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
    user *userIn, *userOut; // for a 30% faster insert, random memory problems may occur
    double sum;
    bool isValid = true;
    transaction(std::string inPublicKey, std::string outPublicKey, int sum);
    operator=(transaction b);
};

class block{
public:
    block() {
        previousHash="";
        merkelHash=badHash("");
    }
    std::string previousHash, merkelHash;
    int version = 0, difficulty = 3;
    unsigned long long int nonce=0;
    std::vector<transaction> transactions;
    std::vector<std::vector<transaction>::iterator> transPtr;
    int timestamp = time(0);
};

#endif // HEADER_H_INCLUDED
