#ifndef PLAYER
#define PLAYER
#include <raylib.h>
#include <tuple>

class player
{
	int points_	       = 0;
	float player_x_    = 0;
	float player_y_    = 0;
	int player_width_  = 25;
	int player_height_ = 150;

	KeyboardKey up_movement_key_;
	KeyboardKey down_movement_key_;
	
public:
	player(float x, float y, KeyboardKey up, KeyboardKey down);
	player(KeyboardKey up, KeyboardKey down);
	void move_player(int height);
	void render_player() const;
	[[nodiscard]] std::tuple<int, int> get_player_size() const;
	void set_player_position(float x, float y);
	[[nodiscard]] int get_player_height() const;
	[[nodiscard]] int get_player_width() const;
	[[nodiscard]] float get_player_x() const;
	[[nodiscard]] float get_player_y() const;
	void add_point();
	[[nodiscard]] int get_points() const;
};

#endif