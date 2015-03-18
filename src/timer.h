#ifndef __TIMER_H__
#define __TIMER_H__

#include "core.h"

class Timer {
	unsigned int start_time;
public:
	Timer();
	~Timer();

	void start();
	unsigned int get_time();
};

#endif
