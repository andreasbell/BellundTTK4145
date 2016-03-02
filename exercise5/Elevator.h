#pragma once
#include "que.h"
#include "elev.h"
#include <vector>
#include "Timer.h"

typedef enum { RUN, IDLE, OPENDOOR, EMERGENCY }elevator_state;

class Elevator{
public:
	elevator_state current_state;
	elev_motor_direction_t direction;
	int last_floor;
	Timer timer;
    Que que;

	bool run();
	Elevator();

	/*Help functions*/
	int update_last_floor();
	bool should_stop(int floor);
	elev_motor_direction_t choose_direction();

private:
	/*Finite state machine*/
	void fsm_run();
	void fsm_idle();
	void fsm_opendoor();
	void fsm_emergency();
};
