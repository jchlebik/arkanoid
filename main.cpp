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

    const int paddle_width;
    const int paddle_height;
    const int paddle_speed;
    const int paddle_offset;

    const int ball_size;
    const int ball_speed;
};

/**
 * Ball struct
 * 
 * SDL_Rect rect: x, y, width, height. x, y are the top left corner.
 * bool is_visible: this brick was hit by the ball yet or not.
 * int points: number of points player earns by hiting this brick.
 * 
 */
struct Brick 
{
    SDL_Rect rect;
    bool is_visible;
    int points;
};


/**
 * Ball struct
 * 
 * SDL_Rect rect: x, y, width, height. x, y are the top left corner.
 * int velocity_x: velocity in the x direction.
 * int velocity_y: velocity in the y direction.
 * 
 */
struct Ball
{
    SDL_Rect rect;
    int velocity_x;
    int velocity_y;
    bool is_moving;
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
                    .is_visible = true,
                    .points = 10
                }
            );
        }
    }
    return bricks;
}

/**
 * Create the paddle for the game. The paddle starts at the bottom center of the screen.
 * 
 * Params:
 * const GameSettings& game_constants: game settings created at the start of the program.
 */
SDL_Rect create_paddle(const GameSettings& game_constants)
{
    return SDL_Rect{ 
        .x = game_constants.screen_width / 2 - game_constants.paddle_width / 2, 
        .y = game_constants.screen_height - game_constants.paddle_offset, 
        .w = game_constants.paddle_width, 
        .h = game_constants.paddle_height 
    };
}

/**
 * Create the ball for the game. The ball starts at the center of the screen with a velocity towards the top.
 * 
 * Params:
 * const GameSettings& game_constants: game settings created at the start of the program.
 */
Ball create_ball(const GameSettings& game_constants)
{
    return Ball {
        .rect = SDL_Rect{ 
            .x = game_constants.screen_width / 2, 
            .y = game_constants.screen_height / 2, 
            .w = game_constants.ball_size, 
            .h = game_constants.ball_size 
        },
        .velocity_x = game_constants.ball_speed,
        .velocity_y = -game_constants.ball_speed,
        .is_moving = false
    };
}

/**
 * Reset the ball to the paddle.
 * 
 * Params:
 * Ball& ball: ball to reset.
 * const SDL_Rect& paddle: paddle to reset the ball to.
 */
void reset_ball(Ball& ball, const SDL_Rect& paddle, const GameSettings& game_constants)
{
    ball.rect.x = paddle.x + game_constants.paddle_width/2 - game_constants.ball_size/2;
    ball.rect.y = paddle.y - 1 - game_constants.ball_size;
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
    int remaining_bricks = 0;
    for (int i = 0; i < bricks.size(); i++)
    {
        if (bricks[i].is_visible) 
        {
            remaining_bricks++;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            SDL_RenderFillRect(renderer, &bricks[i].rect);
        }
    }
    return remaining_bricks;
}

/**
 * Move the paddle left or right based on the key press.
 * 
 * Params:
 * SDL_Rect& paddle: paddle to move.
 * const GameSettings& game_constants: game settings created at the start of the program.
 * 
 */
int input_actions(SDL_Rect& paddle, Ball& ball, const GameSettings& game_constants)
{
    bool paddle_state = false;
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);      // left or right arrows for movement
    if (keyState[SDL_SCANCODE_LEFT] && paddle.x > 0) 
    {
        paddle.x -= game_constants.paddle_speed;
    }
    if (keyState[SDL_SCANCODE_RIGHT] && paddle.x + paddle.w < game_constants.screen_width) 
    {
        paddle.x += game_constants.paddle_speed;
    }
    if (keyState[SDL_SCANCODE_SPACE] && !ball.is_moving)
    {
        ball.is_moving = true;
    }
    if (keyState[SDL_SCANCODE_Q] || keyState[SDL_SCANCODE_ESCAPE])
    {
        return 1;
    }
    return 0;
}


/**
 * Move the ball based on its velocity. 
 * The ball bounces off the walls and does not interact with the bricks or paddle for now.
 * 
 * Params:
 * Ball& ball: ball to move.
 * const GameSettings& game_constants: game settings created at the start of the program.
 * 
 */
