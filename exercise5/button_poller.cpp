#include "button_poller.h"
#include <stdio.h> 
bool button_matrix[N_FLOORS][3] = {};

void poll_orders(Elevator& e){
	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button_type = 0; button_type < 3; button_type++){
			if(!(floor == 0 && button_type == BUTTON_CALL_DOWN) && !(floor == N_FLOORS-1 && button_type == BUTTON_CALL_UP)){
				if((bool)elev_get_button_signal((elev_button_type_t)button_type, floor) != button_matrix[floor][button_type]){
					button_matrix[floor][button_type] = !button_matrix[floor][button_type];
					if(button_matrix[floor][button_type]){
						//insertNewOrder
						Order o(floor, (elev_button_type_t)button_type);
						e.que.insert(o, e.direction, e.last_floor);
						printf("num orders: %i \n", e.que.size());
					}
				}
			}	
		}	
	}
}