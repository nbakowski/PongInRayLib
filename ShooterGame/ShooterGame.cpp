#include "ball.h"
#include "raylib.h"
#include "player.h"

#include <array>
#include <format>

inline constexpr int score_font_size = 32;
inline constexpr int pause_text_size = 48;
inline constexpr int text_padding = 12;
inline constexpr int line_thickness = 6;

int main()
{
    constexpr int screen_width = 1280;
    constexpr int screen_height = 1024;
    bool is_game_paused = false;

    InitWindow(screen_width, screen_height, "SigmaPong");
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_ERROR);


    player player_one(KEY_W, KEY_S);
    player player_two(KEY_UP, KEY_DOWN);
    ball ball(screen_width / 2.0f, screen_height / 2.0f, 10.0f, RED);

    player_one.set_player_position(
        0, 
        screen_height - player_one.get_player_height() / 2.0f
    );

    player_two.set_player_position(
        screen_width - player_two.get_player_width(), 
        screen_height - player_one.get_player_height() / 2.0f
    );

    const std::array players = {&player_one, &player_two};

    std::string player_one_text;
    std::string player_two_text;

    constexpr char pause_t[7] = "PAUSED";

    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(WHITE);

        DrawLine(
            screen_width / 2,
            0,
            screen_width / 2,
            screen_height,
            BLACK
        );

        if (IsKeyPressed(KEY_SPACE))
        {
            is_game_paused = !is_game_paused;
        }

        if (!is_game_paused)
        {

            player_one.move_player(screen_height);
            player_two.move_player(screen_height);

            ball.move_ball();

            for (const player* p : players)
            {
                ball.check_collision_and_bounce(
                    p->get_player_x(),
                    p->get_player_y(),
                    p->get_player_x() + p->get_player_width(),
                    p->get_player_y() + p->get_player_height()
                );
            }

            if (const auto c = ball.check_side_wall_collision(0, screen_width, screen_height); c)
            {
                switch (const auto v = c.value(); v)
                {
                case direction::RIGHT:
                    player_one.add_point();
                    break;
                case direction::LEFT:
                    player_two.add_point();
                    break;
                }
            }

            ball.check_top_and_bottom_collision(0, screen_height);
        }

        player_one.render_player();
        player_two.render_player();
        ball.render_ball();

        player_one_text = std::format("Player One Points: {}", player_one.get_points());
        player_two_text = std::format("Player Two Points: {}", player_two.get_points());

        DrawText(player_one_text.c_str(), text_padding, text_padding, score_font_size, BLACK);
        DrawText(player_two_text.c_str(), screen_width - MeasureText(player_two_text.c_str(), score_font_size) - text_padding, text_padding, score_font_size, BLACK);

        if (is_game_paused)
        {
            DrawText(
                pause_t,
                (screen_width - MeasureText(pause_t, pause_text_size)) / 2,
                screen_height / 4, 
                pause_text_size, 
                RED
            );
        }

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
