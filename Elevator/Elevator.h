#pragma once
#include "elev.h"
#include "Timer.h"
#include <mutex>
#include <atomic>

typedef enum { RUN, IDLE, OPENDOOR, EMERGENCY }elevator_state;

class Elevator{
public:
	/*State*/
	std::atomic<elevator_state> current_state;
	std::atomic<elev_motor_direction_t> direction;
	std::atomic<int> last_floor;
	Timer timer;

	/*Elevator functions*/
	void init();
	bool run();

	/*Constructors*/
	Elevator() = default;
	Elevator(const Elevator& e);

	/*Help functions*/
	int next_stop();
	bool get_order(int floor, int button) const;
	void set_order(int floor, int button, bool value);
	bool poll_orders(int& f, elev_button_type_t& t);
	void add_order(int floor, elev_button_type_t type);
	void remove_order(int floor);
	void set_order_lights(bool lights[N_FLOORS][N_BUTTONS]);

private:
	/*Orders*/
	mutable std::mutex order_mutex;
	bool orders[N_FLOORS][N_BUTTONS] = {};

	/*Finite state machine*/
	void fsm_run();
	void fsm_idle();
	void fsm_opendoor();
	void fsm_emergency();
};

void elevator_to_string(char string[], Elevator& elevator);
void string_to_elevator(char string[], Elevator& elevator);
