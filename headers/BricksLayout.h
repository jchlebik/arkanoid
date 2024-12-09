#ifndef BRICKSLAYOUT_H
#define BRICKSLAYOUT_H

#include <vector>
#include "Brick.h"

/**
 * Interface for creating bricks layouts. 
 * Each created layout must implement a create_bricks method that returns a vector of bricks.
 * Layouts can be created by implementing this interface and passing it to the ArkanoidGame class through polymorphism.
 */

class BricksLayout {
public:
    virtual std::vector<Brick> create_bricks() = 0;
    virtual ~BricksLayout() = default;
};

#endif // !BRICKSLAYOUT_H