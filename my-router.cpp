#include "routerFunctions.h"
#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <pthread.h>

#define max_threads 10

void *handleTable(void *id);

int main(int argc, char* argv[])
{

    char buf[1024];
    int opensocket;
	int sendcheck;

	routerFunctions router;
    
	router.start_up(argv);   

	opensocket = router.createsocket();

    router.findConnections();

    sendcheck = router.sendDV(opensocket);

    if (sendcheck == -1) {
        std::cerr << "Error sending DV";
		
    }

    sockaddr_in client; // Use to hold the client information (port / ip address)
    int clientLength = sizeof(client); // The size of the client information

    while (router.stable == false) {
        memset(&client, 0, clientLength); // Clear the client structure
        memset(buf, 0, 1024); // Clear the receive buffer

        // Wait for message
        int bytesIn = recv(opensocket, buf, 1024, 0);


        // Display message and client info
        char clientIp[256]; // Create enough space to convert the address byte array
        memset(clientIp, 0, 256); // to string of characters

        // Convert from byte array to chars
        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
  
        string str(buf);
        
		router.add_entry(str);

		router.printTable();
        
        sendcheck = router.sendDV(opensocket);

		if (sendcheck == -1) {
        std::cerr << "Error sending DV";
	    }    

	}

    close(opensocket);
    return 0;
}

void *handleTable(void *id){





}
