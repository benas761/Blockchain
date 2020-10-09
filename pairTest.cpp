#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

void For5(string input) {
	ifstream fd(input);
	int i = 0, count = 0; string h, h0, h1;
	while(fd >> h) {
		if(i%2==0) h0 = h;
		else if(h0==h) count++;
		i++;
	}
	fd.close();
	cout << count << " pairs were the same.\n";
}

void For6hex(string input) {
	ifstream fd(input);
	int i = 0; string h, h0, h1;
	float diff=0, diffPerc=0, mn=999, mx=0;
	while(fd >> h) {
		if(i%2==0) h0 = h;
		else {
            for(int j=0; j<h.length(); j++) if(h0[j]==h[j]) diff++;
            diffPerc += diff/h.length()*100;
            mn = (mn>diff/h.length()*100) ? diff/h.length()*100 : mn;
            mx = (mx<diff/h.length()*100) ? diff/h.length()*100 : mx;
            diff = 0;
		}
		i++;
	}
	diffPerc /= i/2;
	fd.close();
	printf("Average: %f%%, Minimum: %f%%, Maximum: %f%%.", diffPerc, mn, mx);
}

void For6bit(string input) {
	ifstream fd(input);
	int i = 0; string h, h0, h1;
	float diff=0, diffPerc, mn=999, mx=0;
	while(fd >> h) {
		if(i%2==0) h0 = h;
		else {
            for(int j=0; j<h.length(); j++) {
                bitset<8> x = h[j];
                bitset<8> y = h0[j];
                for(int k=0; k<8; k++) if(x[k]==y[k]) diff++;
            }
            diffPerc += diff/(h.length()*8)*100;

            mn = (mn>diff/(h.length()*8)*100) ? diff/(h.length()*8)*100 : mn;
            mx = (mx<diff/(h.length()*8)*100) ? diff/(h.length()*8)*100 : mx;
            diff = 0;
		}
		i++;
	}
	diffPerc /= i/2;
	fd.close();
	printf("Average: %f%%, Minimum: %f%%, Maximum: %f%%.", diffPerc, mn, mx);
}

int main() {
	For6hex("hashRand.txt");
}
