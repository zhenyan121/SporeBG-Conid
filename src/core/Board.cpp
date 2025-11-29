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
int Board::getPieceID(int row, int col) const {
    if (row < 0 || row >= m_rows || col < 0 || col >= m_cols) {
        std::cout << "PieceID over!!" << std::endl;
        exit(EXIT_FAILURE);
    }
    return row * m_cols + col;
}

std::pair<int, int> Board::getCoordFromID(int PieceID) const {
    int row = PieceID / m_cols;
    int col = PieceID % m_cols;
    return {row, col};
}

const Piece* Board::getPieceAt(int row, int col) const {
    int PieceID = getPieceID(row, col);
    return m_grid[PieceID].get();
}

void Board::placePieceAt(int row, int col, PlayerID ID) {
    int PieceID = getPieceID(row, col);
    if (!Rule::canPlacePiece(m_grid[PieceID].get())) {
        std::cout << "can't place piece at " << row << ", " << col << "\n";
        return;
    }
    m_grid[PieceID] = std::make_unique<Piece>(ID);
    //创建方向数组
    const int dx[] = {0, 0, -1, 1};
    const int dy[] = {-1, 1, 0, 0};
    
    std::vector<int> existingNeighbors;  //记录附近的同色邻居

    for (int i = 0; i < 4; i++) {
        int nx = row + dx[i];
        int ny = col + dy[i];
        if (nx < 0 || nx >= m_rows || ny < 0 || ny >= m_cols) {
            continue;
        }
        int pos = getPieceID(nx, ny);
        //添加邻居
        if (Rule::canConnect(m_grid[PieceID].get(), m_grid[pos].get())) {
            existingNeighbors.push_back(pos);

        } 
    }
    // 调用componet处理一片的添加和连接操作
    m_component->addPiece(PieceID, existingNeighbors);
    std::cout << "Place piece at" << row << " " << col << "\n";

}

void Board::removePieceAt(int row, int col) {
    
    int PieceID = getPieceID(row, col);
    if (m_grid[PieceID] == nullptr) {
        return;
    }
    m_grid[PieceID].reset();
    
    m_component->removePiece(PieceID);
    std::cout << "Remove piece at" << row << " " << col << "\n";
}

bool Board::initialize() {
    placePieceAt(0, 0, PlayerID::P1);
    placePieceAt(0 ,2, PlayerID::P1);
    placePieceAt(6, 6, PlayerID::P2);
    placePieceAt(5, 5, PlayerID::P2);
    return true;
}



void Board::printBoard() {
    //removePieceAt(0, 0);
    std::vector<std::string> gameBoard(m_rows);
    for (int i = 0; i < m_rows * m_cols; i++) {
        if (m_grid[i] == nullptr) {
            gameBoard[i / m_cols] += "0";
        } else if (m_grid[i]->getPieceOwner() == PlayerID::P1) {
            gameBoard[i / m_cols] += "1";
        } else {
            gameBoard[i / m_cols] += "2";
        }
    }
    for (std::string& s : gameBoard) {
        std::cout << s << std::endl;
    }
}

std::unordered_set<int> Board::getAllPlayerComponent(PlayerID ID) const {
    std::unordered_set<int> PlayerComponentIDs;
    auto allComponents = m_component->getAllComponents();
    //因为组件没有区分玩家，所以先要获取所有的组件判断组件是否是玩家的在添加进去
    for (const auto& [componentID, pieceSet] : allComponents) {

        if (pieceSet.empty()) continue;

        // 取组件中任意一个棋子（比如第一个）
        int somePieceID = *pieceSet.begin();

        // 安全检查：确保该位置有棋子
        if (m_grid[somePieceID] != nullptr && 
            m_grid[somePieceID]->getPieceOwner() == ID) {
            PlayerComponentIDs.insert(componentID);
        }
    }
    return PlayerComponentIDs;
}

int Board::getComponentID(int row, int col) const{
    int pieceID = getPieceID(row, col);
    int componentID = m_component->getComponentID(pieceID);
    if (pieceID == -1) {
        std::cout << "can't find component id\n";
    }
    return componentID;
}

std::unordered_set<int> Board::getComponentByPieceID(int PieceID) const {
    m_component->selectComponentByPiece(PieceID);
    auto SelectedComponent =  m_component->getSelectedComponent();
    return SelectedComponent;
}

std::unordered_set<int> Board::getOrthogonalNeighbors(int PieceID) const {
    auto [row, col] = getCoordFromID(PieceID);
    std::unordered_set<int> neighbors;

     // 上 (row-1, col)
    if (row > 0) neighbors.insert(getPieceID(row - 1, col));
    // 下 (row+1, col)
    if (row < m_rows - 1) neighbors.insert(getPieceID(row + 1, col));
    // 左 (row, col-1)
    if (col > 0) neighbors.insert(getPieceID(row, col - 1));
    // 右 (row, col+1)
    if (col < m_cols - 1) neighbors.insert(getPieceID(row, col + 1));

    return neighbors;

}

std::unordered_set<int> Board::getSporeRange(int PieceID) const {
    auto [row, col] = getCoordFromID(PieceID);
    std::unordered_set<int> SporeRegion;
    const std::vector<std::pair<int, int>> dir = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1},
        {2, 0}, {-2, 0}, {0, 2}, {0, -2}
    }; 
    
        for (auto [dx, dy] : dir) {
            int nx = row + dx;
            int ny = col + dy;
            if (nx < 0 || nx >= m_rows || ny < 0 || ny >= m_cols) {
                continue;
            }
            
            SporeRegion.insert(getPieceID(nx, ny));
        }
    

    return SporeRegion;
}