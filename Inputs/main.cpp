#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>

using namespace std;

int main()
{
    srand(time(0));
    ofstream fr0("0.txt"); // atsitiktinai generuoti 1000 simboliu
    ofstream fr1("1.txt"); // kaip 0, tik skiriasi 500-uoju simboliu
    ofstream fr2("2.txt"); // daug simboliu
    ofstream fr3("3.txt"); // poru testavimas 0
    ofstream fr4("4.txt"); // poru testavimas 1
    for(int i=0; i<1000; i++) {
        char temp = (rand()%(127-33))+33; // normalus ascii simboliai
        fr0 << temp;
        if(i!=500) fr1 << temp;
        else fr1 << temp+1; // vienu simboliu toliau ascii bibliotekoje
    }
    for(int i=0; i<pow(1024, 3); i++) { fr2 << (char)((rand()%(127-33))+33); }
    for(int i=0; i<100000; i++) {
        if(i<25000)  {
            for(int j=0; j<10; j++) {
                fr3 << (char)((rand()%(127-33))+33);
            } fr3 << " ";
            for(int j=0; j<10; j++) {
                fr3 << (char)((rand()%(127-33))+33);
            } fr3 << "\n";
        }
        else if(i<50000)  {
            for(int j=0; j<100; j++) {
                fr3 << (char)((rand()%(127-33))+33);
            } fr3 << " ";
            for(int j=0; j<100; j++) {
                fr3 << (char)((rand()%(127-33))+33);
            } fr3 << "\n";
        }
        else if(i<75000)  {
            for(int j=0; j<500; j++) {
                fr3 << (char)((rand()%(127-33))+33);
            } fr3 << " ";
            for(int j=0; j<500; j++) {
                fr3 << (char)((rand()%(127-33))+33);
            } fr3 << "\n";
        }
        else {
            for(int j=0; j<1000; j++) {
                fr3 << (char)((rand()%(127-33))+33);
            } fr3 << " ";
            for(int j=0; j<1000; j++) {
                fr3 << (char)((rand()%(127-33))+33);
            } fr3 << "\n";
        }
    }
    for(int i=0; i<100000; i++) {
        string temp = "";
        if(i<25000)  {
            for(int j=0; j<10; j++) {
                char t = (char)((rand()%(127-33))+33);
                fr4 << t;
                temp += t;
            } fr4 << " ";
            temp[temp.length()/2]+=1;
            fr4 << temp << "\n";
        }
        else if(i<50000)  {
            for(int j=0; j<100; j++) {
                char t = (char)((rand()%(127-33))+33);
                fr4 << t;
                temp += t;
            } fr4 << " ";
            temp[temp.length()/2]+=1;
            fr4 << temp << "\n";
        }
        else if(i<75000)  {
            for(int j=0; j<500; j++) {
                char t = (char)((rand()%(127-33))+33);
                fr4 << t;
                temp += t;
            } fr4 << " ";
            temp[temp.length()/2]+=1;
            fr4 << temp << "\n";
        }
        else {
            for(int j=0; j<1000; j++) {
                char t = (char)((rand()%(127-33))+33);
                fr4 << t;
                temp += t;
            } fr4 << " ";
            temp[temp.length()/2]+=1;
            fr4 << temp << "\n";
        }
    }

    fr0.close();fr1.close();fr2.close();fr3.close(); fr4.close();
    return 0;
}
