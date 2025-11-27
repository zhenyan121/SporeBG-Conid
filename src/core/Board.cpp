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

const int Board::getIndex(int row, int col) {
    return row * m_cols + col;
}

bool Board::initialize() {
    m_grid[getIndex(0, 0)] = std::make_unique<Piece>(PlayerID::P1);
    m_grid[getIndex(0, 2)] = std::make_unique<Piece>(PlayerID::P1);
    m_grid[getIndex(6, 6)] = std::make_unique<Piece>(PlayerID::P2);
    m_grid[getIndex(5, 5)] = std::make_unique<Piece>(PlayerID::P2);
}


std::unique_ptr<Piece>& Board::at(int row, int col) {
    return m_grid[row * m_cols + col];
}