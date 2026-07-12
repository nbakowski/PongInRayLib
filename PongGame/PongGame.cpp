#include "ball.h"
#include "raylib.h"
#include "player.h"
#include "audio_player.h"

#include <array>
#include <string>

inline constexpr int default_window_width = 1280;
inline constexpr int default_window_height = 1024;
inline constexpr int min_window_width = 640;
inline constexpr int min_window_height = 480;
inline constexpr int score_font_size = 96;
inline constexpr int pause_text_size = 48;
inline constexpr int text_padding = 12;
inline constexpr int line_thickness = 6;

int screen_width = default_window_width;
int screen_height = default_window_height;

namespace
{
    void reset_players_position(player& p1, player& p2)
    {
        p1.set_player_position(
            0,
            (screen_height - p1.get_player_height()) / 2.0f
        );

        p2.set_player_position(
            screen_width - p2.get_player_width(),
            (screen_height - p2.get_player_height()) / 2.0f
        );
    }

    void restart_the_game(auto& players, ball& b)
    {
        for (player* p : players)
        {
            p->reset_points();
        }
        b.reset_ball_position(screen_width, screen_height);
    }

    void render_dashed_line()
    {
        constexpr int rec_height = 20;
        for (auto i = 0; i < screen_height; i += rec_height * 2)
        {
            DrawRectangle((screen_width - line_thickness) / 2, i, line_thickness, rec_height, BLACK);
        }
    }

    void draw_the_score(const int p_one_points, const int p_two_points)
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
            screen_width - MeasureText(std::to_string(p_two_points).c_str(),
                score_font_size) - text_padding,
            text_padding,
            score_font_size,
            RED
        );
    }
}

int main()
{
    bool is_game_paused = false;
    bool is_shader_loaded = false;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "SigmaPong");
    SetWindowMinSize(min_window_width, min_window_height);
    InitAudioDevice();
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_ERROR);

    player player_one(KEY_W, KEY_S);
    player player_two(KEY_UP, KEY_DOWN);
    
    ball ball(screen_width / 2.0f, screen_height / 2.0f, 10.0f, RED);

    reset_players_position(player_one, player_two);

    const std::array players = {&player_one, &player_two};

    constexpr char pause_t[7] = "PAUSED";

    const audio_player audio;
    audio.play_start();

    const Shader crt = LoadShader(nullptr, "shaders/crt.fs");
    
    if (crt.id == 0 || crt.locs == nullptr)
    {
        TraceLog(LOG_ERROR, "Failed to load the shader.");
    }
    else
    {
        is_shader_loaded = true;
    }
    
    RenderTexture2D canvas = LoadRenderTexture(screen_width, screen_height);

    while (!WindowShouldClose())
    {
        if (IsWindowResized())
        {
            UnloadRenderTexture(canvas);
            screen_width = GetScreenWidth();
            screen_height = GetScreenHeight();
            canvas = LoadRenderTexture(screen_width, screen_height);
            reset_players_position(player_one, player_two);
            ball.reset_ball_position(screen_width, screen_height);
        }

        BeginDrawing();

        if (IsKeyPressed(KEY_SPACE))
        {
            is_game_paused = !is_game_paused;
        }
        
        if (IsKeyPressed(KEY_R))
        {
            restart_the_game(players, ball);
        }
        
        if (IsKeyPressed(KEY_F11))
        {
            ToggleBorderlessWindowed();
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
            render_dashed_line();
	        player_one.render_player();
	        player_two.render_player();
	        ball.render_ball();
            draw_the_score(player_one.get_points(), player_two.get_points());
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

        if (is_shader_loaded) BeginShaderMode(crt);
            const Rectangle rec =
            {
                .x = 0,
                .y = 0,
                .width = static_cast<float>(canvas.texture.width),
                .height = static_cast<float>(-canvas.texture.height)
            };

            const Vector2 vec =
            {
                .x = (screen_width - canvas.texture.width) / 2.0f,
                .y = (screen_height - canvas.texture.height) / 2.0f
            };

            DrawTextureRec(canvas.texture, rec, vec, WHITE);
        if (is_shader_loaded) EndShaderMode();

        EndDrawing();
    }

    CloseAudioDevice();
    UnloadRenderTexture(canvas);
    UnloadShader(crt);
    CloseWindow();

    return 0;
}
