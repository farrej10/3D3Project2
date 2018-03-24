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

int main(int argc, char* argv[])
{
    int stable_count = 0;
    char buf[1024];
    routerFunctions router(argv);
    for (unsigned int i = 0; i < router.RTentries.size(); i++) {
        string sample = router.RTentries.at(i);
        //cout << sample << endl;
    }

    router.start_up();

    for (unsigned int i = 0; i < router.RTentries.size(); i++) {
        string sample = router.RTentries.at(i);
        cout << sample << endl;
    }
    int opensocket = router.createsocket();
    router.findConnections();
    int sendcheck = router.sendDV(opensocket);

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
        //while(bytesIn < 0){
        // bytesIn = recv(opensocket, buf, 1024, 0);

        //}

        // Display message and client info
        char clientIp[256]; // Create enough space to convert the address byte array
        memset(clientIp, 0, 256); // to string of characters

        // Convert from byte array to chars
        inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

        // Display the message / who sent it
        //cout << "Message recv from " << clientIp << " : " << buf << endl;
        string str(buf);
        router.add_entry(str);

        if (router.print == 1) {
            std::cerr << std::endl
                      << "New Table: " << std::endl;
            for (unsigned int i = 0; i < router.RTentries.size(); i++) {
                string sample = router.RTentries.at(i);
                cout << sample << endl;
            }

            router.print = 0;
        }

        sendcheck = router.sendDV(opensocket);
    }

    close(opensocket);
    return 0;
}
