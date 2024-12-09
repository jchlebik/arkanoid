#include <stdexcept>
#include <memory>
#include <string_view>
#include <optional>

#include "SDL.h"
#include "SDL_ttf.h"

#include "Screen.h"
#include "Score.h"

Score::Score(std::string_view font_path, int font_size, int num_balls):
    m_points{0},
    m_font_size{font_size},
    m_num_balls{num_balls},
    m_balls_remaining{num_balls}
{
    if (TTF_Init() < 0) 
    {
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION,"TTF could not initialize! TTF_Error: %s", TTF_GetError());
        throw std::runtime_error("SDL Library could not be initialized!\n");;
    }

    m_font_ptr.reset(TTF_OpenFont(font_path.data(), font_size));
    if (!m_font_ptr) 
    {
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION,"Failed to load font: %s\n", TTF_GetError());
    }
}

Score::~Score()
{
    // Same as with screen, need to manually invoke the dtors before TTF_Quit. Order matters.
    if (m_font_ptr) m_font_ptr.reset(nullptr);
    if (m_text_ptr) m_text_ptr.reset(nullptr);
    if (m_text_texture_ptr) m_text_texture_ptr.reset(nullptr);
    TTF_Quit();
}

int Score::get_text_width() const
{
    return m_text_ptr->w;
}

int Score::get_text_height() const
{
    return m_text_ptr->h;
}

void Score::draw(Screen& screen, std::optional<int> x, std::optional<int> y)
{   
    SDL_Rect dest = {x.value_or(0), y.value_or(screen.height() - get_text_height() - 2), get_text_width(), get_text_height()};
    SDL_RenderCopy(
        screen.get_renderer_ptr_raw(), 
        m_text_texture_ptr.get(), 
        nullptr, 
        &dest
    );
}

void Score::prepare(Screen& screen, const SDL_Color& color)
{
    char status_string[50] = {0};
    snprintf(status_string, 50, m_score_format_string, get_points(), get_balls_remaining());
    m_text_ptr.reset(TTF_RenderText_Solid_Wrapped(m_font_ptr.get(), status_string, color, 0));
    m_text_texture_ptr.reset(
        SDL_CreateTextureFromSurface(
            screen.get_renderer_ptr_raw(), 
            m_text_ptr.get()
        )
    );
}

void Score::prepare(Screen& screen, const std::string_view status_string, const SDL_Color& color)
{
    m_text_ptr.reset(TTF_RenderText_Solid_Wrapped(m_font_ptr.get(), status_string.data(), color, 0));
    m_text_texture_ptr.reset(
        SDL_CreateTextureFromSurface(
            screen.get_renderer_ptr_raw(), 
            m_text_ptr.get()
        )
    );
}

void Score::change_font_size(int new_font_size)
{
    if (m_font_ptr)
    {
        TTF_SetFontSize(m_font_ptr.get(), new_font_size);
    }
}

void Score::decrement_counter()
{
    m_balls_remaining--;
}

void Score::add_points(int points)
{
    m_points += points;
}

int Score::get_points() const
{
    return m_points;
}

int Score::get_balls_remaining() const
{
    return m_balls_remaining;
}

void Score::reset()
{
    m_points = 0;
    m_balls_remaining = m_num_balls;
    change_font_size(m_font_size);
}