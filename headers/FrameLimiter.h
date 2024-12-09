#ifndef FRAME_LIMITER_H
#define FRAME_LIMITER_H

#include "SDL.h"

/**
 * FrameLimiter
 * 
 * This class is used to limit the frame rate of the game. 
 * Implemented roughly using SDL_Delay function which calls os sleep with poor resolution.
 * 
 * Public Methods:
 *  - void start_frame(): start the frame.
 *  - void limit_to_desired(): limit the frame rate to the desired frame rate.
 * 
 */
class FrameLimiter
{
    int m_desired_fps;
    int m_delta;
    uint64_t m_start;

public:
    /**
     * Constructor for the FrameLimiter class. 
     * 
     * Params:
     * int desired_fps: desired frames per second.
     */
    FrameLimiter(int desired_fps = 60);
    
    /**
     * Start the frame and record the start time.
     */
    void start_frame();

    /**
     * Roughly limit the frame rate to the desired frame rate.
     */
    void limit_to_desired();

};

#endif // !FRAME_LIMITER_H