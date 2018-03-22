#pragma once
#include <string>
#include <vector>
#include <iostream>


using namespace std;

class routerFunctions {
public:
	routerFunctions(char * argv[]);
	void start_up();
	void add_entry(string DV);
	int createsocket();
	int port;
	char ID;                        // ID of router e.g A/B/C/D/E/F
	vector<string> RTentries;       //vector to store the DVs
	bool stable;                    //boolean to keep track of when the routing table has stablized and gotten all entries
	int nodes;


};
