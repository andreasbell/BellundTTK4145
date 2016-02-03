

#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

typedef enum { RUN, STOP, OPENDOOR, EMERGENCY }State;

class Elevator{
public:
	State get_state() const { return current_state; };
	elev_motor_direction_t get_direction() const { return direction; };
	int get_last_floor() const { return last_floor; };

	bool run();
	Elevator();

private:
	State current_state;
	elev_motor_direction_t direction;
	int last_floor;

	void fsm_run();
	void fsm_stop();
	void fsm_opendoor();
	void fsm_emergency();


};


#endif 