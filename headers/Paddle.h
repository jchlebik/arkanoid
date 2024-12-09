#ifndef PADDLE_H
#define PADDLE_H

#include "SDL.h"

#include "Screen.h"

/**
 * Paddle class represents the paddle in the game. It can move left and right. A wrapper around SDL_Rect.
 * 
 * SDL_Rect m_rect: rectangle representing the paddle.
 * int m_paddle_speed: speed of the paddle.
 * 
 * const SDL_Rect m_original_rect: original rectangle of the paddle when the instance was constructed.
 * 
 * Public Methods:
 * - SDL_Rect* get(): get the SDL_Rect of the paddle.
 * - void draw(): draw the paddle on the screen.
 * 
 * - void move_left(): move the paddle to the left.
 * - void move_right(): move the paddle to the right.
 * 
 * - void reset(): reset the paddle to the original position.
 * 
 * - int left(): get the left edge of the paddle.
 * - int right(): get the right edge of the paddle.
 * - int top(): get the top edge of the paddle.
 * - int bottom(): get the bottom edge of the paddle.
 * 
 * - int width(): get the width of the paddle.
 * - int height(): get the height of the paddle.
 * 
 */
class Paddle
{
    SDL_Rect m_rect;
    int m_paddle_speed;

    const SDL_Rect m_original_rect;

public:
    /**
     * Constructor for the Paddle class.
     * 
     * Params:
     * int x: x position of the left top corner of the paddle.
     * int y: y position of the left top corner of the paddle.
     * int paddle_width: width of the paddle.
     * int paddle_height: height of the paddle.
     * int paddle_speed: speed of the paddle.
     * 
    */
    Paddle(int x, int y, int paddle_width, int paddle_height, int paddle_speed);

    /**
     * Get the SDL_Rect of the paddle. Needed for SDL library functions. Used for collision detection.
     */
    SDL_Rect* get();

    /**
     * Draw the paddle on the screen.
     * 
     * Params:
     * Screen& screen: screen to draw the paddle on.
     * SDL_Color color: color of the paddle. Default is white.
     */
    void draw(Screen& screen, SDL_Color color = {255, 255, 255, 255});

    /**
     * Move the paddle to the left.
     * 
     * Params:
     * const int& edge: left edge of the screen.
     */
    void move_left(const int& edge);

    /**
     * Move the paddle to the right.
     * 
     * Params:
     * const int& edge: right edge of the screen.
     */
    void move_right(const int& edge);

    /**
     * Reset the paddle to the original position.
     */
    void reset();

    /**
     * Get the left edge of the paddle.
     */
    int left() const;

    /**
     * Get the right edge of the paddle.
     */
    int right() const;

    /**
     * Get the top edge of the paddle.
     */
    int top() const;

    /**
     * Get the bottom edge of the paddle.
     */
    int bottom() const;

    /**
     * Get the width of the paddle.
     */
    int width() const;

    /**
     * Get the height of the paddle.
     */
    int height() const;
};

#endif // !PADDLE_H