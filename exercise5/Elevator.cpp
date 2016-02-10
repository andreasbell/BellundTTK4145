//extern"C"{
//	#include "elev.h"
//}
#include <vector>
#include "Timer.h"
#include "Elevator.h"

Elevator::Elevator(){
	elev_init(); //Returns false if it fails
	elev_set_motor_direction(DIRN_DOWN);
	while (elev_get_floor_sensor_signal() == -1){}
	elev_set_motor_direction(DIRN_STOP);
	last_floor = elev_get_floor_sensor_signal();
}

void Elevator::fsm_run(){
	switch (current_state){
	case RUN:
		break;

	case STOP:
		elev_set_motor_direction(que[0] < last_floor ? DIRN_DOWN : DIRN_UP);
		break;

	case OPENDOOR:
		elev_set_door_open_lamp(0);
		elev_set_motor_direction(que[0] < last_floor ? DIRN_DOWN : DIRN_UP);
		break;

	case EMERGENCY:
		elev_set_stop_lamp(0);
		elev_set_door_open_lamp(0);
		elev_set_motor_direction(que[0] < last_floor ? DIRN_DOWN : DIRN_UP);
		break;
	}
	current_state = RUN;
}

void Elevator::fsm_stop(){
	switch (current_state){
	case RUN:
		elev_set_motor_direction(DIRN_STOP);
		break;

	case STOP:
		break;

	case OPENDOOR:
		elev_set_door_open_lamp(0);
		break;

	case EMERGENCY:
		elev_set_stop_lamp(0);
		elev_set_door_open_lamp(0);
		break;
	}
	current_state = STOP;
}

void Elevator::fsm_opendoor(){
	switch (current_state){
	case RUN:
		elev_set_motor_direction(DIRN_STOP);
		elev_set_door_open_lamp(1);
		//deleteOrder(last_floor);
		timer.start();
		break;

	case STOP:
		elev_set_door_open_lamp(1);
		//deleteOrder(last_floor);
		timer.start();
		break;

	case OPENDOOR:
		if (que[0] == last_floor){ //only starts timer once
			//deleteOrder(last_floor);
			timer.start();
		}
		break;

	case EMERGENCY:
		elev_set_stop_lamp(0);
		elev_set_door_open_lamp(1);
		timer.start();
		break;
	}
	current_state = OPENDOOR;
}

void Elevator::fsm_emergency(){
	if (current_state != EMERGENCY){
		elev_set_motor_direction(DIRN_STOP);
		elev_set_stop_lamp(1);
		if (elev_get_floor_sensor_signal() >= 0){
			elev_set_door_open_lamp(1);
		}
	}
	current_state = EMERGENCY;
}

bool Elevator::run(){
	int current_floor = update_last_floor();

	switch (current_state){
	case RUN:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (que.size() > 0 && current_floor == que[0]){fsm_opendoor();}
		else if (current_floor >= 0 && que.size() == 0){fsm_stop();}
		else {fsm_run();}
		break;

	case STOP:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (que.size() > 0 && current_floor == que[0]){fsm_opendoor();}
		else if (que.size() > 0 && current_floor != que[0]){fsm_run();}
		else {fsm_stop();}
		break;

	case OPENDOOR:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (timer.is_time_out(3) && que.size() > 0){fsm_run();}
		else if (timer.is_time_out(3) && que.size() == 0){fsm_stop();}
		else {fsm_opendoor();}
		break;

	case EMERGENCY:
		if (!elev_get_stop_signal()){fsm_stop();}
		else {fsm_emergency();}
		break;
	}
	
	return true;
}

int Elevator::update_last_floor(){
	int floor = elev_get_floor_sensor_signal();
	if (floor >= 0 && floor != last_floor){
		last_floor = floor;
		elev_set_floor_indicator(floor);

	} 
	return floor;
}

