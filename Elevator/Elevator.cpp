
#include "Elevator.h"

void Elevator::init(){
	elev_init(); //Returns false if it fails
	direction = DIRN_DOWN;
	elev_set_motor_direction(direction);
	timer.start();
	while (elev_get_floor_sensor_signal() == -1 && !timer.is_time_out(3)){}
	elev_set_motor_direction(DIRN_STOP);
	last_floor = elev_get_floor_sensor_signal();
	elev_set_floor_indicator(last_floor);
	current_state = IDLE;
}

void Elevator::fsm_run(){
	switch (current_state){
	case RUN:
		break;

	case IDLE:
		break;

	case OPENDOOR:
		elev_set_door_open_lamp(0);
		break;

	case EMERGENCY:
		elev_set_stop_lamp(0);
		break;
	}
	elev_set_motor_direction(direction);
	current_state = RUN;
}

void Elevator::fsm_idle(){
	switch (current_state){
	case RUN:
		elev_set_motor_direction(DIRN_STOP);
		break;

	case IDLE:
		break;

	case OPENDOOR:
		elev_set_door_open_lamp(0);
		break;

	case EMERGENCY:
		elev_set_stop_lamp(0);
		elev_set_door_open_lamp(0);
		break;
	}
	current_state = IDLE;
}

void Elevator::fsm_opendoor(){
	switch (current_state){
	case RUN:
		elev_set_motor_direction(DIRN_STOP);
		elev_set_door_open_lamp(1);
		timer.start();
		break;

	case IDLE:
		elev_set_door_open_lamp(1);
		timer.start();
		break;

	case OPENDOOR:
		if (elev_get_obstruction_signal()){
			timer.start();
		}
		if (next_stop() == last_floor){ //only starts timer once
			remove_order(last_floor);
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
	int next = next_stop();

	//Find and set last_floor and floor indicator
	int current_floor = elev_get_floor_sensor_signal();
	if (current_floor >= 0 && current_floor != last_floor){
		last_floor = current_floor;
		elev_set_floor_indicator(last_floor);
	}

	//Elevator statemachine
	switch (current_state){
	case RUN:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (next == current_floor){fsm_opendoor();}
		else if (current_floor >= 0 && next < 0){fsm_idle();}
		else {fsm_run();}
		break;

	case IDLE:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (next == current_floor){fsm_opendoor();}
		else if (next >= 0 && current_floor != next){fsm_run();}
		else {fsm_idle();}
		break;

	case OPENDOOR:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (timer.is_time_out(3) && next >= 0){fsm_run();}
		else if (timer.is_time_out(3) && next < 0){fsm_idle();}
		else {fsm_opendoor();}
		break;

	case EMERGENCY:
		if (!elev_get_stop_signal()){fsm_idle();}
		else {fsm_emergency();}
		break;
	}
	return true;
}

int Elevator::next_stop(){
	if(current_state == IDLE || current_state == OPENDOOR){
		if (orders[last_floor][BUTTON_COMMAND] || orders[last_floor][BUTTON_CALL_DOWN] || orders[last_floor][BUTTON_CALL_UP]){
			return last_floor;
		}
	}

	int loop_position = (direction == DIRN_UP)? last_floor + direction: 2*N_FLOORS - last_floor - direction;
	for(int count = 0; count < 2*N_FLOORS; count++){
		int floor = ((count + loop_position)/N_FLOORS%2)? N_FLOORS -1 - (count + loop_position)%N_FLOORS : (count + loop_position)%N_FLOORS;
		int type = (count + loop_position)/N_FLOORS%2;
		if(orders[floor][type] || orders[floor][BUTTON_COMMAND]){
			direction = (floor < last_floor)? DIRN_DOWN : DIRN_UP;
			return floor;
		}
	}

	return -1;
}

void Elevator::add_order(int floor, elev_button_type_t type){
	orders[floor][type] = true;
}

void Elevator::remove_order(int floor){
	for (int type = 0; type < N_BUTTONS; ++type){
		orders[floor][type] = false;
	}
}

void Elevator::set_order_lights(elev_button_type_t type){
	for (int floor = 0; floor < N_FLOORS; ++floor){
			elev_set_button_lamp(type, floor, orders[floor][type]);
	}
}


bool Elevator::poll_orders(int& f, elev_button_type_t& t){
	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button_type = 0; button_type < N_BUTTONS; button_type++){
			if(!(floor == 0 && button_type == BUTTON_CALL_DOWN) && !(floor == N_FLOORS-1 && button_type == BUTTON_CALL_UP)){
				if((bool)elev_get_button_signal((elev_button_type_t)button_type, floor) && !orders[floor][button_type]){
					f = floor; 
					t = (elev_button_type_t)button_type;
					return true;
				}
			}	
		}	
	}
	return false;
}

void elevator_to_string(char string[], Elevator& elevator){
	string[0] = (signed char)elevator.current_state;
	string[1] = (signed char)elevator.direction;
	string[2] = (signed char)elevator.last_floor;
	for (int f = 0; f < N_FLOORS; f++){
		for (int t = 0; t < N_BUTTONS; t++){
			string[3 + f*N_BUTTONS + t] = elevator.orders[f][t];
		}
	}
}

void string_to_elevator(char string[], Elevator& elevator){
	elevator.current_state = (elevator_state)string[0];
	elevator.direction = (elev_motor_direction_t)string[1];
	elevator.last_floor = (int)string[2];
	for (int f = 0; f < N_FLOORS; f++){
		for (int t = 0; t < N_BUTTONS; t++){
			elevator.orders[f][t] = string[3 + f*N_BUTTONS + t];
		}
	}
}