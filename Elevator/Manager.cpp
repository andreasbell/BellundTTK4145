#include "Manager.h"

#define TIMEOUT 2
#define TIME_TRAVEL_BETWEEN_FLOORS 2100
#define TIME_DOOR_OPEN 3000


Manager::Manager(){
	current_state = SLAVE;
	connected = false;
	timer.start();
}

void Manager::run(){
	switch(current_state){
		case MASTER:
			if(UDP.receive(in_buffer, 128) > 0){
				message_handler(in_buffer);
				if(in_buffer[0] == 'M'){current_state = SLAVE;}
			}
			break;

		case SLAVE:
			if(UDP.receive(in_buffer, 128) > 0){
				message_handler(in_buffer);
				if(in_buffer[0] == 'M'){timer.start();}
			}
			if(timer.is_time_out(TIMEOUT)){
				current_state = MASTER;
			}
			break;
	}
}

void Manager::message_handler(char msg[]){
	switch(atoi(&msg[4])){
		case UPDATE:
			string_to_elevator(&msg[8], elevators[atoi(&msg[2])]);
			break;

		case ORDER:
			break;

	}
}

void Manager::send_status(){
	switch(current_state){
		case MASTER:
			sprintf(out_buffer, "M,%i,%i", ID, UPDATE);
			break;

		case SLAVE:
			sprintf(out_buffer, "S,%i,%i", ID, UPDATE);
			break;
	}
	elevator_to_string(&out_buffer[8], elevators[ID]);
	UDP.send(out_buffer, 128);
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
		if (e.next_stop() == e.last_floor){
			duration += TIME_DOOR_OPEN;
			e.remove_order(e.last_floor);
			if (e.next_stop() < 0){
				return duration;
			}
		}
		duration += TIME_TRAVEL_BETWEEN_FLOORS;
		e.last_floor += e.direction;
	}
}
