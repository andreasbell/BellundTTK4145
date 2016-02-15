#include "button_poller.h"

void poll_orders(){
	for(int floor = 0; floor < N_FLOORS; floor++){
		for(int button_type = 0; button_type < 3; button_type++){
			if(!(floor == 0 && button_type == BUTTON_CALL_DOWN) && !(floor == N_FLOORS-1 && button_type == BUTTON_CALL_UP) && elev_get_button_signal((elev_button_type_t)button_type, floor)){
				//insertNewOrder(button_type, floor);	
			}	
		}	
	}
}