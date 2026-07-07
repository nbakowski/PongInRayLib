#ifndef AUDIO_PLAYER
#define AUDIO_PLAYER
#include <raylib.h>

class audio_player
{
	Sound collision_sound_ = LoadSound("sounds/bong.wav");
	Sound game_start_ = LoadSound("sounds/game_start.mp3");
	Sound lost_point_ = LoadSound("sounds/lost_life.mp3");
public:
	~audio_player();
	void play_bong() const;
	void play_start() const;
	void play_lost() const;
};

#endif