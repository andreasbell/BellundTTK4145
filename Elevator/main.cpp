
#include <stdio.h>
#include <unistd.h>
#include <thread>

#include "Elevator.h"
#include "Manager.h"

#include <string.h>
#include <errno.h>

using namespace std;
/*
Elevator e;
Elevator test;
char buffer[128];

void thread_1_func(){
	while(true){
		e.run();
	}
}

void thread_2_func(){
	//char buffer[1024];
	while(true){
		//printf("Next Foor: %i \n", e.que.next().floor);
		//fgets(buffer,1024,stdin);
		//e.que.push_back(buffer[0]-'1');
		e.poll_orders();
		int tmp1, tmp2;
		if (tmp1 != e.next_stop() || tmp2 != e.current_state){
			tmp1 = e.next_stop();
			tmp2 = e.current_state;
			elevator_to_string(buffer, e);
			string_to_elevator(buffer, test);
			printf("Next stop: %i State: %i \n", tmp1, tmp2);

		}
	}
}
*/
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
		//printf("ERROR: %s\n", strerror(errno));
		usleep(1000*5000);
		//manager.send_order(BUTTON_CALL_UP, 1);
	}
}

void run_elevator(){
	manager.elevators[manager.ID].init();
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
			manager.send_order(t, f);
		}
		usleep(1000*100);
	}
}



int main(){
	printf("ELEVATOR PROGRAM STARTED\n");

	thread t1(run_manager);
	thread t2(send_status);
	thread t3(run_elevator);
	thread t4(poll_orders);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	return 0;
}
