/**
 * ArkanoidGame.h
 * 
 * This file contains the main class of the game. It contains the game loop and the game logic.
 * 
 */

#ifndef ARKANOID_GAME_H
#define ARKANOID_GAME_H

#include <vector>
#include <iostream>

#include "SDL.h"

#include "Bricks.h"
#include "Screen.h"
#include "Ball.h"
#include "Paddle.h"
#include "FrameLimiter.h"
#include "BricksLayout.h"
#include "Score.h"
#include "GameSettings.h"


/**
 * Main class of the game. It contains the game loop and the game logic.
 * 
 * Params:
 * const GameSettings settings: settings for the game.
 * BricksLayout& bricks_layout: layout of the bricks.
 * 
 * Public Methods:
 * bool game_loop(): main game loop. Returns true if the player hard quit.
 * bool show_end_screen(): show the end screen. Returns true if the game should be restarted.
 * 
 * Private Methods:
 * void poll_for_events(): poll for SDL events.
 * void restart(): restart the game state in preparation for a new game.
 * void player_input(bool end_screen): handle player input. If end_screen is true, it handles the input for the end screen.
 * 
 * 
 */
class ArkanoidGame
{
    const GameSettings m_settings;  // game settings
    Screen m_screen;                // Holds the resources to screen to draw the game on in RAII pattern.
    Score m_score;                  // Score of the player. Holds the font, surface and texture resources in RAII pattern.
    Bricks m_bricks;               
    Ball m_ball;
    Paddle m_paddle;
    FrameLimiter m_frame_limiter;  

    bool m_running = true;          // game is running
    bool m_hard_quit = false;       // player hard quit
    bool m_restart = false;         // player wants to restart the game

public:
    ArkanoidGame(const GameSettings settings, BricksLayout& bricks_layout);

    /**
     * This method is the main game loop. It handles the game logic and rendering.
     * 
     * Returns:
     * bool: true if the player hard quit.
     * 
     */
    bool game_loop();
    
    /**
     * This method shows the end screen. It shows the player if they won or lost the game.
     * 
     * Returns:
     * bool: true if the player wants to restart the game.
     * 
     * Note: This method should be invoked when the player loses all the balls or wins the game. Q to quit, R to restart.
     */
    bool show_end_screen();

private:
    /**
     * This method is called in the game loop to handle the SDL events. 
     * Currently, it only handles the SDL_QUIT event.
     */
    void poll_for_events();

    /**
     * This method restarts the game state in preparation for a new game. 
     * Invoked at the start of the game loop function but before the actual looping.
     */
    void restart();

    /**
     * This method is called in the game loop to handle the player input. 
     * It handles the left, right, space, Q and ESC keys and an R key.
     * 
     * Params:
     *  bool end_screen: if true, it handles only the input for the end screen (Q or R).
     */
    void player_input(bool end_screen = false);
};

#endif // !ARKANOID_GAME_H