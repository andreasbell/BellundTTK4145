#include "Manager.h"

#define TIMEOUT 5

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
	double weight_distance = e.last_floor - floor;
	weight_distance = (abs(weight_distance)/(weight_distance) - (int)e.direction)? abs(e.last_floor - floor) : 10;


	double weight_state;

	switch (e.current_state){
	case RUN:
		weight_state = 2;
		break;

	case IDLE:
		weight_state = 1;
		break;

	case OPENDOOR:
		weight_state = 2;
		break;

	case EMERGENCY:
		weight_state = 10;
		break;
	}

	double weight_que = e.que.size();

	return weight_distance + weight_state + weight_que;
}