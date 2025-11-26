#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <memory>

class ConnectedComponentManager {
private:
    // 并查集的父节点数组，使用连续内存存储
    std::vector<int> m_parent;
    //记录根节点的大小（按秩合并）
    std::vector<int> m_rank;
    // 记录每个棋子的直接连接关系（邻接表）
    std::vector<std::unordered_set<int>> m_adjacentList;
    // 组件ID到棋子集合的映射
    std::unordered_map<int, std::unordered_set<int>> m_componentPieces;
    // 棋子ID到组件ID的映射
    std::unordered_map<int, int> m_pieceToComponent;
    // 当前选中的组件ID
    int m_slelectedComponentId = -1;
    // 总棋子数
    int m_totalPieces;

    // 重新计算连通性（断开后调用）
    void recomputeComponentsAfterDisconnection(int disconnectedPiece);
    // 处理组件分裂
    void handleComponentSplit(int oldComponentId, const std::unordered_set<int>& affectedPieces);

    // 使用BFS查找连通区域
    std::unordered_set<int> bfsConnectedRegion(int startPiece, const std::unordered_set<int>& afftedPieces);

    //  创建新组件
    int createNewComponent(int rootPiece);

public:
    // 构造函数：初始化指定数量的棋子
    explicit ConnectedComponentManager(int numPieces);
    // 查找操作（带路径压缩）
    int find(int pieceId);
    // 合并两个棋子所在的组件（按秩合并）
    void unite(int pieceId1, int pieceId2);
    // 连接两个组件
    void addConnection(int pieceId1, int pieceId2);
    // 断开棋子与指定邻居的连接
    bool disconnectFromNeighbor(int pieceId, int neighborId);
    // 将棋子完全从当前组件断开，成为独立组件
    bool disconnectFromComponent(int pieceId);
    // 通过棋子选择整个连通片
    void selectComponentByPiece(int pieceId);
    // 获取当前选中的组件中的所有棋子
    const std::unordered_set<int>& getSelectedComponent() const;
    // 获取棋子所属的组件ID
    int getComponentId(int pieceId) const;
    // 获取组件内所有棋子
    const std::unordered_set<int>& getPiecesInComponent(int pieceId) const;
    // 检查两个棋子是否在同一个组件中
    bool areConnected(int pieceId1, int pieceId2);
    // 获取棋子的所有直接连接
    const std::unordered_set<int>& getPieceConnections(int pieceId) const;
    // 检查两个棋子是否直接相连
    bool areDirectlyConnected(int pieceId1, int pieceId2) const;
    // 清除选中状态
    void clearSelection();

    //获取所有组件
    std::unordered_map<int, std::unordered_set<int>> getAllComponents() const;
    

};