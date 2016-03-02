#pragma once

#include "Elevator.h"
#include "Timer.h"

typedef enum {MASTER, SLAVE, CONNECTING}manager_state;

class Manager{
private:
	Timer timer;
	bool connected;

public:
	manager_state current_state;

	Manager();
	void run();

	
};

double cost_function(Elevator e, int floor);