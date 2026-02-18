#include "Board.h"

Board::Board() : boardSize(100) {}

Board::Board(int size) : boardSize(size)
{
    if (size <= 0)
        throw std::invalid_argument("Board size must be positive");
}

void Board::addSnake(const Snake &snake)
{
    if (snake.getHead() > boardSize && snake.getTails() > boardSize)
        throw std::invalid_argument("Snake positions must lie inside the board");

    // check for overlaps
    if (snakesMap.find(snake.getHead()) != snakesMap.end())
    {
        throw std::invalid_argument("A snake already exist at position: " + std::to_string(snake.getHead()));
    }
    if (laddersMap.find(snake.getHead()) != laddersMap.end())
    {
        throw std::invalid_argument("A ladder already exist at position: " + std::to_string(snake.getHead()));
    }

    // snakesMap[snake.getHead()] = snake;
    snakesMap.emplace(snake.getHead(), snake);
}

void Board::addLadder(const Ladder &ladder)
{
    if (ladder.getEnd() > boardSize && ladder.getStart() > boardSize)
        throw std::invalid_argument("Ladder positions must lie inside the board");

    // check for overlaps
    if (snakesMap.find(ladder.getStart()) != snakesMap.end())
    {
        throw std::invalid_argument("A snake already exist at position: " + std::to_string(ladder.getStart()));
    }
    if (laddersMap.find(ladder.getStart()) != laddersMap.end())
    {
        throw std::invalid_argument("A ladder already exist at position: " + std::to_string(ladder.getStart()));
    }

    // laddersMap[ladder.getStart()] = ladder;
    laddersMap.emplace(ladder.getStart(), ladder);
}

int Board::getNewPositionAfterSnakeOrLadder(int position) const
{
    // check for snake
    auto hasSnake = snakesMap.find(position);
    if (hasSnake != snakesMap.end())
    {
        return hasSnake->second.getTails();
    }

    // check for ladder
    auto hasLadder = laddersMap.find(position);
    if (hasLadder != laddersMap.end())
    {
        return hasLadder->second.getEnd();
    }

    return position;
}

int Board::getSize() const { return boardSize; }

const std::vector<Snake> Board::getSnakes() const
{
    std::vector<Snake> snakes;
    for (auto &snake : snakesMap)
    {
        snakes.push_back(snake.second);
    }

    return snakes;
}

const std::vector<Ladder> Board::getLadders() const
{
    std::vector<Ladder> ladders;
    for (auto &ladder : laddersMap)
    {
        ladders.push_back(ladder.second);
    }

    return ladders;
}
