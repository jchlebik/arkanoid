#ifndef BALL_H
#define BALL_H

#include <cassert>

#include "SDL.h"

#include "Screen.h"
#include "Paddle.h"
#include "Bricks.h"
#include "Score.h"

/**
 * Ball class represents the ball in the game. It can move, bounce off the screen, paddle or bricks.
 * The ball can be active or inactive. When inactive, it is reset to the paddle.
 * 
 * SDL_Rect m_rect: rectangle representing the ball.
 * int m_velocity_x: velocity of the ball in x direction.
 * int m_velocity_y: velocity of the ball in y direction.
 * bool m_is_moving: is the ball moving or not.
 * 
 * const int m_original_velocity_x: original velocity of the ball in x direction when the instance was constructed.
 * const int m_original_velocity_y: original velocity of the ball in y direction when the instance was constructed.
 * 
 * Public Methods:
 *  - bool interact(): interact with the game objects. Move the ball, bounce off the screen, paddle or bricks.
 *  - void set_moving(): set the ball to be moving or not moving.
 *  - bool is_moving(): check if the ball is moving or not.
 *  - void set_velocity_x(): set the velocity of the ball in x direction.
 *  - void set_velocity_y(): set the velocity of the ball in y direction.
 *  - void reset_to_paddle(): reset the ball to the paddle.
 *  - void draw(): draw the ball on the screen.
 * 
 * Private Methods:
 *  - void move_forward(): move the ball with the given velocity.
 *  - void bounce_from_screen(): bounce the ball off the screen walls.
 *  - void bounce_from_paddle(): bounce the ball off the paddle.
 *  - void bounce_from_brick(): bounce the ball off the brick.
 *  - void bounce_x(): bounce the ball in x direction.
 *  - void bounce_y(): bounce the ball in y direction.
 */
class Ball
{
    SDL_Rect m_rect;
    int m_velocity_x;
    int m_velocity_y;
    bool m_is_moving;

    const int m_original_velocity_x;
    const int m_original_velocity_y;


public:

    /**
     * Constructor for the Ball class.
     * 
     * Params:
     * int ball_size: size of the ball.
     * int velocity_x: velocity of the ball in x direction.
     * int velocity_y: velocity of the ball in y direction.
     * bool is_moving: is the ball moving or not (game is active or waiting for launch with spacebar).
     * 
     */
    Ball(int ball_size, int velocity_x, int velocity_y, bool is_moving);

    /**
     * Interact with the game objects. Move the ball, bounce off the screen, paddle or bricks.
     * 
     * Params:
     * const Screen& screen: screen to bounce off.
     * Paddle& paddle: paddle to bounce off.
     * Bricks& bricks: bricks to bounce off.
     * Score& score: score to update.
     * 
     */
    bool interact(const Screen& screen, Paddle& paddle, Bricks& bricks, Score& score);
    
    /**
     * Set the ball to be moving or not moving.
     */
    void set_moving(const bool moving);

    /**
     * Check if the ball is moving or not.
     */
    bool is_moving() const;

    /**
     * Set the velocity of the ball in x direction.
     */
    void set_velocity_x(const int velocity);

    /**
     * Set the velocity of the ball in y direction.
     */
    void set_velocity_y(const int velocity);

    /**
     * Reset the ball to the paddle.
     * 
     * Params:
     * const Paddle& paddle: paddle to reset the ball to.
     * 
     */
    void reset_to_paddle(const Paddle& paddle);

    /**
     * Draw the ball on the screen.
     * 
     * Params:
     * Screen& screen: screen instance holding the SDL resources.
     * SDL_Color color: color of the ball. Default is white.
 
     */
    void draw(Screen& screen, SDL_Color color = {255, 255, 255, 255});

private:

    /**
     * Move the ball with the given velocity. No dt is used in this simple game.
     */
    void move_forward();

    /**
     * Bounce the ball off the screen walls.
     * 
     * Params:
     * const Screen& screen: screen to bounce off.
     * 
     */
    void bounce_from_screen(const Screen& screen);

    /**
     * Bounce the ball off the paddle.
     * 
     * Params:
     * const Paddle& paddle: paddle to bounce off.
     * 
     */
    void bounce_from_paddle(const Paddle& paddle);

    /**
     * Bounce the ball off the brick based on the collision point. Right side, left side, top or bottom.
     * 
     * Params:
     * const Brick& brick: brick to bounce off.
     */
    void bounce_from_brick(const Brick& brick);

    /**
     * Bounce the ball in x ax.
     */
    void bounce_x();

    /**
     * Bounce the ball in y ax.
     */
    void bounce_y();

};

#endif // !BALL_H