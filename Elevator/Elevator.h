#pragma once
#include "elev.h"
#include "Timer.h"
#include <mutex>

typedef enum { RUN, IDLE, OPENDOOR, EMERGENCY }elevator_state;

class Elevator{
public:
	/*State*/
	elevator_state current_state;
	elev_motor_direction_t direction;
	int last_floor;
	Timer timer;

	/*Orders*/
	bool orders[N_FLOORS][N_BUTTONS] = {};

	/*Elevator functions*/
	void init();
	bool run();

	/*Help functions*/
	int next_stop();

	bool poll_orders(int& f, elev_button_type_t& t);
	void add_order(int floor, elev_button_type_t type);
	void remove_order(int floor);
	void set_order_lights(elev_button_type_t type);

private:
	/*Finite state machine*/
	void fsm_run();
	void fsm_idle();
	void fsm_opendoor();
	void fsm_emergency();
};

void elevator_to_string(char string[], Elevator& elevator);
void string_to_elevator(char string[], Elevator& elevator);
