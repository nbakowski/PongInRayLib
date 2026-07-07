#include "audio_player.h"

audio_player::~audio_player()
{
	UnloadSound(collision_sound_);
	UnloadSound(game_start_);
	UnloadSound(lost_point_);
}

void audio_player::play_bong() const
{
	PlaySound(collision_sound_);
}

void audio_player::play_start() const
{
	PlaySound(game_start_);
}

void audio_player::play_lost() const
{
	PlaySound(lost_point_);
}
