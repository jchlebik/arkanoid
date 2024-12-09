#include <vector>
#include <iostream>

#include "SDL.h"

#include "Bricks.h"
#include "Screen.h"
#include "Ball.h"
#include "Paddle.h"
#include "FrameLimiter.h"
#include "BricksLayout.h"
#include "Score.h"
#include "GameSettings.h"

#include "ArkanoidGame.h"


ArkanoidGame::ArkanoidGame(
    const GameSettings settings,
    BricksLayout& bricks_layout
):
    m_settings(settings),
    m_screen("Arkanoid", settings.screen_width, settings.screen_height),
    m_ball(settings.ball_size, settings.ball_speed, settings.ball_speed, false),
    m_paddle(settings.screen_width / 2 - settings.paddle_width / 2, settings.screen_height - settings.paddle_offset, settings.paddle_width, settings.paddle_height, settings.paddle_speed),
    m_bricks(bricks_layout),
    m_score("assets/DejaVuSans.ttf", 20, settings.num_of_balls),
    m_frame_limiter(m_settings.fps_limit)
{
    m_screen.make_resizable();
}

void ArkanoidGame::poll_for_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) 
    {
        if (e.type == SDL_QUIT)  // Quit the game
        {
            m_hard_quit = true;
        }
    }
}

void ArkanoidGame::restart()
{
    m_restart = false;
    m_running = true;
    m_hard_quit = false;

    m_ball.reset_to_paddle(m_paddle);
    m_paddle.reset();
    m_score.reset();
    m_bricks.reset();
}


void ArkanoidGame::player_input(bool end_screen)
{
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);      // left or right arrows for movement
    if (!end_screen)
    {
        if (keyState[SDL_SCANCODE_LEFT] && m_paddle.left() > m_screen.left()) 
        {
            m_paddle.move_left(0);
        }
        if (keyState[SDL_SCANCODE_RIGHT] && m_paddle.right() < m_screen.right()) 
        {
            m_paddle.move_right(m_screen.width());
        }
        if (keyState[SDL_SCANCODE_SPACE] && !m_ball.is_moving())        // space to launch the ball if it is not moving
        {
            m_ball.set_moving(true);
        }
        if (keyState[SDL_SCANCODE_Q] || keyState[SDL_SCANCODE_ESCAPE])  // Q or ESC to quit the game
        {
            m_hard_quit = true;
            m_running = false;
        }
    }

    if (end_screen)
    {
        if (keyState[SDL_SCANCODE_R])  // R to restart the game
        {
            m_restart = true;
            m_running = false;
        }
        if (keyState[SDL_SCANCODE_Q] || keyState[SDL_SCANCODE_ESCAPE]) 
        {
            m_running = false;
        }
    }
}

bool ArkanoidGame::game_loop()
{
    restart();
    m_score.prepare(
        m_screen,
        SDL_Color{255, 255, 255, 255}
    );

    while(m_running && !m_hard_quit)
    {
        bool score_changed = false;
        m_frame_limiter.start_frame();
        
        poll_for_events();
        player_input();

        if (m_ball.is_moving())
        {
            score_changed = m_ball.interact(m_screen, m_paddle, m_bricks, m_score);
        }
        else
        {
            m_ball.reset_to_paddle(m_paddle);
        }

        if (m_score.get_balls_remaining() < 0 || m_bricks.get_brick_count() == 0)
        {
            m_running = false;
        }
    
        m_screen.clear(SDL_Color{0, 0, 0, 255});
        m_paddle.draw(m_screen, SDL_Color{255, 255, 255, 255});
        m_ball.draw(m_screen, SDL_Color{0, 255, 0, 255});
        m_bricks.draw(m_screen);

        if (score_changed)
        {
            m_score.prepare(m_screen, SDL_Color{255, 255, 255, 255});
        }
        m_score.draw(m_screen);

        m_screen.present();
        m_frame_limiter.limit_to_desired();
    }
    return m_hard_quit;
}

bool ArkanoidGame::show_end_screen()
{
    m_score.change_font_size(33);

    bool won = m_score.get_balls_remaining() >= 0;
    SDL_Color color;

    char status_string[75] = {0};
    if (won)
    {
        color = SDL_Color{ 0, 255, 0, 255 };
        snprintf(status_string, 75, "    Congratulations!    \n\nQ to quit / R to restart");
    }
    else
    {
        color = SDL_Color{ 255, 0, 0, 255 };
        snprintf(status_string, 75 ,"       Game Over!       \n\nQ to quit / R to restart");
    }
    m_score.prepare(m_screen, status_string, color);
    m_score.draw(
        m_screen, 
        (m_screen.width() - m_score.get_text_width())/2, 
        (m_screen.height() - m_score.get_text_height())/2
    );
    
    m_running = true;
    while(m_running)
    {
        m_frame_limiter.start_frame();
        poll_for_events();
        player_input(true);

        m_screen.clear(SDL_Color{0, 0, 0, 255});
        m_score.draw(
            m_screen, 
            (m_screen.width() - m_score.get_text_width())/2, 
            (m_screen.height() - m_score.get_text_height())/2
        );
        m_screen.present();
        m_frame_limiter.limit_to_desired();
    }
    return m_restart;
}
