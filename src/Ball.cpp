#include "SDL.h"

#include "Screen.h"
#include "Paddle.h"
#include "Bricks.h"
#include "Score.h"

#include "Ball.h"

Ball::Ball(
    int ball_size,
    int velocity_x, 
    int velocity_y, 
    bool is_moving
):
    m_rect{.x = 0, .y = 0, .w = ball_size, .h = ball_size},
    m_velocity_x{velocity_x},
    m_velocity_y{velocity_y},
    m_is_moving{is_moving},
    m_original_velocity_x{velocity_x},
    m_original_velocity_y{velocity_y}
{
}

bool Ball::interact(
    const Screen& screen, 
    Paddle& paddle,
    Bricks& bricks, 
    Score& score
)
{
    bool score_changed = false;
    bool paddle_collision = false;

    // Move the ball with given velocity
    move_forward();
    bounce_from_screen(screen);

    // Paddle collision
    if (SDL_HasIntersection(&m_rect, paddle.get()))
    {
        bounce_from_paddle(paddle);
        paddle_collision = true;
    }

    if (!paddle_collision)   // if ball collided with the paddle, no need to check for brick collisions
    {
        // Brick collisions
        for (auto& brick : bricks)
        {
            if (brick.is_visible() && SDL_HasIntersection(&m_rect, brick.get()))
            {
                bricks.decrement_counter();
                brick.set_visible(false);
                bounce_from_brick(brick);
                score.add_points(brick.get_points());
                score_changed = true;
                break;
            }
        }

        // Reset if the ball falls below the paddle
        if (m_rect.y > screen.height()) 
        {
            reset_to_paddle(paddle);   // Ball needs to be reset
            score_changed = true;
            score.decrement_counter();
        }
    }
    return score_changed;
}

void Ball::set_moving(const bool moving)
{
    m_is_moving = moving;
}

bool Ball::is_moving() const
{
    return m_is_moving;
}

void Ball::set_velocity_x(const int velocity)
{
    m_velocity_x = velocity;
}

void Ball::set_velocity_y(const int velocity)
{
    m_velocity_y = velocity;
}

void Ball::reset_to_paddle(const Paddle& paddle)
{
    m_rect.x = paddle.left() + paddle.width()/2 - m_rect.w/2;
    m_rect.y = paddle.top() - 1 - m_rect.h;
    m_velocity_x = m_original_velocity_x;
    m_velocity_y = m_original_velocity_y;
    m_is_moving = false;
}

void Ball::draw(Screen& screen, SDL_Color color)
{
    SDL_SetRenderDrawColor(screen.get_renderer_ptr_raw(), color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(screen.get_renderer_ptr_raw(), &m_rect);
}

void Ball::move_forward()
{
    m_rect.x += m_velocity_x;
    m_rect.y += m_velocity_y;
}

void Ball::bounce_from_screen(const Screen& screen)
{
    if (m_rect.x <= screen.left()) // left wall
    {
        m_rect.x = screen.left();
        bounce_x();
    }
    if (m_rect.x + m_rect.w >= screen.right()) // right wall
    {
        m_rect.x = screen.right() - m_rect.w;
        bounce_x();
    }
    if (m_rect.y <= screen.top()) // top wall
    {
        m_rect.y = screen.top();
        bounce_y();
    }
}

void Ball::bounce_from_paddle(const Paddle& paddle)
{
    m_rect.y = paddle.top() - 1 - m_rect.w;
    bounce_y();
}

void Ball::bounce_from_brick(const Brick& brick)
{
    int overlap_left = m_rect.x + m_rect.w - brick.left();
    int overlap_right = brick.right() - m_rect.x;
    int overlap_top = m_rect.y + m_rect.w - brick.top();
    int overlap_bottom = brick.bottom() - m_rect.y;

    if (overlap_left < overlap_right && overlap_left < overlap_top && overlap_left < overlap_bottom) // left
    {
        bounce_x();
    }
    else if (overlap_right < overlap_left && overlap_right < overlap_top && overlap_right < overlap_bottom) // right
    {
        bounce_x();
    }
    else if (overlap_top < overlap_left && overlap_top < overlap_right && overlap_top < overlap_bottom) // top
    {
        bounce_y();
    }
    else // bottom as a default
    {
        bounce_y();
    }
}

void Ball::bounce_x()
{
    m_velocity_x = -m_velocity_x;
}

void Ball::bounce_y()
{
    m_velocity_y = -m_velocity_y;
}
