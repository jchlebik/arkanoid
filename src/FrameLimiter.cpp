#include "SDL.h"

#include "FrameLimiter.h"

FrameLimiter::FrameLimiter(int desired_fps)
{
    m_desired_fps = desired_fps;
    m_delta = 1000 / m_desired_fps;
}

void FrameLimiter::start_frame()
{
    m_start = SDL_GetTicks64();
}

void FrameLimiter::limit_to_desired()
{
    float elapsed_ms = (SDL_GetTicks64() - m_start) ;
    if (elapsed_ms < m_delta)
    {
        SDL_Delay(m_delta - elapsed_ms);
    }
}
