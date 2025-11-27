#pragma once
#include "Piece.h"
#include "ComponentManager.h"
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
    //使用一维数组表示棋盘
    std::vector<std::unique_ptr<Piece>> m_grid;
    std::unique_ptr<ComponentManager> m_component;
    const int m_rows;
    const int m_cols;
public:
    Board(int rows, int cols);
    ~Board();
    const int getIndex(int row, int col);
    std::unique_ptr<Piece>& at(int row, int col);
    bool initialize();
};