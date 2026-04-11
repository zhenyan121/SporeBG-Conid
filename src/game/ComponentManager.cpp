#include "ComponentManager.h"
#include <iostream>
ComponentManager::ComponentManager(int maxPossiblePieces)
    : m_maxPossiblePieces(maxPossiblePieces) {
    //初始化数组大小 ROWS * COLS
    m_parent.resize(maxPossiblePieces, -1); // -1表示无棋子
    m_rank.resize(maxPossiblePieces, 0);
    m_adjacentList.resize(maxPossiblePieces);
    
}

void ComponentManager::addPiece(int PieceID, const std::vector<int>& adjacentPiece) {
    if (PieceID < 0 || PieceID >= m_maxPossiblePieces) return;
    if (m_parent[PieceID] != -1) return; // 已存在
    
    m_parent[PieceID] = PieceID;
    m_rank[PieceID] = 0;
    m_componentPieces[PieceID] = {PieceID};
    m_pieceToComponent[PieceID] = PieceID;

    for (int neighbor : adjacentPiece) {
        if (neighbor < 0 || neighbor >= m_maxPossiblePieces || m_parent[neighbor] == -1)
            continue; // 邻居无效或不存在
        // ★ 先无条件建立邻接表连接（无论是否已在同一组件）
        addConnection(PieceID, neighbor);
        unite(PieceID, neighbor);
        
    }

}

void ComponentManager::removePiece(int PieceID) {
    std::cout << "try to remove piece " << PieceID << " ";
    // 先检查棋子是否存在
    if (m_parent[PieceID] == -1){
        std::cout << "piece don't extist" <<std::endl;
    }
    // 1. 断开所有连接（这会为棋子创建独立组件）
    
    disconnectFromComponent(PieceID);
}


int ComponentManager::find(int pieceID) {

    std::cout << "DEBUG find: pieceID=" << pieceID;
    // 添加边界检查
    if (pieceID < 0 || pieceID >= m_maxPossiblePieces) {
        std::cout << " -> Invalid range" << std::endl;
        return -1;  // 返回无效值
    }
    if (m_parent[pieceID] == -1) {
        std::cout << " -> No parent (piece not exists)" << std::endl;
        return -1;  // 棋子不存在
    }
    if (m_parent[pieceID] != pieceID) {
        m_parent[pieceID] = find(m_parent[pieceID]);
    }
    return m_parent[pieceID];
}

void ComponentManager::unite(int pieceID1, int pieceID2) {
    int root1 = find(pieceID1);
    int root2 = find(pieceID2);
    if (root1 == -1 || root2 == -1) return; // ★ 加空检查
    if (root1 == root2) return;

    if (m_rank[root1] < m_rank[root2]) {
        m_parent[root1] = root2;
        m_componentPieces[root2].merge(m_componentPieces[root1]);
        m_componentPieces.erase(root1);
        for (int piece : m_componentPieces[root2]) {
            m_pieceToComponent[piece] = root2;
        }
    } else if (m_rank[root1] > m_rank[root2]) {
        m_parent[root2] = root1;
        m_componentPieces[root1].merge(m_componentPieces[root2]);
        m_componentPieces.erase(root2);
        for (int piece : m_componentPieces[root1]) {
            m_pieceToComponent[piece] = root1;
        }
    } else {
        // rank 相等：root2 挂到 root1 下，root1 的 rank +1
        m_parent[root2] = root1;
        m_rank[root1]++;
        m_componentPieces[root1].merge(m_componentPieces[root2]);
        m_componentPieces.erase(root2);
        for (int piece : m_componentPieces[root1]) {
            m_pieceToComponent[piece] = root1;
        }
    }
    
}

void ComponentManager::addConnection(int pieceID1, int pieceID2) {
    //将元素放入邻接表
    m_adjacentList[pieceID1].insert(pieceID2);
    m_adjacentList[pieceID2].insert(pieceID1);

}

bool ComponentManager::disconnectFromComponent(int pieceID) {
    int oldComponentID = find(pieceID);
    if (oldComponentID == -1) return false;

    // 1. 断开所有邻接表连接
    auto neighbors = m_adjacentList[pieceID];
    for (int neighborID : neighbors) {
        m_adjacentList[neighborID].erase(pieceID);
    }
    m_adjacentList[pieceID].clear();

    // 2. 从组件的棋子集合里移除 pieceID
    std::unordered_set<int> remaining;
    if (m_componentPieces.count(oldComponentID)) {
        m_componentPieces[oldComponentID].erase(pieceID);
        remaining = m_componentPieces[oldComponentID]; // 拷贝！
    }

    // 3. 删除 pieceID 自身的所有数据
    m_parent[pieceID] = -1;
    m_pieceToComponent.erase(pieceID);
    // ★ 注意：不再在这里做根节点迁移，统一交给下面处理

    // 4. 处理剩余棋子
    if (!remaining.empty()) {
        // 先删旧组件（remaining已经是拷贝，安全）
        m_componentPieces.erase(oldComponentID);
        // 重新计算连通性（内部会重建所有组件）
        recomputeComponentsAfterDisconnection(oldComponentID, remaining);
    } else {
        m_componentPieces.erase(oldComponentID);
    }

    return true;
}

