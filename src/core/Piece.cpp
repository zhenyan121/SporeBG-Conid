#include "Piece.h"

Piece::Piece(PlayerID ID) : m_owner(ID) {

}

Piece::~Piece() {

}

PlayerID Piece::getPieceOwner() const {
    return m_owner;
}