#pragma once

#include "elev.h"
#include <vector>
#include "Timer.h"

typedef enum { RUN, IDLE, OPENDOOR, EMERGENCY }State;

class Elevator{
public:
	State current_state;
	elev_motor_direction_t direction;
	int last_floor;
	std::vector<int> que;
	Timer timer;

	bool run();
	Elevator();

private:
	void fsm_run();
	void fsm_idle();
	void fsm_opendoor();
	void fsm_emergency();

	int update_last_floor();


};
