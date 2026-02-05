#pragma once
#include "Piece.h"
#include "ComponentManager.h"
#include "Rule.h"
#include <memory>
#include <vector>
#include <string>
#include <iostream>
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
    // 连接片管理
    std::unique_ptr<ComponentManager> m_component;
    const int m_rows;
    const int m_cols;
public:
    Board(int rows, int cols);
    ~Board();
    // 获取一维索引
    int getPieceID(int row, int col) const;
    // 从一维反推二维
    std::pair<int, int> getCoordFromID(int PieceID) const;
    //获取棋子指针（不可修改）
    const Piece* getPieceAt(int row, int col) const;
    //初始化
    bool initialize();
    // 放置棋子
    void placePieceAt(int row, int col, PlayerID ID);
    //删除棋子
    void removePieceAt(int row, int col);
    //打印棋盘
    void printBoard();
    //获取相同玩家棋子的所有棋子块的根节点
    std::unordered_set<int> getAllPlayerComponent(PlayerID ID) const;
    //获取棋子ID到组件ID的映射
    std::unordered_map<int, int> getALLPiecetoComponent() const;
    // 获取坐标对应的块的ID
    int getComponentID(int row, int col) const;
    // 获取组件ID对应的组件
    std::unordered_set<int> getComponentByPieceID(int PieceID) const;
    // 获取 上下左右四个相邻位置（即 4-邻域）
    std::unordered_set<int> getOrthogonalNeighbors(int PieceID) const;
    // 获取扩散范围
    std::unordered_set<int> getSporeRange(int PieceID) const;

    bool changeHP(int row, int col, int num);

    bool changeATK(int row, int col, int num);

    bool setPieceInfo(int row, int col, PieceInfo pieceInfo);
};