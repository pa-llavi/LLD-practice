#pragma once

#include "GameBoard.h"
#include "Snake.h"
#include <vector>
#include <string>
#include <random>

class SnakeGame
{
    GameBoard *board;
    Snake snake;
    Position currFood;
    int score;
    std::mt19937 rng;

    Position getNextPosition(const Position &head, const std::string &direction);
    void spawnFood();

public:
    SnakeGame(int width, int height);

    int move(const std::string &direction);
    int getSnakeSize() const;
    int getScore() const;
    void displayBoard() const;
    // Position getFoodPosition() const;
};