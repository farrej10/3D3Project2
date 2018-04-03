#pragma once
#include <string>
#include <vector>
#include <iostream>

class routerFunctions {
public:
	routerFunctions(char * argv[]);
	void start_up();
	void add_entry(std::string DV);
	void findConnections();
	void generateDV();	
	int createsocket();
	int port;
	char ID;                        // ID of router e.g A/B/C/D/E/F
	std::vector<std::string> RTentries;       //vector to store the DVs
	bool stable;                    //boolean to keep track of when the routing table has stablized and gotten all entries
	int nodes;
	int sendDV(int opensocket);
	int connections;
	int ports[6];
	std::string currentDV;
	int print;
};
