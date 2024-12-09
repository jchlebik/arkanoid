#ifndef SCORE_H
#define SCORE_H

#include <stdexcept>
#include <memory>
#include <string_view>
#include <optional>
#include <cassert>

#include "SDL.h"
#include "SDL_ttf.h"

#include "Screen.h"

/**
 * RAII for SDL resources needed to render score on screen.
 * 
 * Score is rendered onto a surface, which is then rendered onto a texture on screen.
 * 
 * std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> m_font_ptr: unique_ptr to the font resource. 
 *      Held for the entire run, just changing the font size. Font is loaded from the assets folder.
 * std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> m_text_ptr: unique_ptr to the surface resource.
 *      Surface is created from the font and the text to render. Cleared and reinitilized every score update.
 * std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_text_texture_ptr: unique_ptr to the texture resource.
 *      Texture is created from the surface. Cleared and reinitilized every score update.
 * 
 * const char* m_score_format_string: format string for the score.
 * int m_font_size: size of the font.
 * int m_num_balls: number of balls the player has.
 * 
 * int m_points: number of points the player has.
 * int m_balls_remaining: number of balls the player has remaining.
 * 
 */
class Score
{
    std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> m_font_ptr{nullptr, TTF_CloseFont};
    std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> m_text_ptr{nullptr, SDL_FreeSurface};
    std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_text_texture_ptr{nullptr, SDL_DestroyTexture};
    const char* m_score_format_string = "Score: %d | Lives: %d";
    int m_font_size;
    int m_num_balls;

public:
    int m_points;
    int m_balls_remaining;

    /**
     * Constructor for the Score class.
     * 
     * Params:
     * const std::string_view font_path: path to the font file. MUST be zero terminated.
     * int font_size: size of the font.
     */
    Score(const std::string_view font_path, int font_size, int num_balls);

    ~Score();

    /**
     * Get the width of the text box.
     */
    int get_text_width() const;

    /**
     * Get the height of the text box.
     */
    int get_text_height() const;

    /**
     * Draw the score on the screen.
     * 
     * Params:
     * Screen& screen: screen to draw the score on.
     * std::optional<int> x: x position of the top left corner of the text box. Default is 0.
     * std::optional<int> y: y position of the top left corner of the text box. Default is screen height - text height - 2.
     */
    void draw(Screen& screen, std::optional<int> x = std::nullopt, std::optional<int> y = std::nullopt);

    /**
     * Prepare the score to be rendered on the screen.
     * 
     * Params:
     * Screen& screen: screen to draw the score on.
     * Score& score: score structure containing the font, surface and texture to prepare.
     * const SDL_Color& color: color of the text.
     */
    void prepare(Screen& screen, const SDL_Color& color);

    /**
     * Prepare the score to be rendered on the screen. Overloaded function to render a custom string.
     * 
     * Params:
     * Screen& screen: screen to draw the score on.
     * const std::string_view status_string: string to render.
     * const SDL_Color& color: color of the text.
     */
    void prepare(Screen& screen, const std::string_view status_string, const SDL_Color& color);

    /**
     * Change the font size of the score.
     * 
     * Params:
     * int new_font_size: new font size.
     */
    void change_font_size(int new_font_size);

    /**
     * Increment the score by the given number of points.
     * 
     * Params:
     * int points: number of points to add.
     */
    void decrement_counter();

    /**
     * Increment the score by the given number of points.
     * 
     * Params:
     * int points: number of points to add.
     */
    void add_points(int points);

    /**
     * Get the current score.
     */
    int get_points() const;

    /**
     * Get the number of balls remaining.
     */
    int get_balls_remaining() const;

    /**
     * Reset the score to the original values.
     */
    void reset();
};

#endif // !SCORE_H