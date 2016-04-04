
#include <stdio.h>
#include <unistd.h>
#include <thread>

#include "Elevator.h"
#include "Manager.h"

Manager manager(1);

void run_manager(){
	while(true){
		manager.run();
		usleep(1000);
	}
}

void send_status(){
	while(true){
		manager.send_status();
		usleep(1000*4000);
		manager.send_order(BUTTON_CALL_UP, 2, manager.ID, NEW_ORDER);
	}
}

void run_elevator(){
	//manager.elevators[manager.ID].init();
	while(true){
		manager.elevators[manager.ID].run();
		usleep(1000);
	}
}

void poll_orders(){
	int f;
	elev_button_type_t t;
	while(true){
		if (manager.elevators[manager.ID].poll_orders(f, t)){
			manager.send_order(t, f, manager.ID, NEW_ORDER);
		}
		usleep(1000*100);
	}
}



int main(){
	printf("ELEVATOR PROGRAM STARTED\n");

	std::thread t1(run_manager);
	std::thread t2(send_status);
	std::thread t3(run_elevator);
	std::thread t4(poll_orders);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	return 0;
}
