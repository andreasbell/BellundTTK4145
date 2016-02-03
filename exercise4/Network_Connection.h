#include <netinet/in.h>

class Network_Connection{
public:
	int socket_in;
	int socket_out;
	int portNum;
	int nBytes;

	struct sockaddr_in sa_in;
	struct sockaddr_in sa_out;

	virtual int send(char message[], int lengt) = 0;
	virtual int receive(char message[], int lengt) = 0;
};