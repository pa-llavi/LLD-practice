#include "Snake.h"

Snake::Snake()
{
    Position start(0, 0);
    body.push_back(start);
    positionSet.insert(start);
}

Position Snake::getHead() const { return body.front(); }
Position Snake::getTail() const { return body.back(); }
int Snake::getSize() const { return static_cast<int>(body.size()); }

bool Snake::occupied(const Position &pos) const
{
    return positionSet.count(pos) > 0;
}
void Snake::addHead(const Position &pos)
{
    body.push_front(pos);
    positionSet.insert(pos);
}
void Snake::removeTail()
{
    positionSet.erase(body.back());
    body.pop_back();
}