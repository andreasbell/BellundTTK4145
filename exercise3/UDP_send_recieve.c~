/************* UDP CLIENT CODE *******************/
//clear; clang-3.6 -Weverything UDP_send_recieve.c -o test

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
  int socket_in, socket_out, portNum, nBytes;
  char buffer[1024];
  struct sockaddr_in sa_in, sa_out;

  /*Create UDP socket*/
  socket_in = socket(PF_INET, SOCK_DGRAM, 0);
  socket_out = socket(PF_INET, SOCK_DGRAM, 0);


  /*Configure settings in address struct*/
  sa_in.sin_family = AF_INET;
  sa_in.sin_port = htons(20016);
  sa_in.sin_addr.s_addr = htonl(INADDR_ANY);

  sa_out.sin_family = AF_INET;
  sa_out.sin_port = htons(20016);
  sa_out.sin_addr.s_addr = inet_addr("129.241.187.23");



  bind(socket_in, (struct sockaddr *)&sa_in, sizeof sa_in);
  bind(socket_out, (struct sockaddr *)&sa_out, sizeof sa_out);

  while(1){

    printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);

    nBytes = strlen(buffer) + 1;
    

    /*Send message to server*/
    sendto(socket_out,buffer,nBytes,0,(struct sockaddr *)&sa_out, sizeof sa_out);


    nBytes = recv(socket_in,buffer,1024,0);

    printf("Received from server: %s\n",buffer);
  }

  return 0;
}