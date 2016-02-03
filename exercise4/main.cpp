#include <stdio.h>
#include "UDP_Connection.h"
#include "TCP_Connection.h"


int main(){
  char buffer[1024];
  UDP_Connection UDP(20016, "129.241.187.23");
  TCP_Connection TCP(33546);
  TCP.connect_to("129.241.187.23");

  TCP_Connection TCP2(33547);
  TCP.send("Connect to: 129.241.187.147:33547\0", 1024);
  printf("%i \n",TCP2.accept_connection());


  while(0){

    printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);
    
    /*Send message to server*/
    UDP.send(buffer, 1024);

    /*receive message to server*/
    UDP.receive(buffer, 1024);

    printf("Received from server: %s\n",buffer);
  }

  while(0){

    printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);
    
    /*Send message to server*/
    TCP.send(buffer, 1024);

    /*receive message to server*/
    TCP.receive(buffer, 1024);

    printf("Received from server: %s\n",buffer);
  }

  while(1){

    printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);
    
    /*Send message to server*/
    TCP2.send(buffer, 1024);

    /*receive message to server*/
    TCP2.receive(buffer, 1024);

    printf("Received from server: %s\n",buffer);
  }

  return 0;
}