#include "ConnectedComponentManager.h"

ConnectedComponentManager::ConnectedComponentManager(int numPieces)
    : m_totalPieces(numPieces) {
    //初始化数组大小 ROWS * COLS
    m_parent.resize(numPieces);
    m_rank.resize(numPieces, 0);
    m_adjacentList.resize(numPieces);
    //初始化每个棋子为独立组件
    for (int i = 0; i < numPieces; i++) {
        m_parent[i] = i;
        m_componentPieces[i] = {i}; //每个组件初始包含一个棋子
        m_pieceToComponent[i] = i; //棋子指向的组件
    }
}

int ConnectedComponentManager::find(int pieceId) {
    if (m_parent[pieceId] != pieceId) {
        m_parent[pieceId] = find(m_parent[pieceId]);
    }
    return m_parent[pieceId];
}

void ConnectedComponentManager::unite(int pieceId1, int pieceId2) {
    int root1 = find(pieceId1);
    int root2 = find(pieceId2);

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
    addConnection(pieceId1, pieceId2);
}

void ConnectedComponentManager::addConnection(int pieceId1, int pieceId2) {
    //将元素放入邻接表
    m_adjacentList[pieceId1].insert(pieceId2);
    m_adjacentList[pieceId2].insert(pieceId1);

}

bool ConnectedComponentManager::disconnectFromNeighbor(int pieceId, int neighborId){
    // 检查是否真的相连
    if (!areDirectlyConnected(pieceId, neighborId)) {
        return false;
    }
    // 从邻接表中移除连接
    m_adjacentList[pieceId].erase(neighborId);
    m_adjacentList[neighborId].erase(pieceId);
    // 重新计算连通性
    recomputeComponentsAfterDisconnection(pieceId);

    return true;
}

bool ConnectedComponentManager::disconnectFromComponent(int pieceId) {
    int oldComponentId = find(pieceId);
    if (oldComponentId == -1) return false;

    // 记录所有直接连接
    auto neighbors = m_adjacentList[pieceId];

    // 断开所有连接
    for (int neighborId : neighbors) {
    m_adjacentList[pieceId].erase(neighborId);
    m_adjacentList[neighborId].erase(pieceId);
    }

    // 将被断开的棋子设为独立组件
    m_parent[pieceId] = pieceId;
    m_rank[pieceId] = 0;
    m_componentPieces[pieceId] = {pieceId};
    m_pieceToComponent[pieceId] = pieceId;

    // 如果原组件还有其他棋子，需要重新计算连通性
    if (!neighbors.empty()) {
    recomputeComponentsAfterDisconnection(pieceId);
    }
    return true;
}

void ConnectedComponentManager::recomputeComponentsAfterDisconnection(int disconnectedPiece) {
    int oldComponentId = find(disconnectedPiece);
    if (oldComponentId == -1 || m_componentPieces[disconnectedPiece].size() <= 1) {
        return;
    }

    // 获取原组件中除断开棋子外的所有棋子
    std::unordered_set<int> remainingPieces = m_componentPieces[oldComponentId];
    remainingPieces.erase(disconnectedPiece);

    //处理组件分裂
    handleComponentSplit(oldComponentId, remainingPieces);
}

void ConnectedComponentManager::handleComponentSplit(int oldComponentId, const std:: unordered_set<int>& remainingPieces ) {
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
    m_componentPieces.erase(oldComponentId);

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


std::unordered_set<int> ConnectedComponentManager::bfsConnectedRegion(int startPiece, const std::unordered_set<int>& availablepieces) {
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

int ConnectedComponentManager::createNewComponent(int rootPiece) {
    m_parent[rootPiece] = rootPiece;
    m_rank[rootPiece] = 0;
    m_componentPieces[rootPiece] = {rootPiece};
    m_pieceToComponent[rootPiece] = rootPiece;
    return rootPiece;
}

void ConnectedComponentManager::selectComponentByPiece(int pieceId) {
    m_selectedComponentId = find(pieceId);
}

const std::unordered_set<int>& ConnectedComponentManager::getSelectedComponent() const {
    static std::unordered_set<int> emptySet;
    if (m_selectedComponentId == -1 ||
        m_componentPieces.find(m_selectedComponentId) == m_componentPieces.end()) {

            return emptySet;
        }
    return m_componentPieces.at(m_selectedComponentId);
}

int ConnectedComponentManager::getComponentId(int pieceId) const {
    auto it = m_pieceToComponent.find(pieceId);
    return (it != m_pieceToComponent.end()) ? it->second : -1;
}

const std::unordered_set<int>& ConnectedComponentManager::getPiecesInComponent(int componentId) const {
    static std::unordered_set<int> emptySet;
    auto it = m_componentPieces.find(componentId);

    return (it != m_componentPieces.end()) ? it->second : emptySet;
}

bool ConnectedComponentManager::areConnected(int pieceId1, int pieceId2) {
    return find(pieceId1) == find(pieceId2);
}

const std::unordered_set<int>& ConnectedComponentManager::getPieceConnections(int pieceId) const {
    static std::unordered_set<int> emptySet;
    if (pieceId < 0 || pieceId >= m_totalPieces) return emptySet;
    return m_adjacentList[pieceId];
}

bool ConnectedComponentManager::areDirectlyConnected(int pieceId1, int pieceId2) const {
    if (pieceId1 < 0 || pieceId1 >= m_totalPieces || 
        pieceId2 < 0 || pieceId2 >= m_totalPieces) {
        return false;
    }
    return m_adjacentList[pieceId1].find(pieceId2) != m_adjacentList[pieceId1].end();
}

void ConnectedComponentManager::clearSelection() {
    m_selectedComponentId = -1;
}

std:: unordered_map<int, std::unordered_set<int>> ConnectedComponentManager::getAllComponents() const {
    return m_componentPieces;
}




