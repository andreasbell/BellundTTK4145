#include <stdio.h>
#include "UDP_connection.h"


int main(){
  char buffer[1024];
  UDP_connection UDP(20016, "129.241.187.23");

  while(1){

    printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);
    
    /*Send message to server*/
    UDP.send(buffer, 1024);

    /*receive message to server*/
    UDP.receive(buffer, 1024);

    printf("Received from server: %s\n",buffer);
  }

  return 0;
}