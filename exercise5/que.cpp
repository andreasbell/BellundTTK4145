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
		int prev = last_floor;
		for (int i = 0; i < q.size(); ++i){
			int next = q[i].floor;
			if (prev < next && order.type != EXTERNAL_DOWN && order.floor <= next){
				q.insert (q.begin() + i, order);
				return;
			}
			if (prev > next && order.type != EXTERNAL_UP && order.floor >= next){
				q.insert (q.begin() + i, order);
				return;
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
	Order invalid(-1 , INTERNAL);
	return invalid;
}
void Que::delete_order(){
	if(this->size() > 0){
		q.erase(q.begin());
	}
}
