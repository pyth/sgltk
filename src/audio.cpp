#include "audio.h"

using namespace std;

Track::Track() {
	init_lib();
	music = NULL;
}

Track::~Track() {
	Mix_FreeMusic(music);
}

bool Track::load_music(const char *filename) {
	music = Mix_LoadMUS(filename);
	if(music == NULL) {
		cerr << "Mix_LoadMUS: " << Mix_GetError() << endl;
		return false;
	}
	return true;
}

void Track::play(int loops) {
	Mix_PlayMusic(music, loops);
}

void Track::pause() {
	Mix_PausedMusic();
}

void Track::stop() {
	Mix_HaltMusic();
}
