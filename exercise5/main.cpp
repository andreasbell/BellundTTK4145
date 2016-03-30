
#include <stdio.h>
#include "Elevator.h"
#include <thread>

using namespace std;

Elevator e;

void thread_1_func(){
	Order o(2, INTERNAL);
	e.que.insert(o, e.direction, e.last_floor);

	while(true){
		e.run();
	}
}

void thread_2_func(){
	//char buffer[1024];
	while(true){
		printf("Next Foor: %i \n", e.que.next().floor);
		//fgets(buffer,1024,stdin);
		//e.que.push_back(buffer[0]-'1');
	}
}

int main(){
	printf("HEI\n");

	thread t1(thread_1_func);
	thread t2(thread_2_func);

	t1.join();
	t2.join();

	return 0;
}
