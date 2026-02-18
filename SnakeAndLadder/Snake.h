#ifndef SNAKE_H
#define SNAKE_H

#include <stdexcept>

class Snake
{
    int head;
    int tail;

public:
    Snake(int start, int end) : head(start), tail(end)
    {
        if (start <= end)
        {
            throw std::invalid_argument("Snake head must be at a higher position than tail!");
        }

        if (start <= 0 || tail <= 0)
            throw std::invalid_argument("Snake positions must me positive");
    }

    int getHead() const { return head; }

    int getTails() const { return tail; }

    bool hasSnakeAt(int position) const { return position == head; };
};

#endif