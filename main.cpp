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
#include "SDL_ttf.h"

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

    const int score_font_size;
    const int score_pos_x;
    const int score_pos_y;
    const int num_of_balls;
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
    SDL_Color color;
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

struct Score
{
    TTF_Font* font;
    SDL_Surface* text;
    SDL_Texture* text_texture;
    SDL_Rect dest;

    int points;
    int balls_remaining;
    int bricks_remaining;
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
int init(SDL_Window*& window, SDL_Renderer*& renderer, const GameSettings& game_settings) 
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
        game_settings.screen_width, 
        game_settings.screen_height, 
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

    if (TTF_Init() < 0) 
    {
        SDL_Log("TTF could not initialize! TTF_Error: %s", TTF_GetError());
        return 4;
    }

    return 0;
}

Score create_score(std::string_view font_path, const GameSettings& game_settings)
{
    TTF_Font* font = TTF_OpenFont(font_path.data(), 20);
    if (!font) 
    {
        SDL_Log("Failed to load font: %s\n", TTF_GetError());
    }

    return Score {
        .font = font,
        .points = 0,
        .balls_remaining = game_settings.num_of_balls
    };
}

/**
 * Create bricks for the game.
 * 
 * Params:
 * const GameSettings& game_settings: game settings created at the start of the program. 
 *      Requires brick_rows, brick_cols, brick_width, brick_height, brick_spacing.
 * 
 * Returns:
 * std::vector<Brick>: vector of bricks created for the game.
 * 
 * Note: Bricks are created in a grid pattern with a spacing between them.
 *      Bricks can have two colors, red or yellow, where red is 10 points and yellow 20.
 * 
 */
std::vector<Brick> create_bricks(const GameSettings& game_settings)
{
    std::vector<Brick> bricks;
    for (int row = 0; row < game_settings.brick_rows; row++) 
    {
        for (int col = 0; col < game_settings.brick_cols; col++) 
        {
            Brick b = {
                .rect = SDL_Rect{
                    col * game_settings.brick_width, 
                    (game_settings.starting_row + row) * game_settings.brick_height, 
                    game_settings.brick_width - game_settings.brick_spacing, 
                    game_settings.brick_height - game_settings.brick_spacing
                },
                .is_visible = true,
            };

            if (row % 2 == 0) 
            {
                b.points = 10;
                b.color = SDL_Color{ 255, 0, 0, 255 };
            }
            else 
            {
                b.points = 20;
                b.color = SDL_Color{ 255, 255, 0, 255 };
            }

            bricks.push_back(b);
        }
    }
    return bricks;
}

/**
 * Create the paddle for the game. The paddle starts at the bottom center of the screen.
 * 
 * Params:
 * const GameSettings& game_settings: game settings created at the start of the program.
 *      Requires paddle_width, paddle_height, paddle_offset.
 */
SDL_Rect create_paddle(const GameSettings& game_settings)
{
    return SDL_Rect{ 
        .x = game_settings.screen_width / 2 - game_settings.paddle_width / 2, 
        .y = game_settings.screen_height - game_settings.paddle_offset, 
        .w = game_settings.paddle_width, 
        .h = game_settings.paddle_height 
    };
}

/**
 * Create the ball for the game. The ball starts at the center of the screen with a velocity towards the top.
 * 
 * Params:
 * const GameSettings& game_settings: game settings created at the start of the program.
 *    Requires ball_size, ball_speed.
 */
Ball create_ball(const GameSettings& game_settings)
{
    return Ball {
        .rect = SDL_Rect{ 
            .x = game_settings.screen_width / 2, 
            .y = game_settings.screen_height / 2, 
            .w = game_settings.ball_size, 
            .h = game_settings.ball_size 
        },
        .velocity_x = game_settings.ball_speed,
        .velocity_y = -game_settings.ball_speed,
        .is_moving = false
    };
}

/**
 * Reset the ball to the paddle.
 * 
 * Params:
 * Ball& ball: ball to reset.
 * const SDL_Rect& paddle: paddle to reset the ball to.
 * const GameSettings& game_settings: game settings created at the start of the program.
 *      Requires paddle_width, ball_size.
 * 
 */
void reset_ball(Ball& ball, const SDL_Rect& paddle, const GameSettings& game_settings)
{
    ball.rect.x = paddle.x + game_settings.paddle_width/2 - game_settings.ball_size/2;
    ball.rect.y = paddle.y - 1 - game_settings.ball_size;
    ball.is_moving = false;
}

