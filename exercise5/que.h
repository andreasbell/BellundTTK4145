#pragma once

#include <vector>
#include "elev.h"

class Order{
public:
	int floor;
	elev_button_type_t type;

	Order(int floor, elev_button_type_t type): floor(floor), type(type){};
};

class Que{
private:
	std::vector<Order> q;
public:
	int size();
    Order get_order(int index);
	Order next();
	void delete_order();
	void insert(Order order, elev_motor_direction_t direction, int last_floor);
};
