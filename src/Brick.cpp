#include <stdexcept>
#include "SDL.h"
#include "Screen.h"

#include "Brick.h"

/**
 * Each brick on the field is represented by this class. If a brick is hit, the visible flag is set to false.
 */
Brick::Brick(
    const int x, 
    const int y, 
    const int brick_width, 
    const int brick_height, 
    const int points, 
    const SDL_Color&& color
):
    m_rect{.x = x, .y = y, .w = brick_width, .h = brick_height},
    m_visible{true},
    m_points{points},
    m_color{color}
{
}

void Brick::set_visible(const bool visibility)
{
    m_visible = visibility;
}

bool Brick::is_visible() const
{
    return m_visible;
}

void Brick::set_points(const int points)
{
    m_points = points;
}

int Brick::get_points() const
{
    return m_points;
}

void Brick::set_color(const SDL_Color&& color)
{
    m_color = color;
}

SDL_Color Brick::get_color() const
{
    return m_color;
}

void Brick::draw(Screen& screen)
{
    if (SDL_SetRenderDrawColor(screen.get_renderer_ptr_raw(), m_color.r, m_color.g, m_color.b, m_color.a) != 0)
    {
        SDL_Log("SDL_SetRenderDrawColor failed %s \n", SDL_GetError());
        throw std::runtime_error("SDL_SetRenderDrawColor failed");
    }
    if (SDL_RenderFillRect(screen.get_renderer_ptr_raw(), &m_rect) != 0)
    {
        SDL_Log("SDL_RenderFillRect failed %s \n", SDL_GetError());
        throw std::runtime_error("SDL_RenderFillRect failed");
    }
}

SDL_Rect* Brick::get()
{
    return &m_rect;
}

int Brick::left() const
{
    return m_rect.x;
}

int Brick::right() const
{
    return m_rect.x + m_rect.w;
}

int Brick::top() const
{
    return m_rect.y;
}

int Brick::bottom() const
{
    return m_rect.y + m_rect.h;
}