#ifndef LADDER_H
#define LADDER_H

#include <stdexcept>

class Ladder
{
private:
    int start;
    int end;

public:
    Ladder(int start, int end);

    int getStart() const;
    int getEnd() const;
    bool hasLadderAt(int position) const;
};

inline Ladder::Ladder(int start, int end) : start(start), end(end)
{
    if (start >= end)
        throw std::invalid_argument("Ladder end must be at a higher position than start!");

    if (start <= 0 || end <= 0)
        throw std::invalid_argument("Ladder positions must be positive");
}

inline int Ladder::getStart() const { return start; }

inline int Ladder::getEnd() const { return end; }

inline bool Ladder::hasLadderAt(int position) const { return position == start; }

#endif