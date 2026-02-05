#include "Piece.h"

Piece::Piece(PlayerID ID) : m_owner(ID) {
    m_pieceInfo.ATK = 10;
    m_pieceInfo.HP = 20;
}

Piece::~Piece() {

}

PlayerID Piece::getPieceOwner() const {
    return m_owner;
}