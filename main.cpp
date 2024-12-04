/**
 * Arkanoid game
 * 
 * This is a simple Arkanoid game implementation using SDL2.
 * 
 * The game has a paddle, a ball and bricks. The player has to hit the bricks with the ball using the paddle.
 * The ball bounces off the walls and the paddle. When the ball hits a brick, the brick disappears and the player earns points.
 * 
 * Each ball missed costs a life. The game ends when all the bricks are destroyed or the player runs out of lives.
 *  
 */

#include <vector>
#include <iostream>

#include "SDL.h"

// Constants
struct GameSettings
{
    const int screen_width;
    const int screen_height;

    const int starting_row;
    const int brick_rows;
    const int brick_cols;
    const int brick_spacing;
    const int brick_width;
    const int brick_height;
};

/**
 * Ball struct
 * 
 * SDL_Rect rect: x, y, width, height. x, y are the top left corner.
 * bool isVisible: this brick was hit by the ball yet or not.
 * int points: number of points player earns by hiting this brick.
 * 
 */
struct Brick 
{
    SDL_Rect rect;
    bool isVisible;
    int points;
};

/**
 * Initialize the game.
 * 
 * Params:
 * SDL_Window*& window: an output pointer to a window to render the game.
 * SDL_Renderer*& renderer: an output pointer to a renderer to render the game.
 *
 * Returns:
 * int: 0 if initialization was successful, otherwise an error code.
 */
int init(SDL_Window*& window, SDL_Renderer*& renderer, const GameSettings& game_constants) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        SDL_Log("SDL could not initialize! SDL_Error: %s \n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Arkanoid", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        game_constants.screen_width, 
        game_constants.screen_height, 
        SDL_WINDOW_SHOWN
    );
    if (!window) 
    {
        SDL_Log("Window could not be created! SDL_Error: %s \n", SDL_GetError());
        return 2;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) 
    {
        SDL_Log("Renderer could not be created! SDL_Error: %s \n", SDL_GetError());
        return 3;
    }

    return 0;
}

/**
 * Render bricks on the screen. Right now, bricks are unicolor and hardcoded to be red.
 * 
 * Params:
 *  const std::vector<Brick>& bricks: vector of bricks to render.
 * 
 * Returns:
 * int: number of bricks still remaining on the screen.
 */
int render_bricks(SDL_Renderer* renderer, const std::vector<Brick>& bricks) 
{
    int remainingBricks = 0;
    for (int i = 0; i < bricks.size(); i++)
    {
        if (bricks[i].isVisible) 
        {
            remainingBricks++;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            SDL_RenderFillRect(renderer, &bricks[i].rect);
        }
    }
    return remainingBricks;
}

/**
 * Create bricks for the game.
 * 
 * Params:
 * int brick_rows: number of rows of bricks.
 * int brick_cols: number of columns of bricks.
 * int brick_width: pixel width of each brick.
 * int brick_height: pixel height of each brick.
 * int bricks_spacing: pixel spacing between bricks.
 */
std::vector<Brick> create_bricks(const GameSettings& game_constants)
{
    std::vector<Brick> bricks;
    for (int row = 0; row < game_constants.brick_rows; row++) 
    {
        for (int col = 0; col < game_constants.brick_cols; col++) 
        {
            bricks.push_back(
                Brick {
                    .rect = SDL_Rect{
                        col * game_constants.brick_width, 
                        row * game_constants.brick_height, 
                        game_constants.brick_width - game_constants.brick_spacing, 
                        game_constants.brick_height - game_constants.brick_spacing
                    },
                    .isVisible = true,
                    .points = 10
                }
            );
        }
    }
    return bricks;
}

/**
 * Game loop for the Arkanoid game. 
 * 
 * Params:
 * SDL_Renderer* renderer: renderer to draw the game.
 * const std::vector<Brick>& bricks: vector of bricks to render.
 * 
 */
void game_loop(SDL_Renderer* renderer, const std::vector<Brick>& bricks)
{
    bool running = true;
    SDL_Event e;

    while (running) 
    {
        // Handle events
        while (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_QUIT)  // Quit the game
            {
                running = false;
            }
        }

        // Paddle actions
        const Uint8* keyState = SDL_GetKeyboardState(nullptr);
        if (keyState[SDL_SCANCODE_Q] || keyState[SDL_SCANCODE_ESCAPE])
        {
            running = false;
        }
    

        // Redraw the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer);

        // Redraw the bricks
        if (render_bricks(renderer, bricks) == 0)
        {   
            running = false;
        }

        // Update render
        SDL_RenderPresent(renderer);
    }

}

/**
 * Main function
 * 
 * Initializes the game and runs the game loop.
 * 
 */
int main() 
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    
    GameSettings game_constants{
        .screen_width = 800, 
        .screen_height = 600, 
        .starting_row = 2, 
        .brick_rows = 5, 
        .brick_cols = 10, 
        .brick_spacing = 4,
        .brick_width = 800 / 10, 
        .brick_height = 30
    };

    if (init(window, renderer, game_constants) > 0) 
    {
        return -1;
    }

    // Create bricks
    std::vector<Brick> bricks = create_bricks(game_constants);

    game_loop(renderer, bricks);

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
