#include <netinet/in.h>

#ifndef __NETWORK_CONNECTION_H__
#define __NETWORK_CONNECTION_H__

class Network_Connection{
public:
	int sock;
	int portNum;
	int nBytes;

	struct sockaddr_in sa_in;
	struct sockaddr_in sa_out;

	virtual int send(char message[], int lengt) = 0;
	virtual int receive(char message[], int lengt) = 0;
};

#endif 