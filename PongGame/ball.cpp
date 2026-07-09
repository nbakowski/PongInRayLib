#include "ball.h"
#include <random>

ball::ball(const float x, const float y, const float r, const Color color)
	: ball_x_(x), ball_y_(y), ball_radius_(r), ball_color_(color) {}

void ball::render_ball() const
{
	DrawCircle(static_cast<int>(ball_x_), static_cast<int>(ball_y_), ball_radius_, ball_color_);
}

bool ball::check_collision_and_bounce(const float s_x, const float s_y, const float e_x, const float e_y)
{
	const Rectangle rec = 
	{
		.x = s_x, 
		.y = s_y, 
		.width = e_x - s_x, 
		.height = e_y - s_y
	};
	const Vector2 c_center = 
	{
		.x = ball_x_,
		.y = ball_y_
	};
	
	if (CheckCollisionCircleRec(c_center, ball_radius_, rec))
	{
		reverse_direction();

		if (ball_y_ >= s_y + (e_y - s_y) / 2)
		{
			vertical_movement_speed_ = -vertical_movement_speed_;
		}

		return true;
	}
	return false;
}

void ball::move_ball()
{
	const float speed = ball_speed_ * GetFrameTime();

	switch (direction_)
	{
	case direction::RIGHT:
		ball_x_ -= speed;
		break;
	case direction::LEFT:
		ball_x_ += speed;
		break;
	}
	ball_y_ += vertical_movement_speed_ * GetFrameTime();
}

std::optional<direction> ball::check_side_wall_collision(const float l_wall_x, const float r_wall_x, const float w_height)
{
	if (ball_x_ < l_wall_x)
	{
		reset_ball_position(r_wall_x, w_height);
		return direction::LEFT;
	}
	if (ball_x_ > r_wall_x)
	{
		reset_ball_position(r_wall_x, w_height);
		return direction::RIGHT;
	}
	return std::nullopt;
}

void ball::check_top_and_bottom_collision(const float s_y, const float e_y)
{
	if (ball_y_ < s_y || ball_y_ > e_y)
	{
		vertical_movement_speed_ = -vertical_movement_speed_;
	}
}

void ball::reset_ball_position(const float w, const float h)
{
	ball_x_ = w / 2 - ball_radius_;
	ball_y_ = h / 2;

	std::random_device r;

	if (std::uniform_int_distribution u(0, 100); u(r) >= 50)
	{
		reverse_direction();
	}
	
}

void ball::reverse_direction()
{
	if (direction_ == direction::RIGHT)
	{
		direction_ = direction::LEFT;
	}
	else
	{
		direction_ = direction::RIGHT;
	}
}
