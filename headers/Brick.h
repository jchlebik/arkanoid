#ifndef BRICK_H
#define BRICK_H

#include <stdexcept>
#include "SDL.h"
#include "Screen.h"

/**
 * Each brick on the field is represented by this class. If a brick is hit, the visible flag is set to false.
 * Held in a vector in the Bricks class. See Bricks.h for more information.
 * 
 * SDL_Rect m_rect: x, y, width, height. x, y are the top left corner.
 * bool m_visible: this brick was hit by the ball yet or not.
 * int m_points: number of points player earns by hiting this brick.
 * SDL_Color m_color: color of the brick.
 * 
 * Public Methods:
 *  - void set_visible(): set the visibility of the brick.
 *  - bool is_visible(): check if the brick is visible.
 * 
 *  - void set_points(): set the points reward for hitting this brick.
 *  - int get_points(): get the points reward for hitting this brick.
 * 
 *  - void set_color(): set the color of the brick.
 *  - SDL_Color get_color(): get the color of the brick.
 * 
 *  - void draw(): draw the brick on the screen.
 *  - SDL_Rect* get(): get the SDL_Rect of the brick. Needed for SDL library functions. Used for collision detection.
 * 
 *  - int left(): get the left edge of the brick.
 *  - int right(): get the right edge of the brick.
 *  - int top(): get the top edge of the brick.
 *  - int bottom(): get the bottom edge of the brick.
 * 
 */
class Brick
{
    SDL_Rect m_rect;          // x, y, width, height. x, y are the top left corner.
    bool m_visible;           // this brick was hit by the ball yet or not.
    int m_points;             // number of points player earns by hiting this brick.
    SDL_Color m_color;        // color of the brick.

public:

    /**
     * Constructor for the Brick class.
     * 
     * Params:
     * const int x: x position of the left top corner of the brick.
     * const int y: y position of the left top corner the brick.
     * const int brick_width: width of the brick.
     * const int brick_height: height of the brick.
     * const int points: points player earns by hiting this brick.
     * const SDL_Color&& color: color of the brick.
     * 
     */
    Brick(
        const int x, 
        const int y, 
        const int brick_width, 
        const int brick_height, 
        const int points, 
        const SDL_Color&& color
    );

    /**
     * Set the visibility of the brick.
     * 
     * Params:
     * const bool visibility: visibility of the brick.
     */
    void set_visible(const bool visibility);

    /**
     * Check if the brick is visible.
     */
    bool is_visible() const;

    /**
     * Set the points reward for hitting this brick.
     * 
     * Params:
     * const int points: points for the brick.
     */
    void set_points(const int points);

    /**
     * Get the points reward for hitting this brick.
     */
    int get_points() const;

    /**
     * Set the color of the brick.
     * 
     * Params:
     * const SDL_Color&& color: color of the brick.
     */
    void set_color(const SDL_Color&& color);

    /**
     * Get the color of the brick.
     */
    SDL_Color get_color() const;

    /**
     * Draw the brick on the screen.
     * 
     * Params:
     * Screen& screen: screen to draw the brick on.
     * 
     * Throws:
     * std::runtime_error: if SDL_SetRenderDrawColor or SDL_RenderFillRect fails.
     */
    void draw(Screen& screen);

    /**
     * Get the SDL_Rect of the brick. Needed for SDL library functions. Used for collision detection.
     * 
     * Returns:
     * SDL_Rect*: pointer to the SDL_Rect of the brick.
     */
    SDL_Rect* get();

    /**
     * Get the left edge of the brick.
     */
    int left() const;

    /**
     * Get the right edge of the brick.
     */
    int right() const;

    /**
     * Get the top edge of the brick.
     */
    int top() const;

    /**
     * Get the bottom edge of the brick.
     */
    int bottom() const;

};

#endif // !BRICK_H