#include "timer.h"

using namespace sgltk;

Timer::Timer() {
	sgltk::init_lib();
	start();
}

Timer::~Timer() {
}

void Timer::start() {
	start_time = std::chrono::high_resolution_clock::now();
}

double Timer::get_time() {
	std::chrono::duration<double> dt = std::chrono::high_resolution_clock::now() -
						start_time;

	return dt.count();
}