/**
 * Update the score texture with the new score points.
 * 
 * Params:
 * SDL_Renderer* renderer: renderer to draw the score.
 * Score& score: score structure containing the font, surface and texture to update.
 * const GameSettings& game_settings: game settings created at the start of the program.
 * 
 * Note: This function is called inside the game loop only when the score points are updated.
 */
void update_score(SDL_Renderer* renderer, Score& score, const GameSettings& game_settings)
{
    char status_string[50];
    sprintf(status_string, "Score: %d | Lives: %d", score.points, score.balls_remaining);
    
    score.text = TTF_RenderText_Solid(score.font, status_string, { 255, 255, 255 });
    score.text_texture = SDL_CreateTextureFromSurface(renderer, score.text);
    score.dest = {0, game_settings.screen_height - score.text->h - 1, score.text->w, score.text->h };
}

/**
 * Free the score texture and surface.
 * 
 * Params:
 * Score& score: score structure conaining the texture and the surface to free.
 * 
 * Note: This function is called inside the game loop only when the score points are updated. 
 * Otherwise, we reuse the texture.
 */
void free_score_texture(Score& score)
{
    if (score.text_texture)
    {
        SDL_DestroyTexture(score.text_texture);
        score.text_texture = nullptr;
    }
    if (score.text)
    {
        SDL_FreeSurface(score.text);
        score.text = nullptr;
    }
}

/**
 * Render bricks on the screen. Right now, bricks are unicolor and hardcoded to be red.
 * 
 * Params:
 *  SDL_Renderer* renderer: renderer to draw the bricks.
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
            SDL_SetRenderDrawColor(renderer, bricks[i].color.r, bricks[i].color.g, bricks[i].color.b, bricks[i].color.a);
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
 * Ball& ball: ball to launch.
 * const GameSettings& game_settings: game settings created at the start of the program.
 *      Requires screen_width, paddle_speed.
 * 
 */
