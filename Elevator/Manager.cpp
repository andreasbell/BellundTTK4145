#include "Manager.h"

#define TIMEOUT 2
#define TIME_TRAVEL_BETWEEN_FLOORS 2100
#define TIME_DOOR_OPEN 3000


Manager::Manager(int ID){
	this->ID = ID;
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
	printf("%s\n", msg);
	switch(atoi(&msg[4])){
		case UPDATE:
			if(atoi(&msg[2]) != ID){
				string_to_elevator(&msg[8], elevators[atoi(&msg[2])]);
			}
			break;

		case ORDER:
			if( current_state == MASTER && msg[0] == 'N'){
				int floor = atoi(&msg[6]);
				int type = atoi(&msg[8]);
				if(current_state == MASTER){
					int best_elev = 100000000;
					int best_time = 100000000;
					for(auto elev = elevators.begin(); elev != elevators.end(); elev++){
						int temp = cost_function(elev->second, floor, (elev_button_type_t)type);
						if (temp < best_time){ 
							best_time = temp;
							best_elev = elev->first;
						}
					}
					char order[16];
					sprintf(order, "O,%i,%i,%i,%i", best_elev, ORDER, floor, (int)type);
					UDP.send(order, 16);
				}
			}
			else if (msg[0] == 'O'){
				elevators[atoi(&msg[2])].add_order(atoi(&msg[6]), (elev_button_type_t)atoi(&msg[8]));
			}
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

void Manager::send_order(elev_button_type_t type, int floor){
	char order[16];
	sprintf(order, "N,%i,%i,%i,%i", ID, ORDER, floor, (int)type);
	UDP.send(order, 16);
}


double cost_function(Elevator e, int floor, elev_button_type_t type){
	double duration = 0;
	e.add_order(floor, type);
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



