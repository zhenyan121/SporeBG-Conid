#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <memory>

class ComponentManager {
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
    int m_selectedComponentID = -1;
    // 总棋子数
    int m_maxPossiblePieces;

    // 重新计算连通性（断开后调用）
    void recomputeComponentsAfterDisconnection(int oldComponentID, const std::unordered_set<int>& remainingPieces);
    // 处理组件分裂
    void handleComponentSplit(int oldComponentID, const std::unordered_set<int>& affectedPieces);

    // 使用BFS查找连通区域
    std::unordered_set<int> bfsConnectedRegion(int startPiece, const std::unordered_set<int>& afftedPieces);

    //  创建新组件
    int createNewComponent(int rootPiece);

public:
    // 构造函数：初始化指定数量的棋子
    explicit ComponentManager(int maxPossiblePieces);
    //添加棋子
    void addPiece(int PieceID, const std::vector<int>& adjacentPiece);
    //移除棋子
    void removePiece(int PieceID);
    // 查找操作（带路径压缩）
    int find(int pieceID);
    // 合并两个棋子所在的组件（按秩合并）
    void unite(int pieceID1, int pieceID2);
    // 连接两个组件
    void addConnection(int pieceID1, int pieceID2);
    // 断开棋子与指定邻居的连接
    bool disconnectFromNeighbor(int pieceID, int neighborID);
    // 将棋子完全从当前组件断开，成为独立组件
    bool disconnectFromComponent(int pieceID);
    // 通过棋子选择整个连通片
    void selectComponentByPiece(int pieceID);
    // 获取当前选中的组件中的所有棋子
    const std::unordered_set<int>& getSelectedComponent() const;
    // 获取棋子所属的组件ID
    int getComponentID(int pieceID) const;
    // 获取组件内所有棋子
    const std::unordered_set<int>& getPiecesInComponent(int pieceID) const;
    // 检查两个棋子是否在同一个组件中
    bool areConnected(int pieceID1, int pieceID2);
    // 获取棋子的所有直接连接
    const std::unordered_set<int>& getPieceConnections(int pieceID) const;
    // 检查两个棋子是否直接相连
    bool areDirectlyConnected(int pieceID1, int pieceID2) const;
    // 清除选中状态
    void clearSelection();

    //获取所有组件
    std::unordered_map<int, std::unordered_set<int>> getAllComponents() const;
    

};