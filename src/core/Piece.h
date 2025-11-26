#pragma once

enum class PlayerColer {
    WHITE,
    BLACK
};


class Piece {
private:
    PlayerColer m_color;
public:
    Piece(PlayerColer color);
    ~Piece();
    PlayerColer getPlayerColor() const;
        
    
};