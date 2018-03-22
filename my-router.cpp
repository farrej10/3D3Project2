#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <thread>
#include <iostream>
#include "routerFunctions.h"




int main(int argc, char* argv[])
{
	
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

	if (int opensocket = router.createsocket() < 0) {
		std::cerr << "Error creating or binding socket";
		return 1;
	}
	

	return 0;

}
