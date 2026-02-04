#pragma once
#include "GameTypes.h"


class Piece {
private:
    PlayerID m_owner;
    int HP = 20;
    int ATK = 10;
public:
    Piece(PlayerID ID);
    ~Piece();
    PlayerID getPieceOwner() const;
    void changeHP(int num) {
        HP += num;
    }
    void changeATK(int num) {
        ATK += num;
    }
    
    int getHP() const {
        return HP;
    }
    int getATK() const {
        return ATK;
    }
    
};