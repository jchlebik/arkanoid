#include <iostream>
#include <vector>

#include "SDL.h"    // expecting SDL to be installed on the sysmte, -I/path/.../SDL2 during compilation, CMAKE should take care of this

#include "ArkanoidGame.h"
#include "RowLayout.h"


int main()
{
    GameSettings settings = GameSettings{
        .screen_width = 800,
        .screen_height = 600,
        .paddle_width = 100,
        .paddle_height = 10,
        .paddle_speed = 6,
        .paddle_offset = 80,
        .ball_size = 10,
        .ball_speed = 4,
        .num_of_balls = 3,
        .fps_limit = 60
    };
    

    RowLayout layout = RowLayout(
        RowLayoutSettings{
            .starting_row = 2,
            .brick_rows = 4,
            .brick_cols = 10,
            .brick_spacing = 10,
            .brick_width = 800 / 10,
            .brick_height = 30
        }
    );

    try
    {
        ArkanoidGame arkanoid(
            settings,
            layout
        );
        bool restart = false;
        do
        {
            bool hard_quit = arkanoid.game_loop();
            if (!hard_quit)
            {
                restart = arkanoid.show_end_screen();
            }
        } while (restart);
    }
    catch(std::runtime_error e)
    {
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION , "Exiting program because of an exception. See cerr for more info.\n");
    }

    return 0;

}