int input_actions(SDL_Rect& paddle, Ball& ball, const GameSettings& game_settings)
{
    bool paddle_state = false;
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);      // left or right arrows for movement
    if (keyState[SDL_SCANCODE_LEFT] && paddle.x > 0) 
    {
        paddle.x -= game_settings.paddle_speed;
    }
    if (keyState[SDL_SCANCODE_RIGHT] && paddle.x + paddle.w < game_settings.screen_width) 
    {
        paddle.x += game_settings.paddle_speed;
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
 * Bounce the ball off the brick based on the collision point. Right side, left side, top or bottom.
 * 
 * Params:
 * Ball& ball: ball to bounce.
 * const Brick& brick: brick to bounce off.
 * const GameSettings& game_settings: game settings created at the start of the program.
 * 
 * Note: This function is called when the ball collides with a brick.
 */
void bounce_ball(Ball& ball, const Brick& brick, const GameSettings& game_settings)
{
    int overlap_left = ball.rect.x + game_settings.ball_size - brick.rect.x;
    int overlap_right = brick.rect.x + brick.rect.w - ball.rect.x;
    int overlap_top = ball.rect.y + game_settings.ball_size - brick.rect.y;
    int overlap_bottom = brick.rect.y + brick.rect.h - ball.rect.y;

    if (overlap_left < overlap_right && overlap_left < overlap_top && overlap_left < overlap_bottom) // left
    {
        ball.velocity_x = -ball.velocity_x;
    }
    else if (overlap_right < overlap_left && overlap_right < overlap_top && overlap_right < overlap_bottom) // right
    {
        ball.velocity_x = -ball.velocity_x;
    }
    else if (overlap_top < overlap_left && overlap_top < overlap_right && overlap_top < overlap_bottom) // top
    {
        ball.velocity_y = -ball.velocity_y;
    }
    else // bottom as a default
    {
        ball.velocity_y = -ball.velocity_y;
    }
}

/**
 * Move the ball based on its velocity. 
 * The ball bounces off the walls or the paddle.
 * 
 * Params:
 * Ball& ball: ball to move.
 * SDL_Rect& paddle: paddle to check for collision.
 * std::vector<Brick>& bricks: vector of bricks to check for collision.
 * int& points: points earned by the player.
 * int& balls_remaining: number of balls remaining.
 * const GameSettings& game_settings: game settings created at the start of the program.
 *    Requires screen_width, screen_height, ball_size. 
 * 
 */
void move_ball(
    Ball& ball, 
    const SDL_Rect& paddle, 
    std::vector<Brick>& bricks, 
    Score& score,
    const GameSettings& game_settings 
)
{
    bool paddle_collision = false;

    // Move the ball
    ball.rect.x += ball.velocity_x;
    ball.rect.y += ball.velocity_y;

    // Wall collisions
    if (ball.rect.x <= 0) // left wall
    {
        ball.rect.x = 0;
        ball.velocity_x = -ball.velocity_x;
    }
    if (ball.rect.x + game_settings.ball_size >= game_settings.screen_width) // right wall
    {
        ball.rect.x = game_settings.screen_width - game_settings.ball_size;
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
        ball.rect.y = paddle.y - 1 - game_settings.ball_size;
        ball.velocity_y = -ball.velocity_y;     // horizontal reflection, maybe add some randomness or paddle side dependency
        paddle_collision = true;
    }

    if (!paddle_collision)      // if ball collided with the paddle, no need to check for brick collisions
    {
        // Brick collisions
        for (int i = 0; i < bricks.size(); i++) 
        {
            if (bricks[i].is_visible && SDL_HasIntersection(&ball.rect, &bricks[i].rect)) 
            {
                bricks[i].is_visible = false;
                score.points += bricks[i].points;
                bounce_ball(ball, bricks[i], game_settings);
                break;
            }
        }

        // Reset if the ball falls below the paddle
        if (ball.rect.y > game_settings.screen_height) 
        {
            reset_ball(ball, paddle, game_settings);   // Ball needs to be reset
            score.balls_remaining--;
        }
    }

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
    Score& score,
    const GameSettings& game_constants
){
    bool running = true;
    SDL_Event e;

    constexpr int desired_fps = 60; // fps is bound to physics in here, TODO: decouple
    constexpr int delta = 1000 / desired_fps;

    int prev_frame_points = 0;
    int prev_frame_balls = score.balls_remaining;

    // Initial score render preparation
    update_score(renderer, score, game_constants);

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
            move_ball(ball, paddle, bricks, score, game_constants);
        }
        else                    // ball is on the paddle ready to be launched
        {   
            reset_ball(ball, paddle, game_constants);
        }

        if (score.balls_remaining < 0)
        {
            running = false;
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

        // Render score text as a texture -- create a new texture only if the score has changed
        if (prev_frame_points != score.points || prev_frame_balls != score.balls_remaining)
        {
            free_score_texture(score);
            update_score(renderer, score, game_constants);
        }
        SDL_RenderCopy(renderer, score.text_texture, nullptr, &score.dest);
        prev_frame_points = score.points;

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
 * Clean up the game resources.
 * 
 * Params:
 * SDL_Window* window: window to destroy.
 * SDL_Renderer* renderer: renderer to destroy.
 * Score& score: score structure containing the font, surface and texture to destroy.
 * 
 * Note: This function should be called at the end of the game. 
 */
void cleanup(SDL_Window* window, SDL_Renderer* renderer, Score& score)
{
    free_score_texture(score);
    TTF_CloseFont(score.font);
    score.font = nullptr;
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    TTF_Quit();
    SDL_Quit();
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

        .brick_spacing = 20,
        .brick_width = 800 / 10, 
        .brick_height = 40,

        .paddle_width = 100,
        .paddle_height = 10,
        .paddle_speed = 6,
        .paddle_offset = 80,

        .ball_size = 10,
        .ball_speed = 4,

        .score_font_size = 20,
        .score_pos_x = 20,
        .score_pos_y = 600 - 10,
        .num_of_balls = 3
    };

    if (init(window, renderer, game_constants) > 0) 
    {
        return -1;
    }
    SDL_RenderSetLogicalSize(renderer, game_constants.screen_width, game_constants.screen_height);
    SDL_RenderSetIntegerScale(renderer, SDL_bool::SDL_TRUE);

    // Create bricks
    std::vector<Brick> bricks = create_bricks(game_constants);
    SDL_Rect paddle = create_paddle(game_constants);
    Ball ball = create_ball(game_constants);
    Score score = create_score("assets/DejaVuSans.ttf", game_constants);

    game_loop(renderer, bricks, paddle, ball, score, game_constants);

    // Clean up
    cleanup(window, renderer, score);

    return 0;
}
