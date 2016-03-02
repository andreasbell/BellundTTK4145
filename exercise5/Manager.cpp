#include "Manager.h"

#define TIMEOUT 5
#define TIME_TRAVEL_BETWEEN_FLOORS 2100
#define TIME_DOOR_OPEN 3000


Manager::Manager(){
	current_state = CONNECTING;
	connected = false;
	timer.start();
}

void Manager::run(){
	switch(current_state){
		case MASTER:
			//Broadcast master status
			//receve and send out command
			//if other masters exist disconnect from slaves
			break;

		case SLAVE:
			//receve and send commands
			if(!connected){
				timer.start();
				current_state = CONNECTING;
			}
			break;

		case CONNECTING:
			//find masters on UDP multicast and connect
			if(connected){
				current_state = SLAVE;
			}
			else if(timer.is_time_out(TIMEOUT)){
				//Become master if no other masters after set number of seconds
				current_state = MASTER;
			}
			break;

	}
}



double cost_function(Elevator e, int floor){
	double duration = 0;
	//function add order to elevator
	if (e.current_state == OPENDOOR){
		duration += TIME_DOOR_OPEN/2.0;
	}
	else if (e.current_state == RUN){
		duration += TIME_TRAVEL_BETWEEN_FLOORS/2.0;
	}
	while(true){
		if (e.should_stop(e.last_floor)){
			duration += TIME_DOOR_OPEN;
			/*clear order*/
			if (e.choose_direction() == DIRN_STOP){
				return duration;
			}
		}
		duration += TIME_TRAVEL_BETWEEN_FLOORS;
		e.last_floor += e.direction;
	}
}