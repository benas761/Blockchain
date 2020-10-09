#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <chrono>

class timer{ // for recording time
	std::chrono::high_resolution_clock::time_point start, end;
	std::chrono::duration<double> length;
	public:
    // Konstruktorius, kuris paleidžia laikmatį.
	timer() { start =  std::chrono::high_resolution_clock::now(); }
	// Funkcija, apskaičiuojanti praėjusį laiką.
	void stop() {
		end =  std::chrono::high_resolution_clock::now();
		length = std::chrono::duration_cast<std::chrono::duration<double>>(end-start);
	}
	// Funkcija, išvedanti praėjusį laiką.
	double duration() { return length.count(); }
};

using namespace std; // I don't use any other library anyway, don't judge me!

void midHash(string& chr32, long long int x[]){
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

void BenoHash() {
    cout << "Input type?\n0 - text, 1 - file\n";
    int inputType;
    cin >> inputType;
    if(inputType==1) {
        cout << "Hash the entire file (0), word by word (1) or line by line (2)?\n";
        cin >> inputType;
        inputType++;
    }

    string chr32 = ""; // global, needs to be here
    long long int x[8];
    char inputLetter;
    for(int i=0; i<8; i++) x[i]=0;

    if(inputType==1) {
        string in;
        cout << "Write the input file's name\n";
        cin >> in;
        timer t;
        ifstream fd(in);
        fd >> noskipws;

        while(fd >> inputLetter) {
            // to 32 letters
            chr32 += inputLetter;
            if(chr32.length()==32) {
                midHash(chr32, x);
            }
        }
        t.stop();
        cout << "Time taken:" << t.duration() << "s\n";
    }
    else if(inputType==0) { // through cmd
        // cmd version (further fd -> ss)
        string input;
        cout << "Write the phrase you want to hash (use only one line)\n";
        getline(cin, input);
        getline(cin, input);
        stringstream ss(input);
        ss >> noskipws;

        while(ss >> inputLetter) {
            // to 32 letters
            chr32 += inputLetter;
            if(chr32.length()==32) {
                midHash(chr32, x);
            }
        }
    }
    else if(inputType==2){
        string line, in;
        cout << "Write the input file's name\n";
        cin >> in;
        ifstream fd(in);

        FILE* f;
        f = fopen("hash.txt", "w");

        int i = 0;
        while(fd >> line){
            stringstream ss(line);
            while(ss >> inputLetter) {
                // to 32 letters
                chr32 += inputLetter;
                if(chr32.length()==32) {
                    midHash(chr32, x);
                }
            }
            if(chr32.length() != 0) while(chr32.length() < 32){
                chr32+='0';
            }
            midHash(chr32, x);
            for(int i=0; i<8; i++) { fprintf(f, "%08x", x[i]); }
            fprintf(f, "\n");
            if(i%2==1) { fprintf(f, "\n"); }
            i++;

        }
        fclose(f);
        return;
    }
        else if(inputType==3){// Same as the last one, except for the while condition, because that's not a pain in the ass to read...
        string line, in;
        cout << "Write the input file's name\n";
        cin >> in;
        ifstream fd(in);

        FILE* f;
        f = fopen("hash.txt", "w");

        int i = 0;
        timer cnt;
        while(getline(fd, line)){
            stringstream ss(line);
            while(ss >> inputLetter) {
                // to 32 letters
                chr32 += inputLetter;
                if(chr32.length()==32) {
                    midHash(chr32, x);
                }
            }
            if(chr32.length() != 0) while(chr32.length() < 32){
                chr32+='0';
            }
            midHash(chr32, x);
            for(int i=0; i<8; i++) { fprintf(f, "%08x", x[i]); }
            fprintf(f, "\n");
            if(i%2==1) { fprintf(f, "\n"); }
            i++;

        }
        cnt.stop();
        cout << "Time taken: " << cnt.duration() << "s.\n";
        fclose(f);
        return;
    }

    // if input doesn't have 32, fill with 0s
    if(chr32.length() != 0) while(chr32.length() < 32){
        chr32+='0';
    }
    midHash(chr32, x);

    FILE* f;
    f = fopen("hash.txt", "w");
    for(int i=0; i<8; i++) { printf("%08x", x[i]); fprintf(f, "%08x", x[i]); }
    fclose(f);
    cout << endl;
}

int main()
{
    BenoHash();
    return 0;
}
