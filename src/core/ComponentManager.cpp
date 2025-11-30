#include "ComponentManager.h"

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
        // 合并连通组件（unite 会自动处理是否已在同一组件）
        unite(PieceID, neighbor);

        
    }

}

void ComponentManager::removePiece(int PieceID) {
    disconnectFromComponent(PieceID);
    m_parent[PieceID] = -1;
    //m_rank[PieceID] = 0; 重复设置了
    //m_componentPieces.erase(PieceID);
    
}


int ComponentManager::find(int pieceID) {
    if (m_parent[pieceID] != pieceID) {
        m_parent[pieceID] = find(m_parent[pieceID]);
    }
    return m_parent[pieceID];
}

void ComponentManager::unite(int pieceID1, int pieceID2) {
    int root1 = find(pieceID1);
    int root2 = find(pieceID2);

    if (root1 == root2) {
        return;

    }
    if (m_rank[root1] < m_rank[root2]) {
        m_parent[root1] = root2;
        // 合并棋子集合
        m_componentPieces[root2].merge(m_componentPieces[root1]);
        m_componentPieces.erase(root1);
        // 更新棋子到组件的映射
        for (int piece : m_componentPieces[root2]) {
            m_pieceToComponent[piece] = root2;
        }
    } else {
        m_parent[root2] = root1;
        m_componentPieces[root1].merge(m_componentPieces[root2]);
        m_componentPieces.erase(root2);
        for (int piece : m_componentPieces[root1]) {
            m_pieceToComponent[piece] = root1;
        }
    }
    addConnection(pieceID1, pieceID2);
}

void ComponentManager::addConnection(int pieceID1, int pieceID2) {
    //将元素放入邻接表
    m_adjacentList[pieceID1].insert(pieceID2);
    m_adjacentList[pieceID2].insert(pieceID1);

}
/*
bool ComponentManager::disconnectFromNeighbor(int pieceID, int neighborID){
    // 检查是否真的相连
    if (!areDirectlyConnected(pieceID, neighborID)) {
        return false;
    }
    // 从邻接表中移除连接
    m_adjacentList[pieceID].erase(neighborID);
    m_adjacentList[neighborID].erase(pieceID);
    // 重新计算连通性
    recomputeComponentsAfterDisconnection(pieceID);

    return true;
}
*/
bool ComponentManager::disconnectFromComponent(int pieceID) {
    int oldComponentID = find(pieceID);
    if (oldComponentID == -1) return false;

    // 记录所有直接连接
    auto neighbors = m_adjacentList[pieceID];

    // 断开所有连接
    for (int neighborID : neighbors) {
    
    m_adjacentList[neighborID].erase(pieceID);
    }
    // 删除pieceID的邻接表
    m_adjacentList[pieceID].clear();
   /* // 将被断开的棋子设为独立组件
    m_parent[pieceID] = pieceID;
    m_rank[pieceID] = 0;
    m_componentPieces[pieceID] = {pieceID};
    m_pieceToComponent[pieceID] = pieceID;
*/
    // 从原组件中移除 pieceID
    if (m_componentPieces.count(oldComponentID)) {
        m_componentPieces[oldComponentID].erase(pieceID);
    }

    m_pieceToComponent.erase(pieceID);
    // 如果原组件还有其他棋子，需要重新计算连通性
    if (!m_componentPieces[oldComponentID].empty()) {
    recomputeComponentsAfterDisconnection(oldComponentID, m_componentPieces[oldComponentID]);
    } else {
        m_componentPieces.erase(oldComponentID);
    }
    return true;
}

void ComponentManager::recomputeComponentsAfterDisconnection(int oldComponentID, const std::unordered_set<int>& remainingPieces) {
    
   

    // 安全检查：remainingPieces 至少要有 2 个棋子才可能分裂
    if (remainingPieces.size() <= 1) {
        // 如果只剩 0 或 1 个棋子，不需要分裂
        // （如果剩 1 个，它自己就是一个新组件；如果为 0，调用方应已处理）
        return;
    }

   
    //处理组件分裂
    handleComponentSplit(oldComponentID, remainingPieces);
}

void ComponentManager::handleComponentSplit(int oldComponentID, const std:: unordered_set<int>& remainingPieces ) {
    std::unordered_set<int> visited;
    std::vector<std::unordered_set<int>> newComponents;
    
    //;// 对剩余棋子进行BFS，找到所有连通区域
    for (int piece : remainingPieces) {
        if (visited.find(piece) != visited.end()) {
            continue;
        }

        // 找到一个新的连通区域
        std::unordered_set<int> connectedRegion = bfsConnectedRegion(piece, remainingPieces);

        // 标记为已访问
        visited.insert(connectedRegion.begin(), connectedRegion.end());
        newComponents.push_back(connectedRegion);
    }
    // 删除原组件
    m_componentPieces.erase(oldComponentID);

    // 为每个新连通区域创建组件
    for (const auto& region : newComponents) {
        if (!region.empty()) {
            int newRoot = *region.begin();
            createNewComponent(newRoot);

            // 设置新组件的父节点和棋子集合
            for (int piece : region) {
                m_parent[piece] = newRoot;
                m_pieceToComponent[piece] = newRoot;
            }
            m_componentPieces[newRoot] = region;
        }
    }
    
}


std::unordered_set<int> ComponentManager::bfsConnectedRegion(int startPiece, const std::unordered_set<int>& availablepieces) {
    std::unordered_set<int> connectedRegion;
    std::queue<int> queue;

    queue.push(startPiece);
    connectedRegion.insert(startPiece);

    while(!queue.empty()) {
        int current = queue.front();
        queue.pop();
        // 遍历当前棋子的所有邻居
        for (int neighbor : m_adjacentList[current]) {
            // 如果邻居在可用棋子中且未被访问
            if (availablepieces.find(neighbor) != availablepieces.end() && 
                connectedRegion.find(neighbor) == connectedRegion.end()) {
                connectedRegion.insert(neighbor);
                queue.push(neighbor);
            }
        }
    }


    return connectedRegion;
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
    return find(pieceID1) == find(pieceID2);
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
    m_selectedComponentID = -1;
}

std:: unordered_map<int, std::unordered_set<int>> ComponentManager::getAllComponents() const {
    return m_componentPieces;
    
}

std::unordered_map<int, int> ComponentManager::getAllPiecetoComponent() const {
    return m_pieceToComponent;
}




