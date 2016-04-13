#pragma once

#include <time.h>
#include <mutex>

class Timer{
private:
	int startTime;
	mutable std::mutex timer_mutex;
public:
	bool is_time_out(int timeLength);
	void start();
	int start_time() const;
	Timer(){start();}
	Timer(const Timer &t) : startTime(t.start_time()){}
};

