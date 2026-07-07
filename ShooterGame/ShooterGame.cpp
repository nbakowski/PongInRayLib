#include "ball.h"
#include "raylib.h"
#include "player.h"
#include "audio_player.h"

#include <array>
#include <format>
#include <string>

inline constexpr int score_font_size = 96;
inline constexpr int pause_text_size = 48;
inline constexpr int text_padding = 12;
inline constexpr int line_thickness = 6;

namespace
{
    void render_dashed_line(const int s_w, const int s_h)
    {
        constexpr int rec_height = 20;
        for (auto i = 0; i < s_h; i += rec_height * 2)
        {
            DrawRectangle((s_w - line_thickness) / 2, i, line_thickness, rec_height, BLACK);
        }
    }

    void draw_the_score(const int p_one_points, const int p_two_points, const int width)
    {
        DrawText(
            std::to_string(p_one_points).c_str(),
            text_padding,
            text_padding,
            score_font_size,
            RED
        );
        DrawText(
            std::to_string(p_two_points).c_str(),
            width - MeasureText(std::to_string(p_two_points).c_str(),
                score_font_size) - text_padding,
            text_padding,
            score_font_size,
            RED
        );
    }
}

int main()
{
    constexpr int screen_width = 1280;
    constexpr int screen_height = 1024;
    bool is_game_paused = false;

    InitWindow(screen_width, screen_height, "SigmaPong");
    InitAudioDevice();
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_ERROR);


    player player_one(KEY_W, KEY_S);
    player player_two(KEY_UP, KEY_DOWN);
    ball ball(screen_width / 2.0f, screen_height / 2.0f, 10.0f, RED);

    player_one.set_player_position(
        0, 
        (screen_height - player_one.get_player_height()) / 2.0f
    );

    player_two.set_player_position(
        screen_width - player_two.get_player_width(), 
        (screen_height - player_one.get_player_height()) / 2.0f
    );

    const std::array players = {&player_one, &player_two};

    constexpr char pause_t[7] = "PAUSED";

    const audio_player audio;
    audio.play_start();

    const Shader crt = LoadShader(nullptr, "shaders/crt.fs");

    const RenderTexture2D canvas = LoadRenderTexture(screen_width, screen_height);

    while (!WindowShouldClose())
    {

        BeginDrawing();

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
                if (ball.check_collision_and_bounce(
                    p->get_player_x(),
                    p->get_player_y(),
                    p->get_player_x() + p->get_player_width(),
                    p->get_player_y() + p->get_player_height()
                ))
                {
                    audio.play_bong();
                }
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
                audio.play_lost();
            }

            ball.check_top_and_bottom_collision(0, screen_height);
        }

        BeginTextureMode(canvas);

    		ClearBackground(RAYWHITE);
            render_dashed_line(screen_width, screen_height);
	        player_one.render_player();
	        player_two.render_player();
	        ball.render_ball();
            draw_the_score(player_one.get_points(), player_two.get_points(), screen_width);
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

        EndTextureMode();

        BeginShaderMode(crt);
			const Rectangle rec =
			{
				.x = 0, 
				.y = 0, 
				.width = static_cast<float>(canvas.texture.width), 
				.height = static_cast<float>(-canvas.texture.height)
			};

            constexpr Vector2 vec =
            {
                .x = 0,
                .y = 0
			};
			DrawTextureRec(canvas.texture, rec, vec, WHITE);
        EndShaderMode();

        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    UnloadShader(crt);
    CloseWindow();

    return 0;
}