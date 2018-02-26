#include "routingtable.cpp"
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
int main(){
    routetable a('A');
    for(int i = 0;i < a.RTentries.size(); i++){
        string sample = a.RTentries.at(i);
        cout << sample << endl;
    }
    a.start_up();
    for(int i = 0;i < a.RTentries.size(); i++){
        string sample = a.RTentries.at(i);
        cout << sample << endl;
    }
    a.add_entry("B,X,0,3,X,2,1");
    for( int i = 0;i < a.RTentries.size(); i++){
        string sample = a.RTentries.at(i);
        cout << sample << endl;
    }
    
    return 0;
}