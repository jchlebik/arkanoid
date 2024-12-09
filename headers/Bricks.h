#ifndef BRICKS_H
#define BRICKS_H

#include <vector>

#include "SDL.h"

#include "Brick.h"
#include "BricksLayout.h"

/**
 * Bricks class holds a vector of bricks. It is responsible for drawing the bricks on the screen.
 * It also keeps track of the number of bricks left on the screen. 
 * It also provides methods to be an iterable over the vector of bricks.
 * 
 * std::vector<Brick> m_bricks: vector of bricks.
 * int m_brick_count: number of bricks left on the screen.
 * 
 * Public Methods:
 *  - Bricks(): constructor that takes a layout and generates bricks from it using its create_bricks method.
 *  - std::vector<Brick>& get_bricks(): returns the vector of bricks.
 *  - void draw(): draws the bricks on the screen.  
 *  - int get_brick_count(): returns the number of bricks left on the screen.
 *  - void set_brick_count(): sets the number of bricks left on the screen.
 *  - void decrement_counter(): decrements the number of bricks left on the screen.
 *  - void reset(): resets the bricks to be visible.
 * 
 * 
 */
class Bricks
{
    std::vector<Brick> m_bricks;
    int m_brick_count;

public:

    /** 
     * Constructor for the Bricks class. It generates the bricks from the layout using its create_bricks method. 
     * 
     * Params:
     * BricksLayout& layout: layout of the bricks.
     * 
     */
    Bricks(BricksLayout& layout);

    /**
     * Get the reference to the vector of bricks.
     * 
     * Returns:
     * std::vector<Brick>&: vector of bricks.
     */
    std::vector<Brick>& get_bricks();

    /**
     * Draw the bricks on the screen.
     * 
     * Params:
     * Screen& screen: screen to draw the bricks on.
     */
    void draw(Screen& screen);

    /**
     * Get the number of bricks left on the screen.
     * 
     * Returns:
     * int: number of bricks left on the screen.
     */
    int get_brick_count() const;

    /**
     * Set the number of bricks left on the screen.
     * 
     * Params:
     * int count: number of bricks left on the screen.
     */
    void set_brick_count(int count);

    /**
     * Decrement the number of bricks left on the screen.
     */
    void decrement_counter();

    /**
     * Reset the bricks to be visible.
     */
    void reset();

    /**
     * Begin iterator for the bricks.
     * 
     * Returns:
     * auto: iterator to the beginning of the vector of bricks.
     */
    std::vector<Brick>::iterator begin();

    /**
     * Const begin iterator for the bricks.
     * 
     * Returns:
     * auto: const iterator to the beginning of the vector of bricks.
     */
    std::vector<Brick>::const_iterator cbegin() const;

    /** 
     * End iterator for the bricks.
     * 
     * Returns:
     * auto: iterator to the end of the vector of bricks.
     */
    std::vector<Brick>::iterator end();

    /**
     * Const end iterator for the bricks.
     * 
     * Returns:
     * auto: const iterator to the end of the vector of bricks.
     */
    std::vector<Brick>::const_iterator cend() const;
};

#endif // !BRICKS_H