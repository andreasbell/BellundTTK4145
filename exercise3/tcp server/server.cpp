/*
    C++ socket server example, handles multiple clients using threads
    check server status:  sudo netstat -ap | grep :8888
*/

#include "server.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>

#define DEFAULT_PORT 8080
 
int server(){
    int socket_server , socket_client;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_server = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_server < 0){
        perror("ERROR CREATING SOCKET");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);
     
    //Bind
    if( bind(socket_server,(struct sockaddr *)&server , sizeof(server)) < 0){
        perror("ERROR BINDING SOCKET");
        return 1;
    }
    puts("bind done");
     
    //Accept and incoming connection
    printf("Waiting for incoming connections...");

    //Listen
    listen(socket_server , 5);
    socklen_t socklen = sizeof(struct sockaddr_in);

    
    while( (socket_client = accept(socket_server, (struct sockaddr *)&client, &socklen)) ){
        printf("Connection accepted");
        std::thread t(connection_handler, socket_client, client);
        t.detach();
    }
     
    if (socket_client < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}



void connection_handler(int sock, struct sockaddr_in client_addr){
    int nBytes;
    char buffer[256];
     
    //Send some messages to the client
    sprintf(buffer, "Greetings! I am your connection handler\n\r");
    write(sock , buffer , strlen(buffer));
     
    //Receive a message from client
    while( (nBytes = recv(sock , buffer, 256 , 0)) > 0 ){
        //Send the message back to client
        buffer[nBytes++] = '\0';
        write(sock, buffer, nBytes);
        printf("Message: %s\n", buffer);
    }

    if(nBytes == 0){
        printf("Client disconnected\n");
    }
    else if(nBytes == -1){
        perror("recv failed");
    }
} 