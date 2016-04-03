#pragma once

#include "Elevator.h"
#include "Timer.h"
#include "UDP_Connection.h"
#include <map>

typedef enum {MASTER, SLAVE}manager_state;
typedef enum {UPDATE, ORDER}message_type;

class Manager{
private:
	Timer timer;
	int connected;
	UDP_Connection UDP;
	char out_buffer[128];
	char in_buffer[128];


public:
	manager_state current_state;
	std::map<int, Elevator> elevators;
	int ID = 0;

	Manager(int ID);
	void run();
	void message_handler(char msg[]);
	void send_status();
	void send_order(elev_button_type_t type, int floor);
};

double cost_function(Elevator e, int floor, elev_button_type_t type);