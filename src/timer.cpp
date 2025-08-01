#include <sgltk/timer.h>

using namespace sgltk;

Timer::Timer() {
	start();
}

Timer::~Timer() {
}

void Timer::start() {
	start_time = std::chrono::high_resolution_clock::now();
}

double Timer::get_time_s() {
	std::chrono::high_resolution_clock::time_point end_time =
		std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> dt = end_time - start_time;

	return dt.count();
}

double Timer::get_time_ms() {
	std::chrono::high_resolution_clock::time_point end_time =
		std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> dt = end_time - start_time;

	return dt.count();
}