void ComponentManager::recomputeComponentsAfterDisconnection(int oldComponentID, const std::unordered_set<int>& remainingPieces) {
    
    // 安全检查
    if (remainingPieces.empty()) {
        
        return;
    }
    //处理组件分裂
    handleComponentSplit(oldComponentID, remainingPieces);
}

void ComponentManager::handleComponentSplit(int oldComponentID, 
                                             const std::unordered_set<int>& remainingPieces) {
    // ★ 关键：先复制，切断对 m_componentPieces 内部数据的引用
    // 因为后续操作会修改 m_componentPieces，导致引用悬空
    std::unordered_set<int> pieces = remainingPieces;

    // Step 1: 先删除旧组件，避免后续 operator[] 和 erase 冲突
    m_componentPieces.erase(oldComponentID);

    // Step 2: 重置所有剩余棋子的 parent，让它们各自独立
    for (int piece : pieces) {
        if (m_parent[piece] == -1) continue;
        m_parent[piece] = piece;
        m_rank[piece] = 0;
        m_componentPieces[piece] = {piece};
        m_pieceToComponent[piece] = piece;
    }

    // Step 3: 按邻接表重新 unite，让连通的自然合并
    for (int piece : pieces) {
        if (m_parent[piece] == -1) continue;
        for (int neighbor : m_adjacentList[piece]) {
            if (m_parent[neighbor] != -1 && pieces.count(neighbor)) {
                unite(piece, neighbor);
            }
        }
    }
}

int ComponentManager::createNewComponent(int rootPiece) {
    m_parent[rootPiece] = rootPiece;
    m_rank[rootPiece] = 0;
    m_componentPieces[rootPiece] = {rootPiece};
    m_pieceToComponent[rootPiece] = rootPiece;
    return rootPiece;
}

void ComponentManager::selectComponentByPiece(int pieceID) {
    m_selectedComponentID = find(pieceID);
    std::cout << "DEBUG selectedComponentID is" << m_selectedComponentID << std::endl;
}

const std::unordered_set<int>& ComponentManager::getSelectedComponent() const {
    static std::unordered_set<int> emptySet;
    if (m_selectedComponentID == -1 ||
        m_componentPieces.find(m_selectedComponentID) == m_componentPieces.end()) {

            return emptySet;
        }
    return m_componentPieces.at(m_selectedComponentID);
}

int ComponentManager::getComponentID(int pieceID) const {
    auto it = m_pieceToComponent.find(pieceID);
    return (it != m_pieceToComponent.end()) ? it->second : -1;
}

const std::unordered_set<int>& ComponentManager::getPiecesInComponent(int componentID) const {
    static std::unordered_set<int> emptySet;
    auto it = m_componentPieces.find(componentID);

    return (it != m_componentPieces.end()) ? it->second : emptySet;
}

bool ComponentManager::areConnected(int pieceID1, int pieceID2) {
    int root1 = find(pieceID1); 
    int root2 = find(pieceID2);
    if (root1 == -1 || root2 == -1) {
        return false;
    }
    return root1 == root2;
}

const std::unordered_set<int>& ComponentManager::getPieceConnections(int pieceID) const {
    static std::unordered_set<int> emptySet;
    if (pieceID < 0 || pieceID >= m_maxPossiblePieces) return emptySet;
    return m_adjacentList[pieceID];
}

bool ComponentManager::areDirectlyConnected(int pieceID1, int pieceID2) const {
    if (pieceID1 < 0 || pieceID1 >= m_maxPossiblePieces || 
        pieceID2 < 0 || pieceID2 >= m_maxPossiblePieces) {
        return false;
    }
    return m_adjacentList[pieceID1].find(pieceID2) != m_adjacentList[pieceID1].end();
}

void ComponentManager::clearSelection() {
    std::cout << "DEBUG clear selected componentID is" << m_selectedComponentID << std::endl;
    m_selectedComponentID = -1;
}

std:: unordered_map<int, std::unordered_set<int>> ComponentManager::getAllComponents() const {
    return m_componentPieces;
    
}

std::unordered_map<int, int> ComponentManager::getAllPiecetoComponent() const {
    return m_pieceToComponent;
}




