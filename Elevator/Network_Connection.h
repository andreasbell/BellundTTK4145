#ifndef __NETWORK_CONNECTION_H__
#define __NETWORK_CONNECTION_H__

#include <netinet/in.h>

class Network_Connection{
public:
	int sock;
	int portNum;

	bool error = false;


	struct sockaddr_in sa_in;
	struct sockaddr_in sa_out;

	virtual int send(char message[], int lengt) = 0;
	virtual int receive(char message[], int lengt) = 0;
};

#endif 