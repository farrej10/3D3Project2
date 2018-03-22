#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sstream>
#include "routerFunctions.h"
#include <string>
#include <thread>
#include <iostream>

routerFunctions::routerFunctions(char * argv[])
{
	stable = false;
    ID = argv[1][0];
	port = atoi(argv[2]);
	nodes = 0;
	char list[] = { 'A','B','C','D','E','F' };
	string sample = "X,X,XXXXX";

	
	for (unsigned int i = 0; i < 6; i++) {
		if (list[i] != ID) {
			sample[0] = list[i];
			RTentries.push_back(sample);
		}
		else {
			sample[0] = list[i];
			sample[2] = '0';
			RTentries.push_back(sample);
			sample[2] = 'X';
		}
	}
}

void routerFunctions::start_up()
{
	char buffer[12];
	FILE *f;
	unsigned int i = 0;
	f = fopen("top.txt", "r");
	if (f != NULL) {
		while (!feof(f)) {
			fgets(buffer, 12, f);
			if (buffer[0] == ID) {
				string a;
				a += buffer[2];
				a += buffer[3];
				a += buffer[10];
				a += buffer[3];
				a += buffer[4];
				a += buffer[5];
				a += buffer[6];
				a += buffer[7];
				a += buffer[8];
				//cout << a << endl;
				//
				for (i = 0; i < RTentries.size(); i++) {
					if (RTentries[i][0] == a[0]) {
						RTentries[i] = a;
						//cout<< "ok" << endl;
					}
				}
			}
		}
	}
}



int routerFunctions::createsocket()
{
	int opensocket = socket(AF_INET, SOCK_DGRAM, 0); //open socket

	if (opensocket == -1) {
		std::cerr << "Error opening socket." << std::endl;
		return -1;
	}


	// clear the socket from the kernal so we don't get an error message if we try to run the server and connect to it again
	int yes = 1;
	if (setsockopt(opensocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		std::cerr << "Error clearing socket from kernal." << std::endl;
		return -1;
	}

	struct sockaddr_in address; //structure containing the address details we are binding to
								//now we set the parameters in that stucture

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");

	int bindsocket = bind(opensocket, (struct sockaddr*) &address, sizeof(address)); //binds the listening socket to a port
	if (bindsocket == -1) {
		std::cerr << "Error binding socket." << std::endl;
		return -1;
	}

	std::cerr << "Socket opened and binded." << std::endl; //output success

	return opensocket;

}

void routerFunctions::add_entry(string DV){
	
	unsigned int i;
	int j = 2;
	int est = 0;
	int dist = 0;
	char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	
	for(i=0 ; i < RTentries.size() ; i++){
	         if(RTentries[i][0] == DV[0] && RTentries[i][2] != 'X'){
				est = (int)RTentries[i][2] - 48;
			}
		}
	for(i=0 ; i < RTentries.size() ; i++){

	        if(DV[j] != 'X'){
					dist = ((int)DV[j] - 48);
		            //cout << "Estimated Distance =" << dist << " " << RTentries[i][0] <<endl;

	            if(dist < ((int)RTentries[i][2] - 48) && RTentries[i][2] != 'X'){

						RTentries[i][2] = digits[dist];
	                // yet to append UDP for next hop
				}
	      		else if( RTentries[i][2] == 'X'){
					RTentries[i][2] = digits[dist];
	                 // yet to append UDP for next hop
							}
					}
			j += 2;
	        
	}




}






