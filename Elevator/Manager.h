#pragma once

#include "Elevator.h"
#include "Timer.h"
#include "UDP_Connection.h"
#include <map>
#include <utility>
#include <mutex>
#include <atomic>

typedef enum {MASTER, SLAVE}manager_state;
typedef enum {STATUS, STATUS_REQUEST, NEW_ORDER, PROCESSED_ORDER}message_type;
typedef enum {MSG_ID, MSG_STATE, MSG_TYPE, MSG_CRC, MSG_PAYLOAD}message_frame;

struct elev_timer_pair {
	Elevator elevator;
	Timer udp_timeout;
};

class Manager{
private:
	/*Manager state*/
	Timer timer;
	manager_state current_state;
	std::mutex state_mutex;

	/*UDP multicast connection*/
	UDP_Connection UDP;

public:
	/*Map over all elevators on network*/
	std::map<int, elev_timer_pair> elevators;

	/*Manager ID*/
	std::atomic<int> ID;

	Manager(int ID);
	void run();

	/*Messaging functions*/
	void send_status(int id);
	void send_status_request(int id);
	void send_order(elev_button_type_t type, int floor, int elevator, message_type order_type);
	void message_handler(char msg[], int length);

	/*Help functions*/
	void set_state(manager_state);
	manager_state get_state();
	void find_best_elevator(elev_button_type_t type, int floor, int elev_id);
	int CRC(char msg[], int length);
	void check_timeout();
};

double cost_function(Elevator e, int floor, elev_button_type_t type);
void create_file_backup(Elevator& elev);
void recover_file_backup(Elevator& elev);
