#pragma once

#include <deque>
#include "player.h"
#include "board.h"

class TicTacToeGame {
    deque<Player> players;
    Board gameBoard;

    public:
    TicTacToeGame(int boardSize);
    void initializeGame();
    string startGame();
    bool checkForWinner(int row, int coloumn, PieceType piece);
};