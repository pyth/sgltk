#ifndef __AUDIO_H__
#define __AUDIO_H__

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

#endif
