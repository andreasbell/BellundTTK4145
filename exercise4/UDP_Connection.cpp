
#include <sys/socket.h>
#include <netinet/in.h>
#include "UDP_connection.h"
#include <arpa/inet.h>
#include <netinet/in.h>

#define DEFAULT_PORT 20016
#define BROADCAST_IP "129.241.187.255"


UDP_connection::UDP_connection(): UDP_connection(DEFAULT_PORT, BROADCAST_IP) {
}

UDP_connection::UDP_connection(int port, char ip[]){
	/*Create UDP socket*/
	socket_in = socket(PF_INET, SOCK_DGRAM, 0);
  	socket_out = socket(PF_INET, SOCK_DGRAM, 0);


	/*Configure settings in address struct*/
	sa_in.sin_family = AF_INET;
	sa_in.sin_port = htons(port);
	sa_in.sin_addr.s_addr = htonl(INADDR_ANY);

	sa_out.sin_family = AF_INET;
	sa_out.sin_port = htons(port);
	sa_out.sin_addr.s_addr = inet_addr(ip);

	bind(socket_in, (struct sockaddr *)&sa_in, sizeof sa_in);
	bind(socket_out, (struct sockaddr *)&sa_out, sizeof sa_out);
}

int UDP_connection::send(char message[], int length){
	/*Send and return num charracters sent*/
	return sendto(socket_out,message,length,0,(struct sockaddr *)&sa_out, sizeof sa_out);
}

int UDP_connection::receive(char message[], int length){
	/*Receive message and return num charracters recieved */
	return recv(socket_in,message,length,0);
}