#ifndef BALL
#define BALL
#include <cstdint>
#include <optional>
#include <raylib.h>

enum class direction : std::uint8_t
{
	LEFT, RIGHT
};

class ball
{

	float ball_x_ = 0;
	float ball_y_ = 0;
	float ball_radius_ = 0;
	Color ball_color_;
	direction direction_ = direction::RIGHT;
	float ball_speed_ = 750;
	float vertical_movement_speed_ = 325;
	void reset_ball_position(float w, float h);
	void reverse_direction();
public:
	ball(float x, float y, float r, Color color);
	void render_ball() const;
	bool check_collision_and_bounce(float s_x, float s_y, float e_x, float e_y);
	void move_ball();
	std::optional<direction> check_side_wall_collision(float l_wall_x, float r_wall_x, float w_height);
	void check_top_and_bottom_collision(float s_y, float e_y);
};

#endif