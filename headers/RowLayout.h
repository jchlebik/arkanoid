#ifndef ROW_LAYOUT_H
#define ROW_LAYOUT_H

#include <vector>

#include "Brick.h"
#include "BricksLayout.h"


/**
 * LayoutSettings struct holds the settings for the row layout of the bricks.
 * 
 * int starting_row: row from the top where the bricks start. 1 row = 1 brick height + spacing
 * int brick_rows: number of rows of bricks.
 * int brick_cols: number of columns of bricks.
 * int brick_spacing: spacing between bricks.
 * int brick_width: width of a brick before acounting for spacing. The actual width of a brick ends up being brick_width - brick_spacing.
 * int brick_height: height of a brick before acounting for spacing. The actual height of a brick ends up being brick_height - brick_spacing.
 * 
 */
struct RowLayoutSettings
{
    const int starting_row;
    const int brick_rows;
    const int brick_cols;
    const int brick_spacing;
    const int brick_width;
    const int brick_height;
};


/**
 * RowLayout class is a concrete implementation of the BricksLayout interface. Layouts the bricks in rows.
 * Passed to the ArkanoidGame class using polymorphism.
 * 
 * Public Methods:
 * - RowLayout(): constructor that takes a layout settings.
 * - std::vector<Brick> create_bricks(): creates the bricks in rows.
 * 
 */

class RowLayout : public BricksLayout
{
    const RowLayoutSettings m_settings;

public:
    RowLayout(const RowLayoutSettings& settings);

    /**
     * Create the bricks in rows. Each row has alternating colors and points.
     * 
     * Returns:
     * std::vector<Brick>: vector of bricks.
     */
    std::vector<Brick> create_bricks();
};

#endif // !ROW_LAYOUT_H