#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

void midHash(string& chr32, long long int x[]){
    // seperate into 8 ints
    cout << chr32 << endl;
    /// v1
    /*for(int i=0; i<8; i++)
        for(int j=0; j<4; j++)
            x[i]+=chr32[i*4+j]*pow(127, j);
*/
    ///v2
    for(int i=0; i<8; i++) {
        for(int j=i; j<32+i; j+=4)
            x[i]+=chr32[j%32]+chr32[(j+1)%32]*127+chr32[(j+2)%32]*pow(127, 2)+chr32[(j+3)%32]*pow(127, 3);

        /*int j = k;
        while(j<32+k) {
            cout << x[i] << "+" << chr32[i*4+(j%32)] << "=";
            x[i]+=chr32[i*4+(j%32)]*pow(127, (j-k)%4);
            cout << x[i] << endl;
            j++;
        }
        k++;
        cout << endl;*/
    }

    // multiply by different primes
    x[0]*=97; x[1]*=71; x[2]*=67; x[3]*=37;
    x[4]*=19; x[5]*=31; x[6]*=59; x[7]*=23;

    //square the parts
    x[0]*=x[0]; x[1]*=x[1]; x[2]*=x[2]; x[3]*=x[3];
    x[4]*=x[4]; x[5]*=x[5]; x[6]*=x[6]; x[7]*=x[7];

    //get middle 8*1.6 (13) chars

    string sx[8];
    for(int i=0; i<8; i++) {
        sx[i]=to_string(x[i]); // convert to string
        if(sx[i].length()<13) sx[i].insert(0, 13-sx[i].length(), '0');// makes sure there IS a 13 character long middle
        sx[i].erase(0, (sx[i].length()-13)/2); // get the last characters coming after the 13 in the middle
        sx[i].erase(13);
        x[i]=stoll(sx[i]);
    }

    chr32="";
}

void HorribleHash(string in) {
    ifstream fd(in);
    fd >> noskipws;

    char inputLetter;
    string chr32 = "";
    long long int x[8];
    for(int i=0; i<8; i++) x[i]=0;
    while(fd >> inputLetter) {
        // to 32 letters
        chr32 += inputLetter;
        if(chr32.length()==31) {
            cout << chr32 << " - str\n";
            midHash(chr32, x);
        }
    }
    // if input doesn't have 32, fill with 0s
    while(chr32.length() < 32){
        chr32+='0';
    }
    midHash(chr32, x);

    for(int i=0; i<8; i++) printf("%08x ", x[i]);
    cout << endl;
}

int main()
{
    HorribleHash("input0.txt");
    HorribleHash("input1.txt"); // YAAAAAAAAAAAASSSSSSSSSSS!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    return 0;
}
