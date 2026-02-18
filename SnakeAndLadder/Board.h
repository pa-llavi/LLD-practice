#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "Snake.h"
#include "Ladder.h"

class Board
{
private:
    int boardSize;
    std::unordered_map<int, Snake> snakesMap;
    std::unordered_map<int, Ladder> laddersMap;

public:
    Board();
    Board(int size);

    void addSnake(const Snake &snake);
    void addLadder(const Ladder &ladder);
    int getNewPositionAfterSnakeOrLadder(int position) const;
    int getSize() const;
    bool isValidPosition(int position) const;
    const std::vector<Snake> getSnakes() const;
    const std::vector<Ladder> getLadders() const;
};

#endif