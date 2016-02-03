#include "Elevator.h"
#include "elev.h"

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
		startElevatorInCorrectDirection();
		break;

	case OPENDOOR:
		elev_set_door_open_lamp(0);
		startElevatorInCorrectDirection();
		break;

	case EMERGENCY:
		elev_set_stop_lamp(0);
		elev_set_door_open_lamp(0);
		startElevatorInCorrectDirection();
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
	updateLastFloor(); //Setts last floor so it can be used in switch
	switch (current_state){
	case RUN:
		elev_set_motor_direction(DIRN_STOP);
		elev_set_door_open_lamp(1);
		deleteOrder(last_floor);
		setOrderLights();
		startTimer();
		break;

	case STOP:
		elev_set_door_open_lamp(1);
		deleteOrder(last_floor);
		startTimer();
		break;

	case OPENDOOR:
		deleteOrder(last_floor);
		setOrderLights();
		if (getNextStop(last_floor) == last_floor){ //only starts timer once
			startTimer();
		}
		break;

	case EMERGENCY:
		elev_set_stop_lamp(0);
		elev_set_door_open_lamp(1);
		startTimer();
		break;
	}
	current_state = OPENDOOR;
}

void Elevator::fsm_emergency()
{
	deleteAllOrders();
	setOrderLights();
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
	updateLastFloor();

	if (detectNewOrder()){
		setOrderLights();
	}

	switch (current_state){
	case RUN:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (getNextStop(getLastFloor()) == elev_get_floor_sensor_signal()){fsm_opendoor();}
		else if (elev_get_floor_sensor_signal() >= 0 && getNextStop(getLastFloor()) < 0){fsm_stop();}
		else {fsm_run();}
		break;

	case STOP:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (elev_get_floor_sensor_signal() >= 0 && getNextStop(getLastFloor()) == elev_get_floor_sensor_signal()){fsm_opendoor();}
		else if (getNextStop(getLastFloor()) >= 0 && getNextStop(getLastFloor()) != elev_get_floor_sensor_signal()){fsm_run();}
		else {fsm_stop();}
		break;

	case OPENDOOR:
		if (elev_get_stop_signal()){fsm_emergency();}
		else if (isTimeOut(3) && (getNextStop(getLastFloor()) >= 0)){fsm_run();}
		else if (isTimeOut(3) && (getNextStop(getLastFloor()) < 0)){fsm_stop();}
		else {fsm_opendoor();}
		break;

	case EMERGENCY:
		if (!elev_get_stop_signal()){fsm_stop();}
		else {fsm_emergency();}
		break;
	}
	
	return true;
}