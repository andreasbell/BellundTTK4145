#pragma once

#include "Elevator.h"
#include "Timer.h"
#include "UDP_Connection.h"
#include <map>

typedef enum {MASTER, SLAVE}manager_state;
typedef enum {UPDATE, NEW_ORDER, PROCESSED_ORDER}message_type;
typedef enum {MSG_ID, MSG_STATE, MSG_TYPE, MSG_CRC, MSG_PAYLOAD}message_frame;

class Manager{
private:
	Timer timer;
	int connected;
	UDP_Connection UDP;


public:
	manager_state current_state;
	std::map<int, Elevator> elevators;
	int ID = 0;

	Manager(int ID);
	void run();
	void send_status();
	void message_handler(char msg[]);
	void find_best_elevator(elev_button_type_t type, int floor, int elev_id);
	void send_order(elev_button_type_t type, int floor, int elevator, message_type order_type);
};

double cost_function(Elevator e, int floor, elev_button_type_t type);