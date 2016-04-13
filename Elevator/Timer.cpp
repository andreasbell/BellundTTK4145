#include "Timer.h"

bool Timer::is_time_out(int timeLength){
	timer_mutex.lock();
	bool temp = !((startTime + timeLength) >  time(NULL));
	timer_mutex.unlock();
	return temp;
}

void Timer::start(){
	timer_mutex.lock();
	startTime = time(NULL);
	timer_mutex.unlock();
}

int Timer::start_time() const{
	timer_mutex.lock();
	int temp = startTime;
	timer_mutex.unlock();
	return temp;
}

