#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

struct routetable{
    char ID;                        // ID of router e.g A/B/C/D/E/F
    vector<string> RTentries;       //vector to store the DVs
    bool stable;                    //boolean to keep track of when the routing table has stablized and gotten all entries
    int nodes;

    void start_up();                //open intial topological file and create the first rendition of the RT
    void add_entry(string DV);      //update table according to incoming DVs    
    routetable(char name);           //constructor
};

/* 
    format for routing table entries
    NODE/COST/NEXT-HOP UDP PORT
    B,3,10001
*/

/*
    format for DV
    SOURCE-NODE/DESTINATION-NODE/COST/UDP
    A,B,3,10001
*/
    