

#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

extern "C"{
#include "elev.h"
}
#include <vector>
#include "Timer.h"


typedef enum { RUN, STOP, OPENDOOR, EMERGENCY }State;

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
	void fsm_stop();
	void fsm_opendoor();
	void fsm_emergency();

	int update_last_floor();


};


#endif 