#include "player.h"
#include "raylib.h"

inline constexpr int movement_speed = 500;

player::player(const float x, const float y, const KeyboardKey up, const KeyboardKey down) 
	: player_x_(x), player_y_(y), up_movement_key_(up), down_movement_key_(down) {};

player::player( const KeyboardKey up, const KeyboardKey down)
	: up_movement_key_(up), down_movement_key_(down) {};

void player::move_player(const int height)
{
	const float speed = movement_speed * GetFrameTime();
	
	if (IsKeyDown(up_movement_key_) && player_y_ >= 0)
	{
		player_y_ -= speed;
	}
	else if (IsKeyDown(down_movement_key_) && player_y_ + static_cast<float>(player_height_) <= static_cast<float>(height))
	{
		player_y_ += speed;
	}
}

void player::render_player() const
{
	DrawRectangle(static_cast<int>(player_x_), static_cast<int>(player_y_), player_width_, player_height_, BLACK);
}

std::tuple<int, int> player::get_player_size() const
{
	return { player_width_, player_height_ };
}

void player::set_player_position(const float x, const float y)
{
	player_x_ = x;
	player_y_ = y;
}

int player::get_player_height() const
{
	return player_height_;
}

int player::get_player_width() const
{
	return player_width_;
}

float player::get_player_x() const
{
	return player_x_;
}

float player::get_player_y() const
{
	return player_y_;
}

void player::add_point()
{
	points_++;
}

int player::get_points() const
{
	return points_;
}
