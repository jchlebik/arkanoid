#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H


/**
 * GameSettings
 * 
 * Settings for the game.
 * 
 * int screen_width: width of the game screen resolution.
 * int screen_height: height of the game screen resolution.
 * 
 * int paddle_width: width of the paddle.
 * int paddle_height: height of the paddle.
 * int paddle_speed: speed of the paddle.
 * int paddle_offset: offset of the paddle from the bottom of the screen.
 * 
 * int ball_size: size of the ball.
 * int ball_speed: speed (velocity) of the ball in both directions.
 * int num_of_balls: number of balls the player has before the game ends.
 * 
 * int fps_limit: frames per second limit.
 */
struct GameSettings
{
    const int screen_width;
    const int screen_height; 
    
    const int paddle_width;
    const int paddle_height;
    const int paddle_speed;
    const int paddle_offset;
    
    const int ball_size;
    const int ball_speed;
    const int num_of_balls;

    const int fps_limit;

    GameSettings(
        const int screen_width,
        const int screen_height,
        const int paddle_width,
        const int paddle_height,
        const int paddle_speed,
        const int paddle_offset,
        const int ball_size,
        const int ball_speed,
        const int num_of_balls,
        const int fps_limit
    ) :
        screen_width{ screen_width },
        screen_height{ screen_height },
        paddle_width{ paddle_width },
        paddle_height{ paddle_height },
        paddle_speed{ paddle_speed },
        paddle_offset{ paddle_offset },
        ball_size{ ball_size },
        ball_speed{ ball_speed },
        num_of_balls{ num_of_balls },
        fps_limit{ fps_limit }
    {
    }
};

#endif // !GAMESETTINGS_H