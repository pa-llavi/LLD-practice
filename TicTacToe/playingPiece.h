#pragma once

#include <string>
using namespace std;
enum class PieceType {
    EMPTY,
    X,
    O
};

// have this class if player pieces have different properties like in chess
// class PlayingPiece {
//     private:
//     PieceType pieceType;

//     public:
//     PlayingPiece(PieceType pieceType) : pieceType(pieceType) {}
//     string getPieceTypeName() {
//         if(pieceType == PieceType::X) return "X";
//         return "O";
//     }
//     PieceType getPieceType() {
//         return pieceType;
//     }
// };