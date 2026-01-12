#pragma once

#include <string>
#include "playingPiece.h"

class Player {
    string name;
    //PlayingPiece piece;
    PieceType piece;

    public:
    Player(string name, PieceType piece) : name(name), piece(piece) {}
    string getPayerName() { return name; }
    PieceType getPieceType() { return piece; }
};