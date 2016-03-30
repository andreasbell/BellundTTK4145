#pragma once

#include <vector>
#include "elev.h"

typedef enum { INTERNAL, EXTERNAL_UP, EXTERNAL_DOWN }order_type;

class Order{
public:
	int floor;
	order_type type;

	Order(int floor, order_type type): floor(floor), type(type){};
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
