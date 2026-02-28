#pragma once

#include "Position.h"
#include <deque>
#include <set>

class Snake
{
    std::deque<Position> body;
    std::set<Position> positionSet; // O(log n) collision lookup

public:
    Snake();
    Position getHead() const;
    Position getTail() const;
    int getSize() const;

    bool occupied(const Position &pos) const;
    void addHead(const Position &pos);
    void removeTail();
};