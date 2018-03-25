#pragma once
#include <string>
#include <vector>
#include <iostream>


using namespace std;

class routerFunctions {
public:
	routerFunctions();
	void start_up(char* argv[]);
	void add_entry(string DV);
	void findConnections();
	void generateDV();	
	void printTable();	
	int createsocket();
	int sendDV(int opensocket);
	

	int port;
	int opensocket;
	char ID;                        // ID of router e.g A/B/C/D/E/F
	vector<string> RTentries;       //vector to store the DVs
	bool stable;                    //boolean to keep track of when the routing table has stablized and gotten all entries
	int nodes;
	int connections;
	int ports[6];
	string currentDV;
	string incomingDV;
	int printOld;	
	int printNew;
};
