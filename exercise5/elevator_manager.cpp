#include "elevator_manager.h"

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