
#include <sys/socket.h>
#include <netinet/in.h>
#include "UDP_Connection.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define DEFAULT_PORT 20016


UDP_Connection::UDP_Connection(){
	init(DEFAULT_PORT, "0.0.0.0", true);
}

UDP_Connection::UDP_Connection(int port, char ip[]){
	init(port, ip, false); 
}

UDP_Connection::UDP_Connection(int port){
	init(port, "0.0.0.0", true); 
}

UDP_Connection::~UDP_Connection(){
	close(sock);
}

void UDP_Connection::init(int port, const char ip[], bool multicast){
	/*Close open socket*/
	//close(sock);

	/*Create UDP socket*/
	if((sock = socket(PF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0)) < 0){error = true;}

	if (multicast){
		broadcastEnable = 1;
		setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	}

	/*Configure settings in address struct*/
	sa_in.sin_family = AF_INET;
	sa_in.sin_port = htons(port);
	sa_in.sin_addr.s_addr = htonl(INADDR_ANY);

	sa_out.sin_family = AF_INET;
	sa_out.sin_port = htons(port);
	sa_out.sin_addr.s_addr = (multicast)? htonl(INADDR_BROADCAST) : inet_addr(ip);

	if(bind(sock, (struct sockaddr *)&sa_in, sizeof sa_in) < 0){error = true;}
}

int UDP_Connection::send(char message[], int length){
	/*Send and return num charracters sent*/
	return sendto(sock,message,length,0,(struct sockaddr *)&sa_out, sizeof sa_out);
}

int UDP_Connection::receive(char message[], int length){
	/*Receive message and return num charracters recieved */
	return recv(sock,message,length,0);
}