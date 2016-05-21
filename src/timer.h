#ifndef __TIMER_H__
#define __TIMER_H__

#include "core.h"

namespace sgltk {

/**
 * @class Timer
 * @brief Provides a simple timer
 */
class Timer {
	std::chrono::high_resolution_clock::time_point start_time;
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
	double get_time();
};
}

#endif
