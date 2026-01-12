#pragma once

#include "playingPiece.h"
#include <vector>
using namespace std;

class Board {
    int size; 

public:
    vector<vector<PieceType>> board;
    Board(int n) : size(n), board(n, vector<PieceType>(n, PieceType::EMPTY)) {}
    bool addPiece(int r, int c, PieceType piece);
    vector<pair<int,int>> getFreeCell();
    void printBoard() const;
    int getSize() const { return size; }
};