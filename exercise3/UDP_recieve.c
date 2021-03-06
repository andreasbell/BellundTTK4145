/************* UDP CLIENT CODE *******************/
//clear; clang-3.6 -Weverything UDP_recieve.c -o test

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

int main(){
  int Socket, portNum, nBytes;
  char buffer[1024];
  struct sockaddr_in serverAddr;

  /*Create UDP socket*/
  Socket = socket(PF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(20016);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(Socket, (struct sockaddr *)&serverAddr, sizeof serverAddr);

  while(1){
    nBytes = recv(Socket,buffer,1024,0);
    if (nBytes > 0){
    	printf("Received from server: %i, %s\n", nBytes, buffer);
	nBytes = 0;
    }
    /*if (nBytes < 0){
	printf("ERROR: %s\n", strerror(errno));
	close(Socket);
	return 1;
    }*/
  }

  return 0;
}
