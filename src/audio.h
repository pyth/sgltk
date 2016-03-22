#ifndef __AUDIO_H__
#define __AUDIO_H__

#ifdef HAVE_SDL_MIXER_H
#include "core.h"

class Track {
	Mix_Music *music;
	Mix_Chunk *mus;
public:
	Track();
	~Track();

	bool load_music(const char *filename);
	void play(int loops);
	void pause();
	void stop();
};
#endif //HAVE_SDL_MIXER_H

#endif
