#pragma once
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

	bool orders[N_FLOORS][N_BUTTONS] = {}; //{{UP}, {DOWN}, {COMMAND}}

	void init();
	bool run();

	/*Help functions*/
	int next_stop();
	int next_stop_in_direction(elev_motor_direction_t dir, int last_floor);

	bool poll_orders(int& f, elev_button_type_t& t);
	void add_order(int floor, elev_button_type_t type);
	void remove_order(int floor);

private:
	/*Finite state machine*/
	void fsm_run();
	void fsm_idle();
	void fsm_opendoor();
	void fsm_emergency();
};

void elevator_to_string(char string[], Elevator& elevator);
void string_to_elevator(char string[], Elevator& elevator);