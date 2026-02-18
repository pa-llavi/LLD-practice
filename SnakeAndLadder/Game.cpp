#include "Game.h"
#include <iostream>
#include <thread>

Game::Game() : board(), dice(), status(GameStatus::NOT_STARTED), winner(nullptr) {}

Game::Game(const Board &board) : board(board), dice(), status(GameStatus::NOT_STARTED), winner(nullptr) {}

void Game::addPlayer(const string &playerName)
{
    if (status != GameStatus::NOT_STARTED)
        throw runtime_error("Cannot add player after the game has started");

    auto player = make_shared<Player>(playerName);
    players.push_back(player);
}

void Game::startGame()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (players.size() < 2)
        throw runtime_error("Need atleast 2 players to start the game");

    initPlayerQueue();

    status = GameStatus::IN_PROGRESS;
    cout << "Game started with " << players.size() << " players\n";
    cout << "Board size: " << board.getSize() << endl;
    cout << "Number of snakes: " << board.getSnakes().size() << endl;
    cout << "Number of ladders: " << board.getLadders().size() << "\n\n";
}

void Game::initPlayerQueue()
{
    // clear queue and add all players
    while (!playerQueue.empty())
        playerQueue.pop();

    for (const auto &player : players)
    {
        playerQueue.push(player);
    }
}

std::string Game::playTurn()
{
    if (status != GameStatus::IN_PROGRESS)
        throw runtime_error("Game is not in progress");

    auto curPlayer = playerQueue.front();
    playerQueue.pop();

    int diceRoll = dice.roll();

    string msg = curPlayer->getName() + " rolled a " + to_string(diceRoll);

    int curPosition = curPlayer->getPosition();
    int newPosition = curPosition + diceRoll;

    // check if move is valid
    if (newPosition > board.getSize())
    {
        msg += " but needs exact roll to win. Stays at " + to_string(curPosition);
        playerQueue.push(curPlayer);
        return msg;
    }

    curPlayer->setPosition(newPosition);
    msg += " and moved from " + (to_string)(curPosition) + " to " + to_string(newPosition);

    // check for snake or ladder
    int finalPostion = board.getNewPositionAfterSnakeOrLadder(newPosition);
    if (finalPostion != newPosition)
    {
        curPlayer->setPosition(finalPostion);
        if (finalPostion < newPosition)
        {
            msg += " but got bitten by a snake! Moved down to " + to_string(finalPostion);
        }
        else
        {
            msg += " and climbed a ladder! Moved up to " + to_string(finalPostion);
        }
    }

    // check if the player has won
    if (hasPlayerWon(curPlayer))
    {
        status = GameStatus::FINISHED;
        winner = curPlayer;
        msg += " \n" + curPlayer->getName() + " has WON the game!";
    }
    else
    {
        playerQueue.push(curPlayer);
    }

    return msg;
}

bool Game::hasPlayerWon(shared_ptr<Player> player) const
{
    if (player->getPosition() == board.getSize())
        return true;
    return false;
}

void Game::playGame()
{
    if (status != GameStatus::IN_PROGRESS)
        throw runtime_error("Game must be started before playing");

    int turncount = 0;
    const int MAX_TURNS = 1000; // safety limit to prevent infinite turns;

    while (status == GameStatus::IN_PROGRESS && turncount < MAX_TURNS)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        string turnResult = playTurn();
        cout << turnResult << "\n\n";
        turncount++;
    }

    if (turncount > MAX_TURNS)
    {
        cout << "Game reached maximum turn limits!\n";
    }
}

GameStatus Game::getStatus() const
{
    return status;
}

shared_ptr<Player> Game::getWinner() const
{
    return winner;
}

const vector<shared_ptr<Player>> &Game::getPlayers() const
{
    return players;
}