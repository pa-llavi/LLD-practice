#include "ticTacToe.h"
#include <iostream>

TicTacToeGame::TicTacToeGame(int boardSize) : gameBoard(boardSize) {}

void TicTacToeGame::initializeGame() {
    //creating 2 players
    //PlayingPiece crossPiece(PieceType::X);
    Player player1("Player1", PieceType::X);

    //PlayingPiece circlePiece(PieceType::O);
    Player player2("Player2", PieceType::O);

    players.push_back(player1);
    players.push_back(player2);
}

string TicTacToeGame::startGame() {
    bool noWinner = true;
    while(noWinner) {
        //remove player whose turn it is and push it back in the end
        Player currentPlayer = players.front();
        players.pop_front();

        //get free spaces from the board
        vector<pair<int,int>> freeSpaces = gameBoard.getFreeCell();
        if(freeSpaces.empty()) {
            noWinner = false;
            cout<<"-------- DRAW! -----------\n";
            gameBoard.printBoard();
            return "Tie";
        }

        //read user input
        cout<<currentPlayer.getPayerName()<<" - Please enter [row, coloumn]: ";
        int row, coloumn;
        cin>>row>>coloumn;

        //place the piece in board
        bool validMove = gameBoard.addPiece(row, coloumn, currentPlayer.getPieceType());
        if(!validMove) {
            cout<<"Try again!\n";
            players.push_front(currentPlayer); // add player back to the queue (in the front)
            continue;
        }
        players.push_back(currentPlayer); //otherwise add player into the back of the queue
        gameBoard.printBoard();

        //check if the current move is winning move or not
        bool isWinner = checkForWinner(row, coloumn, currentPlayer.getPieceType());
        if(isWinner) {
            cout<<"-------- WINNER! -----------\n";
            gameBoard.printBoard();
            return currentPlayer.getPayerName();
        }
    }

    return "Tie";
}

bool TicTacToeGame::checkForWinner(int row, int column, PieceType pieceType) {
    bool rowMatch = true;
        bool columnMatch = true;
        bool diagonalMatch = true;
        bool antiDiagonalMatch = true;

        // Check Row
        for (int i = 0; i < gameBoard.getSize(); i++) {
            if (gameBoard.board[row][i] == PieceType::EMPTY || gameBoard.board[row][i] != pieceType) {
                rowMatch = false;
                break;
            }
        }

        // Check Column
        for (int i = 0; i < gameBoard.getSize(); i++) {
            if (gameBoard.board[i][column] == PieceType::EMPTY || gameBoard.board[i][column] != pieceType) {
                columnMatch = false;
                break;
            }
        }

        // Check Diagonally
        for (int i = 0, j = 0; i < gameBoard.getSize(); i++, j++) {
            if (gameBoard.board[i][j] == PieceType::EMPTY || gameBoard.board[i][j] != pieceType) {
                diagonalMatch = false;
                break;
            }
        }

        // Check Anti-Diagonally
        for (int i = 0, j = gameBoard.getSize() - 1; i < gameBoard.getSize(); i++, j--) {
            if (gameBoard.board[i][j] == PieceType::EMPTY || gameBoard.board[i][j] != pieceType) {
                antiDiagonalMatch = false;
                break;
            }
        }

        return rowMatch || columnMatch || diagonalMatch || antiDiagonalMatch;

}