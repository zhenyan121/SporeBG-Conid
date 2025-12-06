#pragma once
#include "GameTypes.h"


class Piece {
private:
    PlayerID m_owner;
public:
    Piece(PlayerID ID);
    ~Piece();
    PlayerID getPieceOwner() const;
        
    
};