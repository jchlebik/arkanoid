#include <vector>

#include "Brick.h"
#include "BricksLayout.h"

#include "RowLayout.h"


RowLayout::RowLayout(const RowLayoutSettings& settings):
    m_settings(settings)
{
}

std::vector<Brick> RowLayout::create_bricks()
{
    std::vector<Brick> bricks;
    for (int row = 0; row < m_settings.brick_rows; row++) 
    {
        for (int col = 0; col < m_settings.brick_cols; col++) 
        {
            Brick b(
                col * m_settings.brick_width, 
                (m_settings.starting_row + row) * m_settings.brick_height, 
                m_settings.brick_width - m_settings.brick_spacing, 
                m_settings.brick_height - m_settings.brick_spacing,
                0,
                SDL_Color{ 0, 0, 0, 0 }
            );  // create a new brick
                
            if (row % 2 == 0)       // alternate colors and points for each row
            {
                b.set_points(10);
                b.set_color(SDL_Color{ 255, 0, 0, 255 });
            }
            else 
            {
                b.set_points(20);
                b.set_color(SDL_Color{ 255, 255, 0, 255 });
            }

            bricks.push_back(b);
        }
    }
    return bricks;
}
