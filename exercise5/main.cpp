
#include <stdio.h>

#include "Elevator.h"

int main(){
	printf("HEI\n");
	
	Elevator e;
	e.que.push_back(2);
	e.que.push_back(0);
	while(true){
		e.run();
	}
	return 0;
}
