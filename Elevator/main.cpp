
#include <stdio.h>
#include <unistd.h>
#include <thread>

#include "Elevator.h"
#include "Manager.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int get_ip(){
    struct ifreq ifr;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);

    /* Las byte of IP */
    return (((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr >> 8*3)%128;
}

Manager manager(get_ip());

void run_manager(){
	while(true){
		manager.run();
		usleep(1000);
	}
}

void check_and_send_status(){
	manager.send_status_request(manager.ID);
	usleep(1000*200);
	while(true){
		manager.send_status(manager.ID);
		manager.check_timeout();
		usleep(1000*200);
	}
}

void run_elevator(){
	manager.elevators[manager.ID].first.init();
	while(true){
		manager.elevators[manager.ID].first.run();
		usleep(1000);
	}
}

void poll_orders_and_set_order_lights(){
	int f;
	elev_button_type_t t;
	while(true){
		if (manager.elevators[manager.ID].first.poll_orders(f, t)){
			manager.send_order(t, f, manager.ID, NEW_ORDER);
		}
		for(auto elev = manager.elevators.begin(); elev != manager.elevators.end(); ++elev){
			elev->second.first.set_order_lights(BUTTON_CALL_UP);
			elev->second.first.set_order_lights(BUTTON_CALL_DOWN);
			if(elev->first == manager.ID){
				elev->second.first.set_order_lights(BUTTON_COMMAND);
			}
		}
		usleep(1000*100);
	}
}

void print_status(){
	printf("\n");
	while(true){
		int num_elevators = 1;
		printf("Current state: %s ID: %i  \n", manager.current_state == MASTER? "MASTER" : "SLAVE", manager.ID);
		for(auto elev = manager.elevators.begin(); elev != manager.elevators.end(); elev++){
			printf("\tID: %i ", elev->first);
			printf("Timeout: %s ", elev->second.second.is_time_out(2)? "yes" : "no");
			printf("Last floor: %i ", elev->second.first.last_floor);
			printf("Next floor: %i ", elev->second.first.next_stop());
			printf("      \n");
			num_elevators++;
		}
		for (int i = 0; i < num_elevators; ++i){
			printf("\x1b[A");
		}

		usleep(1000*200);
	}
}



int main(){
	printf("ELEVATOR PROGRAM STARTED\n");

	std::thread t1(run_manager);
	std::thread t2(check_and_send_status);
	std::thread t3(run_elevator);
	std::thread t4(poll_orders_and_set_order_lights);
	std::thread t5(print_status);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	return 0;
}
