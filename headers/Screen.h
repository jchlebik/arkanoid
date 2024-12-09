#ifndef SCREEN_H
#define SCREEN_H

#include <stdexcept>
#include <memory>
#include <string_view>
#include <cassert>

#include "SDL.h"

/**
 * RAII for SDL resources needed to render score on screen.
 * 
 * Using unique_ptr to manage the resources. The ptr is instantiated with SDL_Destroy passed to it to invoke when destruction
 * occurs. However, beacause of c++ dtor ordering, we have to manually invoke the destruction before SDL_Quit.
 * 
 * int m_width: width of the screen.
 * int m_height: height of the screen.
 * std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window_ptr: unique_ptr to the window resource.
 * std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_renderer_ptr: unique_ptr to the renderer resource.
 * 
 * Public Methods:
 *  - Screen(): constructor for the Screen class.
 *  - clear(): clear the screen with a background color.
 *  - present(): present the screen to display.
 * 
 *  - width(): get the width of the screen.
 *  - height(): get the height of the screen.
 * 
 *  - left(): get the left edge of the screen.
 *  - right(): get the right edge of the screen.
 *  - top(): get the top edge of the screen.
 *  - bottom(): get the bottom edge of the screen.
 * 
 *  - make_resizable(): set the window as resizable in the x and y directions by an integer factor.
 * 
 *  - get_window_ptr_raw(): get the raw pointer to the window.
 *  - get_renderer_ptr_raw(): get the raw pointer to the renderer.
 * 
 */
class Screen
{
    int m_width;
    int m_height;
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window_ptr {nullptr, SDL_DestroyWindow};
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_renderer_ptr {nullptr, SDL_DestroyRenderer};

public:
    Screen(const Screen&) = delete;             // no copy 
    Screen& operator=(const Screen&) = delete;  // no copy

    /**
     * RAII for SDL resources.
     * 
     * Parameters:
     * - std::string window_name: name of the window.
     * - int width: width of the window.
     * - int height: height of the window.
     * 
     * Throws:
     * std::runtime_error: on any of:
     *      - SDL library could not be initialized, 
     *      - window could not be created, 
     *      - renderer could not be created.
     * 
     */
    Screen(const std::string_view window_name, int width, int height);

    /**
     * Clear the screen with a background color.
     * 
     * Parameters:
     * - SDL_Color clr: base color of the screen background.
     */
    void clear(SDL_Color clr = {0, 0, 0, 255});

    /**
     * Present the screen to the user. To be called after all drawing operations.
     */
    void present();

    /**
     * Get the width of the screen.
     */
    int width() const;

    /**
     * Get the height of the screen.
     */
    int height() const;

    /**
     * Get the left edge of the screen.
     */
    int left() const;

    /**
     * Get the right edge of the screen.
     */
    int right() const;

    /**
     * Get the top edge of the screen.
     */
    int top() const;

    /**
     * Get the bottom edge of the screen.
     */
    int bottom() const;

    /**
     * Set the window as resizable in the x and y directions by an integer factor.
     */
    void make_resizable();

    /**
     * Get the raw pointer to the window. 
     * DANGER: Use with caution as you are getting a raw pointer with this method.
     * Necessary because the SDL library requires raw pointers for its operations.
     * 
     * Returns:
     * - SDL_Window*: raw pointer to the window, still managed by the unique_ptr.
     */
    SDL_Window* get_window_ptr_raw();

    /**
     * Get the raw pointer to the renderer. 
     * DANGER: Use with caution as you are getting a raw pointer with this method.
     * Necessary because the SDL library requires raw pointers for its operations.
     * 
     * Returns:
     * - SDL_Window*: raw pointer to the window, still managed by the unique_ptr.
     */
    SDL_Renderer* get_renderer_ptr_raw();

    /**
     * Destructor for the Screen class. Relies on unique_ptr to handle the destruction of the resources.     * 
     * Calls SDL_Quit to clean up the SDL library.
     */
    ~Screen();
};

#endif // !SCREEN_H