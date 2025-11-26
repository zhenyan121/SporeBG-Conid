#pragma once
#include "Piece.h"
#include <memory>
#include <vector>
// 用 (row, col) 表示坐标，0-based
struct Position {
    int row;
    int col;
    //运算符重载用于判断位置是否相等
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

class Board {
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> m_grid;
    const int m_rows;
    const int m_cols;
public:
    Board(int rows, int cols);
    ~Board();

};