void move_ball(Ball& ball, SDL_Rect& paddle, std::vector<Brick>& bricks, const GameSettings& game_constants)
{
    bool paddle_collision = false;
    ball.rect.x += ball.velocity_x;
    ball.rect.y += ball.velocity_y;

    // Wall collisions
    if (ball.rect.x <= 0) // left wall
    {
        ball.rect.x = 0;
        ball.velocity_x = -ball.velocity_x;
    }
    if (ball.rect.x + game_constants.ball_size >= game_constants.screen_width) // right wall
    {
        ball.rect.x = game_constants.screen_width - game_constants.ball_size;
        ball.velocity_x = -ball.velocity_x;
    }
    if (ball.rect.y <= 0) // top wall
    {
        ball.rect.y = 0;
        ball.velocity_y = -ball.velocity_y;
    }

    // Paddle collision
    if (SDL_HasIntersection(&ball.rect, &paddle)) 
    {
        ball.rect.y = paddle.y - 1 - game_constants.ball_size;
        ball.velocity_y = -ball.velocity_y;
        paddle_collision = true;
    }

    if (!paddle_collision)
    {
        // Brick collisions
        for (int i = 0; i < bricks.size(); i++) 
        {
            if (bricks[i].is_visible && SDL_HasIntersection(&ball.rect, &bricks[i].rect)) 
            {
                bricks[i].is_visible = false;
                ball.velocity_y = -ball.velocity_y;
                //points += bricks[i].points;
                break;
            }
        }

        // Reset if the ball falls below the paddle
        if (ball.rect.y > game_constants.screen_height) 
        {
            reset_ball(ball, paddle, game_constants);   // Ball needs to be reset
            ball.is_moving = false;                               
        }
    }

}

void reset_ball(Ball& ball, SDL_Rect& paddle, const GameSettings& game_constants)
{
    ball.rect.x = paddle.x + paddle.w / 2 - game_constants.ball_size / 2;
    ball.rect.y = paddle.y - game_constants.ball_size;
    ball.is_moving = false;
}
/**
 * Game loop for the Arkanoid game. 
 * 
 * Params:
 * SDL_Renderer* renderer: renderer to draw the game.
 * const std::vector<Brick>& bricks: vector of bricks to render.
 * 
 */
void game_loop(
    SDL_Renderer* renderer,
    std::vector<Brick>& bricks, 
    SDL_Rect& paddle,
    Ball& ball,
    const GameSettings& game_constants)
{
    bool running = true;
    int balls_remaining = 3;
    SDL_Event e;

    constexpr int desired_fps = 60;
    constexpr int delta = 1000 / desired_fps;

    while (running) 
    {
        Uint64 start = SDL_GetTicks64();
        // Handle events
        while (SDL_PollEvent(&e) != 0) 
        {
            if (e.type == SDL_QUIT)  // Quit the game
            {
                running = false;
            }
        }

        // Handle inputs 
        if (input_actions(paddle, ball, game_constants) > 0)
        {
            running = false;
        }

        if (ball.is_moving)     // ball is moving
        {
            move_ball(ball, paddle, bricks, game_constants);
        }
        else                    // ball is on the paddle ready to be launched
        {   
            reset_ball(ball, paddle, game_constants);
        }

        // Reset the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer);

        // Redraw the paddle
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
        SDL_RenderFillRect(renderer, &paddle);

        // Redraw the ball
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
        SDL_RenderFillRect(renderer, &ball.rect);

        // Redraw the bricks
        if (render_bricks(renderer, bricks) == 0)
        {   
            running = false;
        }

        // Update render
        SDL_RenderPresent(renderer);

        float elapsed_ms = (SDL_GetTicks64() - start) ;
        if (elapsed_ms < delta)
        {
            // Cap to 60 FPS
            SDL_Delay(delta - elapsed_ms);
        }
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
        .brick_height = 30,

        .paddle_width = 100,
        .paddle_height = 10,
        .paddle_speed = 6,
        .paddle_offset = 80,

        .ball_size = 10,
        .ball_speed = 4
    };

    if (init(window, renderer, game_constants) > 0) 
    {
        return -1;
    }

    // Create bricks
    std::vector<Brick> bricks = create_bricks(game_constants);
    SDL_Rect paddle = create_paddle(game_constants);
    Ball ball = create_ball(game_constants);

    game_loop(renderer, bricks, paddle, ball, game_constants);

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
