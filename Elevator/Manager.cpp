#include <fstream>
#include "Manager.h"

#define TIMEOUT 2
#define TIME_TRAVEL_BETWEEN_FLOORS 2100
#define TIME_DOOR_OPEN 3000


Manager::Manager(int ID){
	this->ID = ID;
	set_state(SLAVE);
	timer.start();
}

void Manager::run(){
	char msg[128];
	int size = UDP.receive(msg, 128);
	if(size > 0){
		message_handler(msg, size);
	}
	switch(get_state()){
		case MASTER:
			break;

		case SLAVE:
			if(timer.is_time_out(TIMEOUT)){
				set_state(MASTER);
			}
			break;
	}
}

void Manager::message_handler(char msg[], int length){
	if(msg[MSG_CRC] == CRC(msg, length)){
		//printf("State: %s ID: %i Type: %i \n", msg[MSG_STATE] == MASTER? "M" : "S", msg[MSG_ID], msg[MSG_TYPE]);
		if(msg[MSG_STATE] == MASTER){
			if(get_state() == MASTER && msg[MSG_ID] < ID){set_state(SLAVE);}
			else{timer.start();}
		}

		switch(msg[MSG_TYPE]){
			case STATUS:
				elevators[msg[MSG_ID]].udp_timeout.start();
				string_to_elevator(&msg[MSG_PAYLOAD], elevators[msg[MSG_ID]].elevator);
				break;
			case STATUS_REQUEST:
				if(get_state() == MASTER){
					send_status(msg[MSG_PAYLOAD]);
				}
				break;
			case PROCESSED_ORDER:
				elevators[msg[MSG_PAYLOAD]].elevator.add_order((int)msg[MSG_PAYLOAD +1], (elev_button_type_t)msg[MSG_PAYLOAD + 2]);
				break;
			case NEW_ORDER:
				if(get_state() == MASTER){
					find_best_elevator((elev_button_type_t)msg[MSG_PAYLOAD + 2], (int)msg[MSG_PAYLOAD + 1], (int)msg[MSG_ID]);
				}
				break;
		}
	}
}

void Manager::find_best_elevator(elev_button_type_t type, int floor, int elev_id){
	int best_elev = elev_id;
	int best_time = 100000000;
	if(type != BUTTON_COMMAND){
		for(auto elev = elevators.begin(); elev != elevators.end(); elev++){
			int temp = cost_function(elev->second.elevator, floor, type);
			//printf("Elevator %i Duration %i\n",elev->first, temp);
			if (temp < best_time && !elev->second.udp_timeout.is_time_out(TIMEOUT)){ 
				best_time = temp;
				best_elev = elev->first;
			}
		}
	}
	send_order(type, floor, best_elev, PROCESSED_ORDER);
}

void Manager::send_status(int id){
	char msg[128];
	msg[MSG_ID] = id;
	msg[MSG_STATE] = get_state();
	msg[MSG_TYPE] = STATUS;
	elevator_to_string(&msg[MSG_PAYLOAD], elevators[id].elevator);
	msg[MSG_CRC] = CRC(msg, 128);
	UDP.send(msg, 128);
}

void Manager::send_status_request(int id){
	char msg[6];
	msg[MSG_ID] = this->ID;
	msg[MSG_STATE] = get_state();
	msg[MSG_TYPE] = STATUS_REQUEST;
	msg[MSG_PAYLOAD] = id;
	msg[MSG_CRC] = CRC(msg, 6);
	UDP.send(msg, 6);
}

void Manager::send_order(elev_button_type_t type, int floor, int elevator, message_type order_type){
	char msg[8];
	msg[MSG_ID] = ID;
	msg[MSG_STATE] = get_state();
	msg[MSG_TYPE] = order_type;
	msg[MSG_PAYLOAD] = elevator;
	msg[MSG_PAYLOAD + 1] = floor;
	msg[MSG_PAYLOAD + 2] = (int)type;
	msg[MSG_CRC] = CRC(msg, 8);
	UDP.send(msg, 8);
}

int Manager::CRC(char msg[], int length){
	int crc = 0;
	for (int i = 0; i < length; i++){
		if(i != MSG_CRC){
			crc = (crc + msg[i])%128; 
		}
	}
	return crc; 
}

void Manager::check_timeout(){
	for(auto elev = elevators.begin(); elev != elevators.end(); elev++){
		if (elev->second.udp_timeout.is_time_out(TIMEOUT)){ //Redistribute orders
			for (int i = 0; i < N_FLOORS; ++i){
				if(elev->second.elevator.get_order(i, BUTTON_CALL_UP)){
					elev->second.elevator.set_order(i, BUTTON_CALL_UP, false);
					if(get_state() == MASTER){ find_best_elevator(BUTTON_CALL_UP, i, ID);}
				}
				if(elev->second.elevator.get_order(i, BUTTON_CALL_DOWN)){
					elev->second.elevator.set_order(i, BUTTON_CALL_DOWN, false);
					if(get_state() == MASTER){ find_best_elevator(BUTTON_CALL_DOWN, i, ID);}
				}
			}
		}
	}
}

void Manager::set_state(manager_state state){
	state_mutex.lock();
	current_state = state;
	state_mutex.unlock();
}

manager_state Manager::get_state(){
	manager_state temp_state;
	state_mutex.lock();
	temp_state = current_state;
	state_mutex.unlock();
	return temp_state;
}


void create_file_backup(Elevator& elev){
	std::ofstream file("BACKUP.dat");
	if(!file.fail()){
		char buffer[128];
		elevator_to_string(buffer, elev);
		for (int i = 0; i < 128; i++){
			file << buffer[i];
		}
		file.close();
	} 
}

void recover_file_backup(Elevator& elev){
	std::ifstream file("BACKUP.dat");
	if(!file.fail()){
		char buffer[128];
		int i = 0;
		while (!file.eof()){
			file >> buffer[i++];
		}
		string_to_elevator(buffer, elev);
		file.close();
	} 
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
	int next = e.next_stop();
	e.current_state = RUN;
	while(true){
		//printf("Next: %i Last: %i\n", e.next_stop(), e.last_floor);
		if (next == e.last_floor){
			duration += TIME_DOOR_OPEN;
			e.remove_order(e.last_floor);
			next = e.next_stop();
			if (next < 0){
				return duration;
			}
		}
		duration += TIME_TRAVEL_BETWEEN_FLOORS;
		e.last_floor += e.direction;
	}
}
