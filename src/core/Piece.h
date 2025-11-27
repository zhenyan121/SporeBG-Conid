#pragma once

enum class PlayerID {
    P1,
    P2
};


class Piece {
private:
    PlayerID m_owner;
public:
    Piece(PlayerID ID);
    ~Piece();
    PlayerID getPieceOwner() const;
        
    
};