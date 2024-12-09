#include <stdexcept>
#include <memory>
#include <string>

#include "SDL.h"

#include "Screen.h"

Screen::Screen(const std::string_view window_name, int width, int height):
    m_width{width},
    m_height{height}
{
    SDL_Log("SDL Initialization...\n");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL_Error: %s \n", SDL_GetError());
        throw std::runtime_error("SDL Library could not be initialized!\n");
    }
    SDL_Log("SDL Initialization OK\n");

    SDL_Log("Creating SDL Window ...\n");
    m_window_ptr.reset(SDL_CreateWindow(
        window_name.data(), 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        m_width, height, 
        SDL_WINDOW_SHOWN
    ));
    if (!m_window_ptr) {
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s \n", SDL_GetError());
        throw std::runtime_error("Window could not be created!\n");
    }
    SDL_Log("SDL Window OK\n");

    SDL_Log("Creating SDL Renderer ...\n");
    m_renderer_ptr.reset(
        SDL_CreateRenderer(m_window_ptr.get(), -1, SDL_RENDERER_ACCELERATED)
    );
    if (!m_renderer_ptr) {
        SDL_LogError(SDL_LogCategory::SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL_Error: %s \n", SDL_GetError());
        throw std::runtime_error("Renderer could not be created!\n");
    }
    SDL_Log("SDL Renderer OK\n");

}

void Screen::clear(SDL_Color clr)
{
    // Redraw the screen
    SDL_SetRenderDrawColor(m_renderer_ptr.get(), clr.r, clr.g, clr.b, clr.a); // Black
    SDL_RenderClear(m_renderer_ptr.get());
}

void Screen::present()
{
    SDL_RenderPresent(m_renderer_ptr.get());
}

int Screen::width() const
{
    return m_width;
}

int Screen::height() const
{
    return m_height;
}

int Screen::left() const
{
    return 0;
}

int Screen::right() const
{
    return m_width;
}

int Screen::top() const
{
    return 0;
}

int Screen::bottom() const
{
    return m_height;
}

void Screen::make_resizable()
{
    SDL_SetWindowResizable(m_window_ptr.get(), SDL_bool::SDL_TRUE);
    SDL_RenderSetLogicalSize(m_renderer_ptr.get(), m_width, m_height);
    SDL_RenderSetIntegerScale(m_renderer_ptr.get(), SDL_bool::SDL_TRUE);
}

SDL_Window* Screen::get_window_ptr_raw()
{
    return m_window_ptr.get();
}

SDL_Renderer* Screen::get_renderer_ptr_raw()
{
    return m_renderer_ptr.get();
}

Screen::~Screen()
{
    // Have to mannualy reset the unique_ptr to release the resources before SDL_Quit. Order matters.
    if (m_renderer_ptr) m_renderer_ptr.reset(nullptr);
    if (m_window_ptr) m_window_ptr.reset(nullptr);
    SDL_Quit();
}