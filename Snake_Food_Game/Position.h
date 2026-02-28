#pragma once

struct Position
{
    int row, col;

    Position(int r = 0, int c = 0) : row(r), col(c) {}

    bool operator==(const Position &other) const
    {
        return row == other.row && col == other.col;
    }

    bool operator<(const Position &other) const
    {
        if (row != other.row)
            return row < other.row;
        return col < other.col;
    }
};