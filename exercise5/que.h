#pragma once

#include <vector>
#include "Elevator.h"

typedef enum { EXTERNAL, INTERNAL_UP, INTERNAL_DOWN }order_type;

class Order{
public:
	int floor;
	order_type type;

	order(int floor, order_type type): floor(floor), type(type);
};

class Que{
private:
	std::vector<Order> q;
public:
	int size();
    int get_order(int index);
	Order next();
	void delete_order();
	void insert(Order order, Elevator e);
};
