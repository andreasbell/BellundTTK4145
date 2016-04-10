#pragma once

#include "Elevator.h"
#include "Timer.h"
#include "UDP_Connection.h"
#include <map>
#include <utility>

typedef enum {MASTER, SLAVE}manager_state;
typedef enum {STATUS, STATUS_REQUEST, NEW_ORDER, PROCESSED_ORDER}message_type;
typedef enum {MSG_ID, MSG_STATE, MSG_TYPE, MSG_CRC, MSG_PAYLOAD}message_frame;

struct elev_timer_pair {
	Elevator elevator;
	Timer udp_timeout;
};

class Manager{
private:
	Timer timer;
	int connected;
	UDP_Connection UDP;


public:
	manager_state current_state;
	std::map<int, elev_timer_pair> elevators;
	int ID = 0;

	Manager(int ID);
	void run();
	void send_status(int id);
	void send_status_request(int id);
	void message_handler(char msg[], int length);
	void find_best_elevator(elev_button_type_t type, int floor, int elev_id);
	void send_order(elev_button_type_t type, int floor, int elevator, message_type order_type);
	int CRC(char msg[], int length);
	void check_timeout();
};

double cost_function(Elevator e, int floor, elev_button_type_t type);