
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "TCP_Connection.h"


#define DEFAULT_PORT 20016


TCP_Connection::TCP_Connection(){ init(DEFAULT_PORT); }


TCP_Connection::TCP_Connection(int port){ init(port); }

void TCP_Connection::init(int port){
	/*Create TCP socket*/
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	/*Configure settings in address struct*/
	sa_in.sin_family = AF_INET;
	sa_in.sin_port = htons(port);
	sa_in.sin_addr.s_addr = htonl(INADDR_ANY);

	sa_out.sin_family = AF_INET;
	sa_out.sin_port = htons(port);

	bind(sock, (struct sockaddr *)&sa_in, sizeof sa_in);
}

int TCP_Connection::connect_to(char ip[]){
	inet_pton(AF_INET, ip, &sa_out.sin_addr);
	return connect(sock, (struct sockaddr *)&sa_out, sizeof sa_out);
}
int TCP_Connection::accept_connection(){
	listen(sock, 0);
	socklen_t socklen;
	sock = accept(sock, (struct sockaddr *)&sa_out, &socklen);
	return 0;
}

int TCP_Connection::send(char message[], int length){
	/*Send message and return num charracters sent*/
	return sendto(sock,message,length,0,(struct sockaddr *)&sa_out, sizeof(sa_out));
}

int TCP_Connection::receive(char message[], int length){
	/*Receive message and return num charracters recieved */
	//return recvfrom(sock,message,length,0, (struct sockaddr *)&sa_out, sizeof sa_out);
	socklen_t sendsize = sizeof(sa_out);
	return recvfrom(sock,message,length,0, (struct sockaddr *)&sa_out, &sendsize);
}