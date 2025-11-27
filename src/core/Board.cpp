#include "Board.h"

Board::Board(int rows, int cols) : 
    m_rows(rows),
    m_cols(cols)
{
    m_grid.resize(m_rows * m_cols);
    m_component = std::make_unique<ComponentManager>(m_rows * m_cols);
}    

Board::~Board() {

}
//将二维坐标转化成一维坐标
const int Board::getIndex(int row, int col) {
    return row * m_cols + col;
}

void Board::placePieceAt(int row, int col, PlayerID ID) {
    int index = getIndex(row, col);
    m_grid[index] = std::make_unique<Piece>(ID);
    //创建方向数组
    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};
    std::vector<int> existingNeighbors;  //记录附近的同色邻居

    for (int i = 0; i < 4; i++) {
        int nx = row + dx[i];
        int ny = col + dy[i];
        int pos = getIndex(nx, ny);
        //添加邻居
        if (m_grid[pos] != nullptr && Rule::canConnect(m_grid[index]->getPieceOwner(), m_grid[pos]->getPieceOwner())) {
            existingNeighbors.push_back(pos);

        } 
    }
    // 调用componet处理一片的添加和连接操作
    m_component->addPiece(index, existingNeighbors);

}

bool Board::initialize() {
    placePieceAt(0, 0, PlayerID::P1);
    placePieceAt(0 ,2, PlayerID::P1);
    placePieceAt(6, 6, PlayerID::P2);
    placePieceAt(5, 5, PlayerID::P2);
}


std::unique_ptr<Piece>& Board::at(int row, int col) {
    return m_grid[row * m_cols + col];
}