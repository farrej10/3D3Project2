#include "routerFunctions.h"
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <ctime>

routerFunctions::routerFunctions()
{
    
}

void routerFunctions::start_up(char* argv[])
{
	connections = 0;
    printNew = 1;
	printOld = 0;
    currentDV = "X,X,X,X,X,X,X,XXXXX";
    stable = false;
    ID = argv[1][0];
    port = atoi(argv[2]);
    nodes = 0;
    char list[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
    string sample = "X,X,XXXXX";

    for (unsigned int i = 0; i < 6; i++) {
        if (list[i] != ID) {
            sample[0] = list[i];
            RTentries.push_back(sample);
        } else {
            sample[0] = list[i];
            sample[2] = '0';
            RTentries.push_back(sample);
            sample[2] = 'X';
        }
    }

	printTable();


    char buffer[12];
    FILE* f;
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
            
                for (i = 0; i < RTentries.size(); i++) {
                    if (RTentries[i][0] == a[0]) {
                        RTentries[i] = a;
                      
                    }
                }
            }
        }
    }
	printNew = 1;
	printTable();
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

    int bindsocket = bind(opensocket, (struct sockaddr*)&address, sizeof(address)); //binds the listening socket to a port
    if (bindsocket == -1) {
        std::cerr << "Error binding socket." << std::endl;
        return -1;
    }

    return opensocket;
}

void routerFunctions::add_entry(string DV)
{
    unsigned int i;
    int j = 2;
    int est = 0;
    int dist = 0;
    char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

	incomingDV = DV;
	printOld = 0;
	
    for (i = 0; i < RTentries.size(); i++) {
        if (RTentries[i][0] == DV[0] && RTentries[i][2] != 'X') {
            est = ((int)RTentries[i][2]) - 48;
        }
    }

    for (i = 0; i < RTentries.size(); i++) {

        if (DV[j] != 'X' || DV[j] == '0') {
            dist = ((int)DV[j] - 48) + est;


            if (dist < ((int)RTentries[i][2] - 48) && RTentries[i][2] != 'X') {
				
				if (printOld == 0){
						printOld = 1;
						printTable();
					}
				
                RTentries[i][2] = digits[dist];
                RTentries[i][4] = DV[14];
                RTentries[i][5] = DV[15];
                RTentries[i][6] = DV[16];
                RTentries[i][7] = DV[17];
                RTentries[i][8] = DV[18];
                printNew = 1;
                // yet to append UDP for next hop
            } else if (RTentries[i][2] == 'X') {

				if (printOld == 0){
						printOld = 1;
						printTable();
					}
                RTentries[i][2] = digits[dist];
                // yet to append UDP for next hop
                RTentries[i][4] = DV[14];
                RTentries[i][5] = DV[15];
                RTentries[i][6] = DV[16];
                RTentries[i][7] = DV[17];
                RTentries[i][8] = DV[18];
                printNew = 1;
            }
        }
        j += 2;
    }
	printOld = 0;
}

int routerFunctions::sendDV(int opensocket)
{

    generateDV();
   
    int counter = 0;

    while (counter < connections) {
    
        sockaddr_in node;
        node.sin_family = AF_INET; // AF_INET = IPv4 addresses
        node.sin_port = htons(ports[counter]); // Little to big endian conversion
        inet_pton(AF_INET, "127.0.0.1", &node.sin_addr); // Convert from string to byte array

        int sendOk = sendto(opensocket, currentDV.c_str(), currentDV.size() + 1, 0, (sockaddr*)&node, sizeof(node));
        if (sendOk == -1) {
            cout << "That didn't work! " << endl;
            return -1;
        }
      
        counter++;
    }

    return 0;
}

void routerFunctions::findConnections()
{
    char X = 'X';
    for (unsigned int i = 0; i < RTentries.size(); i++) {
        char sub = RTentries[i][4];
        if (sub != X) {
            string substring = RTentries[i].substr(4, 8);
            int port = atoi(substring.c_str());
            ports[connections] = port;
            connections++;
        }
    }
}

void routerFunctions::generateDV()
{

    currentDV[0] = ID;
    currentDV[1] = ',';
    currentDV[2] = RTentries[0][2];
    currentDV[3] = ',';
    currentDV[4] = RTentries[1][2];
    currentDV[5] = ',';
    currentDV[6] = RTentries[2][2];
    currentDV[7] = ',';
    currentDV[8] = RTentries[3][2];
    currentDV[9] = ',';
    currentDV[10] = RTentries[4][2];
    currentDV[11] = ',';
    currentDV[12] = RTentries[5][2];
    currentDV[13] = ',';
    string portno = to_string(port);
    currentDV[14] = portno[0];
    currentDV[15] = portno[1];
    currentDV[16] = portno[2];
    currentDV[17] = portno[3];
    currentDV[18] = portno[4];
}

void routerFunctions::printTable(){
	
	
	if (printOld == 1) {

	std::time_t result = std::time(nullptr);
    std::cerr << std::endl << "Timestamp: " << std::asctime(std::localtime(&result));

		            std::cerr << "Old Table: " << std::endl;
	
            for (unsigned int i = 0; i < RTentries.size(); i++) {
                string sample = RTentries.at(i);
				              
				std::cerr << sample << std::endl;
            }
		
		std::cerr << std::endl << "Parsing incoming Distance Vector: " << incomingDV << std::endl;
	}
 	if (printNew == 1) {
            std::cerr << std::endl <<
                       "New Table: " << std::endl;
	
            for (unsigned int i = 0; i < RTentries.size(); i++) {
                string sample = RTentries.at(i);
				              
				std::cerr << sample << std::endl;
            }


            printNew = 0;
        }
	
 

}
