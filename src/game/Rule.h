#pragma once
#include "GameTypes.h"
class Piece; // 前向声明
class Board;

class Rule {
public:
    static bool canConnect(const Piece* a, const Piece* b);
    static bool canPlacePiece(const Piece* piecePos);
    static bool canbeSelect(const Piece* PieceID, const PlayerID ID);
    static bool canGrow(const Board* board, const int fromRow, const int fromCol, const int toRow, const int toCol, const PlayerID player);
    static bool canMove(const Board* board, const int fromRow, const int fromCol, const int toRow, const int toCol, const PlayerID player);
    static bool canSpore(const Board* board, const int fromRow, const int fromCol, const int toRow, const int toCol, const PlayerID player);
};