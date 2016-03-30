#include "que.h"

int Que::size(){
	return q.size();
}

Order Que::next(){
	return this->get_order(0);
}

void Que::insert(Order order, elev_motor_direction_t direction, int last_floor){
	if (this->size() == 0){
		q.push_back(order);
		return;
	}
	else if(direction == DIRN_STOP && order.floor == last_floor){
		q.insert (q.begin(), order);
		return;
	}
	else{
		for (int i = 0; i < q.size(); i++){
			if(order.floor == q[i].floor && order.type == q[i].type){
				return;
			}
		}
		int prev = last_floor;
		for (int i = 0; i < q.size(); i++){
			int next = q[i].floor;
			/*
			if (order.type == BUTTON_CALL_DOWN && prev > next && order.floor <= next){
				q.insert (q.begin() + i, order);
				return;
			}
			if (order.type == BUTTON_CALL_UP && prev < next && order.floor >= next){
				q.insert (q.begin() + i, order);
				return;
			}
			if (order.type == BUTTON_COMMAND){
				if(prev < next && order.floor <= next && prev != last_floor){ //going up
					q.insert (q.begin() + i, order);
					return;
				}
				if(prev > next && order.floor >= next && prev != last_floor){ //going down
					q.insert (q.begin() + i, order);
					return;
				}
			*/
				if(prev > next){ //Moving down
					switch (order.type){
						case BUTTON_CALL_DOWN:
						case BUTTON_COMMAND:
							if(order.floor < prev && order.floor >= next){
								q.insert (q.begin() + i, order);
								return;
							} 
					}
				}
				if(prev < next){ //Moving up
					switch (order.type){
						case BUTTON_CALL_UP:
						case BUTTON_COMMAND:
							if(order.floor > prev && order.floor <= next){
								q.insert (q.begin() + i, order);
								return;
							} 
					}
				}
			}
			prev = next;
		}
	}
	q.push_back(order);
}

Order Que::get_order(int index){
    if(this->size() > index){
		return q[index];
	}
	Order invalid(-1 , BUTTON_COMMAND);
	return invalid;
}
void Que::delete_order(){
	if(this->size() > 0){
		q.erase(q.begin());
	}
}
