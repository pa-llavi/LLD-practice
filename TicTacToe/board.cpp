#include "board.h"
#include<iostream>

// Board::Board(int size) : size(size) {
//     board.resize(size, std::vector<PlayingPiece*>(size, nullptr));
// }

bool Board::addPiece(int r, int c, PieceType piece) {
    if (r < 0 || c < 0 || r >= size || c >= size) {
        std::cout << "Invalid cell!\n";
        return false;
    }

    if (board[r][c] != PieceType::EMPTY) {
        std::cout << "Cell already occupied!\n";
        return false;
    }

    board[r][c] = piece;

    // std::cout << "Board after the move:\n";
    // printBoard();
    return true;
}

std::vector<std::pair<int,int>> Board::getFreeCell() {
    vector<pair<int,int>> freeCells;

    for(int i=0; i<size; i++) {
        for(int j=0; j<size; j++) {
            if(board[i][j] == PieceType::EMPTY) {
                freeCells.push_back({i,j});
            }
        }
    }   
    return freeCells; 
}

void Board::printBoard() const {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            char c = board[i][j] == PieceType::X ? 'X' :
                     board[i][j] == PieceType::O ? 'O' : '_';
            cout << c << " | ";
        }
        cout << "\n";
    }
}
