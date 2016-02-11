#include "Network_Connection.h"

#ifndef __TCP_CONNECTION_H__
#define __TCP_CONNECTION_H__

class TCP_Connection: public Network_Connection{

public:
	TCP_Connection();
	TCP_Connection(int port);
	void init(int port);

	int connect_to(char ip[]);
	int accept_connection();

	int send(char message[], int lengt);
	int receive(char message[], int length);
};

#endif 