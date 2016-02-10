#include"Timer.h"
#include <time.h>

static int startTime;

int isTimeOut(int timeLength)
{
	return !((startTime + timeLength) >  time(NULL));
}

void startTimer()
{
	startTime = time(NULL);
}


