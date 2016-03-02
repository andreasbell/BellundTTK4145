#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "Phoenix.h"
#include "Timer.h"

#define DEFAULT_PORT 20016
#define BROADCAST_IP "129.241.187.147"

int main(){
	char buffer[1024];
	int nBytes = 0;

	int sock = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
	if(sock < 0){
		printf("Socket error: %i\n", sock);
		return 1;
	}

	int broadcastEnable=1;

	struct sockaddr_in sa_in;
	struct sockaddr_in sa_out;


	/*Configure settings in address struct*/
	sa_in.sin_family = AF_INET;
	sa_in.sin_port = htons(DEFAULT_PORT);
	sa_in.sin_addr.s_addr = htonl(INADDR_ANY);

	sa_out.sin_family = AF_INET;
	sa_out.sin_port = htons(DEFAULT_PORT);
	//sa_out.sin_addr.s_addr = inet_addr(BROADCAST_IP);
	sa_out.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	printf("Bind: %i \n", bind(sock, (struct sockaddr *)&sa_in, sizeof sa_in));

	State current_state = SLAVE;
	Timer timer;
	timer.start();

	int counter = 0;

	printf("Ich bin SLAVE\n");

	while(true){
		switch(current_state){
		case MASTER:
			sprintf(buffer, "I am your master! %i", counter);
			//printf("sendto: %i\n", sendto(sock,buffer,1024,0,(struct sockaddr *)&sa_out, sizeof sa_out));
			//printf("ERROR: %s\n", strerror(errno));
			sendto(sock,buffer,1024,0,(struct sockaddr *)&sa_out, sizeof sa_out);
			printf("%i, %s\n", counter, strerror(errno));
			sleep(1);
			counter++;
			break;
		case SLAVE:
			nBytes = recv(sock,buffer,1024,0);
			if (nBytes > 0 && !strncmp(buffer, "I am your master!", 17)){
				timer.start();
				counter = atoi((&buffer[17]));
				nBytes = 0;
			}
			else if(timer.is_time_out(3)){
				current_state = MASTER;
				printf("Ich bin MASTER\n");
				close(sock);
				system("gnome-terminal -e './executable'");
				sleep(1);
				sock = socket(PF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);

				//Enable UDP broadcast
				int ret=setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
				if(ret){printf("Error: setsockopt call failed"); }

			}
			break;
		}
	}

	close(sock);
	return 0;
}

