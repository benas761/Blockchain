#include "header.h"

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
