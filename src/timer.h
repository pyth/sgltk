#ifndef __TIMER_H__
#define __TIMER_H__

#include "app.h"

namespace sgltk {

/**
 * @class Timer
 * @brief Provides a simple timer
 */
class Timer {
	std::chrono::high_resolution_clock::time_point start_time;
public:
	EXPORT Timer();
	EXPORT ~Timer();

	/**
	 * @brief Starts the timer
	 */
	EXPORT void start();
	/**
	 * @brief Starts the timer
	 * @return Time in milliseconds that has passed since start() was called
	 */
	EXPORT double get_time();
};
}

#endif
