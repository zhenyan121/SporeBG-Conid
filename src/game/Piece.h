#pragma once
#include "GameTypes.h"


class Piece {
private:
    PlayerID m_owner;
    PieceInfo m_pieceInfo;
    
public:
    Piece(PlayerID ID);
    ~Piece();
    PlayerID getPieceOwner() const;
    void changeHP(int num) {
        m_pieceInfo.HP += num;
    }
    void changeATK(int num) {
        m_pieceInfo.ATK += num;
    }
    
    int getHP() const {
        return m_pieceInfo.HP;
    }
    int getATK() const {
        return m_pieceInfo.ATK;
    }
    PieceInfo getPieceInfo() const {
        return m_pieceInfo;
    }

    void setHP(int HP) {
        m_pieceInfo.HP = HP;
    }

    void setATK(int ATK) {
        m_pieceInfo.ATK = ATK;
    }

    void setInfo(PieceInfo pieceInfo) {
        m_pieceInfo = pieceInfo;
    }
    
};