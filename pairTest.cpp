#include <iostream>
#include <fstream>

using namespace std;

int main() {
	ifstream fd("hash.txt");
	int i = 0, count = 0; string h, h0, h1;
	while(fd >> h) {
		if(i%2==0) h0 = h;
		else if(h0==h) count++;
	}
	fd.close();
	cout << count << " pairs were the same.\n";
}