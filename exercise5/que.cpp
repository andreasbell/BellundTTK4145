#include "que.h"

int Que::size(){
	return q.size();
}

Order Que::next(){
	if(this->size() > 0){
		return q[0];
	}
	Order invalid(-1 , EXTERNAL);
	return invalid;
}

void Que::insert(Order order, Elevator){
	if (this->size() == 0){
		q.push_back(order);
		return;
	}
	else if(e.direction == 0 && order.floor == e.last_floor){
		q.insert (q.begin(), order);
		return;
	}
	else{
		int prev = e.last_floor;
		for (int i = 0; i < q.size(); ++i){
			int next = que[i].floor;
			if (prev < next && order.type != INTERNAL_DOWN && order.floor <= next){
				q.insert (q.begin() + i, order);
				return;
			}
			if (prev > next && order.type != INTERNAL_UP && order.floor >= next){
				q.insert (q.begin() + i, order);
				return;
			}
			prev = next;
		}
	}
	q.push_back(order);
}

Order Que::get_order(int index){
    return q[index]; 

void Que::delete_order(){
	if(this->size() > 0){
		q.erase(q.begin());
	}
}
