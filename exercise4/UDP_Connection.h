#include "Network_Connection.h"

#ifndef __UDP_CONNECTION_H__
#define __UDP_CONNECTION_H__

class UDP_Connection: public Network_Connection{

public:
	UDP_Connection();
	UDP_Connection(int port);
	UDP_Connection(int port, char ip[]);

	~UDP_Connection();


	void init(int port, char ip[], bool multicast);

	int send(char message[], int lengt);
	int receive(char message[], int length);
};

#endif 