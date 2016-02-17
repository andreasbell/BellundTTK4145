#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "Phoenix.h"
#include "Timer.h"

#define DEFAULT_PORT 20016
#define BROADCAST_IP "129.241.187.255"

int main(){
	char buffer[1024];
	int nBytes = 0;

	int sock = socket(PF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
	if(sock < 0){
		printf("Socket error: %i\n", sock);
		return 1;
	}

	struct sockaddr_in sa_in;
	struct sockaddr_in sa_out;


	/*Configure settings in address struct*/
	sa_in.sin_family = AF_INET;
	sa_in.sin_port = htons(DEFAULT_PORT);
	sa_in.sin_addr.s_addr = htonl(INADDR_ANY);

	sa_out.sin_family = AF_INET;
	sa_out.sin_port = htons(DEFAULT_PORT);
	sa_out.sin_addr.s_addr = inet_addr(BROADCAST_IP);

	printf("Bind: %i \n", bind(sock, (struct sockaddr *)&sa_in, sizeof sa_in));

	State current_state = SLAVE;
	Timer timer;
	timer.start();

	printf("Ich bin SLAVE\n");

	while(true){
		switch(current_state){
		case MASTER:
			printf("sendto: %i\n", sendto(sock,"I am your master!\0",10,0,(struct sockaddr *)&sa_out, sizeof sa_out));
			sleep(1);
			break;
		case SLAVE:
			nBytes = recv(sock,buffer,1024,0);
			if (nBytes > 0){
				printf("%s\n", buffer);
				nBytes = 0;
			}
			if (nBytes > 0 && buffer == "I am your master!"){
				timer.start();
			}
			else if(timer.is_time_out(3)){
				current_state = MASTER;
				printf("Ich bin MASTER\n");
				//system("gnome-terminal -e './executable'");

			}

			break;
		}
	}

	return 0;
}

