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
	* @brief Returns the time in seconds since the timer was started
	* @return Time in seconds that has passed since start was called
	*/
	EXPORT double get_time_s();
	/**
	 * @brief Returns the time in milliseconds since the timer was started
	 * @return Time in milliseconds that has passed since start was called
	 */
	EXPORT double get_time_ms();
};
}

#endif
