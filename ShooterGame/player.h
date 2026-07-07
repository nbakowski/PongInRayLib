#ifndef PLAYER
#define PLAYER
#include <raylib.h>

class player
{
	int points_	= 0;
	float player_x_ = 0;
	float player_y_ = 0;
	float player_width_ = 25;
	float player_height_ = 150;

	KeyboardKey up_movement_key_;
	KeyboardKey down_movement_key_;
	
public:
	player(float x, float y, KeyboardKey up, KeyboardKey down);
	player(KeyboardKey up, KeyboardKey down);
	void move_player(int height);
	void render_player() const;
	void set_player_position(float x, float y);
	[[nodiscard]] float get_player_height() const;
	[[nodiscard]] float get_player_width() const;
	[[nodiscard]] float get_player_x() const;
	[[nodiscard]] float get_player_y() const;
	void add_point();
	[[nodiscard]] int get_points() const;
};

#endif