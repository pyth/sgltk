#include "timer.h"

Timer::Timer() {
	sgltk::init_lib();
	start_time = 0;
}

Timer::~Timer() {
}

void Timer::start() {
	start_time = SDL_GetTicks();
}

unsigned int Timer::get_time() {
	return SDL_GetTicks() - start_time;
}
