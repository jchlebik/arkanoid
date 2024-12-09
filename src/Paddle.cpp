#include "SDL.h"

#include "Screen.h"

#include "Paddle.h"

Paddle::Paddle(int x, int y, int paddle_width, int paddle_height, int paddle_speed):
    m_rect{.x = x, .y = y, .w = paddle_width, .h = paddle_height}, 
    m_paddle_speed{paddle_speed},
    m_original_rect{.x = x, .y = y, .w = paddle_width, .h = paddle_height}
{
}

SDL_Rect* Paddle::get()
{
    return &m_rect;
}

void Paddle::draw(Screen& screen, SDL_Color color)
{
    SDL_SetRenderDrawColor(screen.get_renderer_ptr_raw(), color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(screen.get_renderer_ptr_raw(), &m_rect);
}

void Paddle::move_left(const int& edge)
{
    if (m_rect.x > edge)
    {
        m_rect.x -= m_paddle_speed;
    }
}

void Paddle::move_right(const int& edge)
{
    if (m_rect.x + m_rect.w < edge)
    {
        m_rect.x += m_paddle_speed;
    }
}

void Paddle::reset()
{
    m_rect = m_original_rect;
}

int Paddle::left() const
{
    return m_rect.x;
}

int Paddle::right() const
{
    return m_rect.x + m_rect.w;
}

int Paddle::top() const
{
    return m_rect.y;
}

int Paddle::bottom() const
{
    return m_rect.y + m_rect.h;
}

int Paddle::width() const
{
    return m_rect.w;
}

int Paddle::height() const
{
    return m_rect.h;
}
