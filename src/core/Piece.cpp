#include "Piece.h"

Piece::Piece(PlayerColer color) : m_color(color) {

}

Piece::~Piece() {

}

PlayerColer Piece::getPlayerColor() const {
    return m_color;
}