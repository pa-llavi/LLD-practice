#include "SnakeGame.h"
#include <iostream>

SnakeGame::SnakeGame(int width, int height) : board(GameBoard::getInstance(width, height)), score(0), rng(std::random_device{}())
{
    spawnFood();
}

void SnakeGame::spawnFood()
{
    std::uniform_int_distribution<int> rowDist(0, board->getHeight() - 1);
    std::uniform_int_distribution<int> colDist(0, board->getWidth() - 1);

    // keep generatingnuntil we find a cell not occupied by snake
    do
    {
        currFood = {rowDist(rng), colDist(rng)};
    } while (snake.occupied(currFood));
}

Position SnakeGame::getNextPosition(const Position &head, const std::string &direction)
{
    if (direction == "U")
        return {head.row - 1, head.col};
    if (direction == "D")
        return {head.row + 1, head.col};
    if (direction == "L")
        return {head.row, head.col - 1};
    if (direction == "R")
        return {head.row, head.col + 1};
}

int SnakeGame::move(const std::string &direction)
{
    Position currentHead = snake.getHead();
    Position newHead = getNextPosition(currentHead, direction);

    // boundary check
    if (newHead.row < 0 || newHead.row >= board->getHeight() ||
        newHead.col < 0 || newHead.col >= board->getWidth())
        return -1;

    // self-collision check, tail will move away so exclude it
    Position currentTail = snake.getTail();
    if (snake.occupied(newHead) && !(newHead == currentTail))
    {
        return -1;
    }

    // food check
    if (currFood == newHead)
    {
        score++;
        spawnFood();
    }
    else
        snake.removeTail();

    // advance head
    snake.addHead(newHead);
    return score;
}

int SnakeGame::getSnakeSize() const
{
    return snake.getSize();
}

int SnakeGame::getScore() const { return score; }

void SnakeGame::displayBoard() const
{
    int H = board->getHeight();
    int W = board->getWidth();
    Position head = snake.getHead();

    // top border
    for (int c = 0; c < W; c++)
    {
        std::cout << "-";
    }
    std::cout << "--\n";

    for (int r = 0; r < H; r++)
    {
        std::cout << "|";
        for (int c = 0; c < W; c++)
        {
            Position p(r, c);
            if (p == head)
            {
                std::cout << "O"; // snake head
            }
            else if (snake.occupied(p))
            {
                std::cout << "*"; // snake body
            }
            else if (p == currFood)
            {
                std::cout << "x"; // food
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << "|\n";
    }

    // bottom border
    for (int c = 0; c < W; c++)
    {
        std::cout << "-";
    }
    std::cout << "--\n";
}

// Position SnakeGame::getFoodPosition() const { return currFood; }