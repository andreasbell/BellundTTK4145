#pragma once

class Timer{
private:
	int startTime;
public:
	bool is_time_out(int timeLength);
	void start();
	
};
