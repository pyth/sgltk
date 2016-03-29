#ifndef __TIMER_H__
#define __TIMER_H__

#include "core.h"

namespace sgltk {

/**
 * @class Timer
 * @brief Provides a simple timer
 */
class Timer {
	unsigned int start_time;
public:
	Timer();
	~Timer();

	/**
	 * @brief Starts the timer
	 */
	void start();
	/**
	 * @brief Starts the timer
	 * @return Time in milliseconds that has passed since start() was called
	 */
	unsigned int get_time();
};
}

#endif
