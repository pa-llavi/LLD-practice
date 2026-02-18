#ifndef GAME_H
#define GAME_H

#include <vector>
#include <queue>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "Dice.h"

enum class GameStatus
{
    NOT_STARTED,
    IN_PROGRESS,
    FINISHED
};

class Game
{
private:
    Board board;
    Dice dice;
    std::vector<std::shared_ptr<Player>> players;
    std::queue<std::shared_ptr<Player>> playerQueue;
    GameStatus status;
    std::shared_ptr<Player> winner;

    void initPlayerQueue();
    bool processTurn(std::shared_ptr<Player> player, int diceRoll);
    bool hasPlayerWon(std::shared_ptr<Player> player) const;

public:
    Game();
    Game(const Board &board);

    void addPlayer(const std::string &playerName);
    void startGame();
    string playTurn();
    void playGame();

    GameStatus getStatus() const;
    std::shared_ptr<Player> getWinner() const;
    const vector<shared_ptr<Player>> &getPlayers() const;
    Board &getBoard() const;
};

#endif