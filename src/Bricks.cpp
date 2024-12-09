#include <vector>

#include "SDL.h"

#include "Brick.h"
#include "BricksLayout.h"

#include "Bricks.h"

Bricks::Bricks(BricksLayout& layout)
{
    m_bricks = layout.create_bricks();
    m_brick_count = m_bricks.size();
}

std::vector<Brick>& Bricks::get_bricks()
{
    return m_bricks;
}

void Bricks::draw(Screen& screen)
{
    int brick_count = 0;
    for (auto& brick : m_bricks)
    {
        if (brick.is_visible())
        {
            brick.draw(screen);
            brick_count++;
        }
    }
}

int Bricks::get_brick_count() const
{
    return m_brick_count;
}

void Bricks::set_brick_count(int count)
{
    m_brick_count = count;
}

void Bricks::decrement_counter()
{
    m_brick_count--;
}

void Bricks::reset()
{
    for (auto& brick : m_bricks)
    {
        brick.set_visible(true);
    }
    m_brick_count = m_bricks.size();
}

std::vector<Brick>::iterator Bricks::begin() { return this->m_bricks.begin(); }
std::vector<Brick>::const_iterator Bricks::cbegin() const { return this->m_bricks.begin(); }
std::vector<Brick>::iterator Bricks::end() { return this->m_bricks.end(); }
std::vector<Brick>::const_iterator Bricks::cend() const { return this->m_bricks.end(); }
