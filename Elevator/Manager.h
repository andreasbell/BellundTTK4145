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
	int ID = 0;
	int connected;
	std::map<int, Elevator> elevators;
	UDP_Connection UDP;
	char out_buffer[128];
	char in_buffer[128];


public:
	manager_state current_state;

	Manager();
	void run();
	void message_handler(char msg[]);
	void send_status();
	
};

double cost_function(Elevator e, int floor);