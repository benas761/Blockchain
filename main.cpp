//export PKG_CONFIG_PATH=/home/me/myprefix/lib/pkgconfig
//g++ -o main main.cpp $(pkg-config --cflags --libs libbitcoin) -fopenmp
////////////////////////////////////////////////////////
#include <bitcoin/bitcoin.hpp>
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
    user operator=(user& b){ // not used in this version
        name=b.name; publicKey=b.publicKey; balance=b.balance;
        return *this;
    }
};

class transaction{
public:
    std::string hashID, inKey, outKey;
    user *userIn, *userOut;
    double sum;
    bool isValid = true;
    transaction(std::string inPublicKey, std::string outPublicKey, int sum);
    transaction operator=(transaction b) {
        inKey = b.inKey;
        outKey = b.outKey;
        sum = b.sum;
        std::string all = b.inKey + b.outKey + std::to_string(b.sum);
        hashID = badHash(all);
        return *this;
    }
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
///////////////////////////////////////////////////////////////////
void midHash(std::string& chr32, long long int x[]){
    // seperate into 8 ints

    for(int i=0; i<8; i++) {
        for(int j=i; j<32+i; j+=4)
            x[i]+=chr32[j%32]+chr32[(j+1)%32]*127+chr32[(j+2)%32]*pow(127, 2)+chr32[(j+3)%32]*pow(127, 3);
    } // I should probably comment about this...

    // multiply by different primes
    x[0]*=97; x[1]*=71; x[2]*=67; x[3]*=37;
    x[4]*=19; x[5]*=31; x[6]*=59; x[7]*=23;

    //square the parts
    x[0]*=x[0]; x[1]*=x[1]; x[2]*=x[2]; x[3]*=x[3];
    x[4]*=x[4]; x[5]*=x[5]; x[6]*=x[6]; x[7]*=x[7];

    //get middle 8*1.6 (13) chars
    std::string sx[8];
    for(int i=0; i<8; i++) {
        sx[i]=std::to_string(x[i]); // convert to string
        if(sx[i].length()<13) sx[i].insert(0, 13-sx[i].length(), '0');// makes sure there IS a 13 character long middle
        sx[i].erase(0, (sx[i].length()-13)/2); // get the last characters coming after the 13 in the middle
        sx[i].erase(13);
        x[i]=stoll(sx[i]);
    }
    chr32="";
}

std::string badHash(std::string in){
    long long int x[8];
    for(int i=0; i<8; i++) x[i]=0;

    std::string t = in.substr(0, 32);
    while(t.length()<32) t+="0";
    midHash(t, x);

    for(int i=32; i<in.length(); i+=32){
        t = in.substr(i, 32);
        while(t.length()<32) t+="0";
        midHash(t, x);
    }
    std::string theHash;
    std::stringstream ss;
    ss<<std::setfill('0')<<std::hex;
    for(int i=0; i<8; i++) {
        ss<<std::setw(8)<<x[i];
        std::string t(ss.str());
        t = t.substr(t.length()-8, 8);
        theHash+=t;
        ss.str(std::string());
    }
    return theHash;
}
//////////////////////////////////////////////////////////////

const int maxBalance = 1000000, maxTrans = 100, userNum=1000, transNum=10000, blockSize = 100; // blocksize<=transNum
std::vector<block> blockchain;

bc::hash_digest create_merkle(bc::hash_list& merkle) {
	// Stop if hash list is empty or contains one element
	if(merkle.empty())
		return bc::null_hash;
	else if(merkle.size()==1)
		return merkle[0];
	// While there is more than 1 hash in the list, keep looping...
	while(merkle.size()>1){
		// If number of hashes is odd, duplicate last hash in the list.
		if(merkle.size()%2!=0)
			merkle.push_back(merkle.back());
		// List size is now even
		assert(merkle.size()%2==0);

		// New hash list
		bc::hash_list new_merkle;
		for(auto it = merkle.begin(); it != merkle.end(); it +=2){
			// Join both current hashes together (concatenate).
			bc::data_chunk concat_data(bc::hash_size *2);
			auto concat = bc::serializer<
				decltype(concat_data.begin())>(concat_data.begin());
			concat.write_hash(*it);
			concat.write_hash(*(it +1));
			// Hash both of the hashes.
			bc::hash_digest new_root = bc::bitcoin_hash(concat_data);
			new_merkle.push_back(new_root);
		}
		// This is the new list.
		merkle = new_merkle;
		// DEBUG output ------------------------------------- 
		/*
		std::cout <<"Current merkle hash list:"<< std::endl;
		for(const auto& hash: merkle)
			std::cout <<"  "<< bc::encode_base16(hash)<< std::endl;
        std::cout << std::endl;
        */
        // --------------------------------------------------
    }
    // Finally we end up with a single item.
    return merkle[0];
}

transaction::transaction(std::string inPublicKey, std::string outPublicKey, int sum){
    inKey = inPublicKey;
    outKey = outPublicKey;
    this->sum = sum;
    std::string all = inPublicKey + outPublicKey + std::to_string(this->sum);
    hashID = badHash(all);
}

/*transaction transaction::operator=(transaction b) {
    inKey = b.inKey;
    outKey = b.outKey;
    sum = b.sum;
    std::string all = b.inKey + b.outKey + std::to_string(b.sum);
    hashID = badHash(all);
}*/

void generateUsers(const int n, user users[]) {
    for(int i=0; i<n; i++) {
        int b = rand()%(maxBalance-100)+100;
        std::string name = "name"+std::to_string(i);
        user t(name, badHash(name), b);
        users[i].name = name;
        users[i].publicKey = badHash(name);
        users[i].balance = b;
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
    }
    // prepare to create merkle root by hashing all of block's transactions
    bc::hash_list hlist;
    for(auto it = b.transactions.begin(); it!=b.transactions.end(); it++) {
    	char hID[65];
    	for(int i=0; i<(*it).hashID.length(); i++) hID[i] = (*it).hashID[i];
		hlist.push_back(bc::hash_literal(hID));
    }
    b.merkelHash = bc::encode_base16(create_merkle(hlist));
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
    b.previousHash = badHash(blockToString(blockchain.back()));
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
    return b; // useless, but gets rid of a warning
}

void addBlock(block b, std::vector<transaction> &transactions, user users[]) {
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
        std::iter_swap(blockchain.back().transPtr[i], (transactions.end()-trDiff-1));
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
        std::cout <<std::setw(6)<< winnerIndex <<" "<<std::setw(5)<< blockchain.back().nonce <<" "<<transactions.size()<<std::endl;
    }

    std::cout << "Every 100th user's balance, before and after:\n";
    for(int i=0; i<userNum; i+=100) {
        std::cout << std::setw(8) << sumsBefore[i] << std::setw(8) << users[i].balance << std::endl;
    }
    /*
	std::vector<std::string> vint;
	bc::hash_list hlint;
	for(int i=0; i<100; i++) vint.push_back(badHash(std::to_string(i)));
	for(int i=0; i<100; i++) {
		bc::data_chunk data{'a', 'b', 'c'};
		data.push_back('d');
		bc::hash_digest h = bc::bitcoin_hash(data);
		hlint.push_back(h);
	}
	std::cout << bc::encode_base16(hlint[10]) << std::endl << bc::encode_base16(hlint[11]) << std::endl;*/
    return 0;
}
