// src/core/Game.h
#pragma once
#include <optional>
#include <utility>
#include <memory>
#include <functional>
#include "Board.h"




class GameSession {

private:
    using GamePieceEventCallback = std::function<void(GamePieceEvent, int fromRow, int fromCol, int toRow, int toCol)>;
    std::unique_ptr<Board> m_board;
    PlayerID m_currentPlayer = PlayerID::P1;
    ActionType m_currentActionType = ActionType::GROW;
    std::optional<std::pair<int, int>> m_seletedPiece = std::nullopt; //表示“可能有值，也可能没有值”
    std::unordered_set<int> m_actionableComponents;;
    // 保存旧的componentid与pieceid的对应关系
    std::unordered_map<int, int> m_oldPieceIDtoComponentID;
    // 如果操作执行成功就从
    void markComponentAsUsed(int componentID);
    
    GameState m_gameState = GameState::GAME_RUNING;

    GamePieceEventCallback m_gamePieceEventCallback;

public:
    GameSession();
    ~GameSession();
    void cleanup();
    bool initialize();
    //获取当前玩家的拥有的组件
    void resetActionableComponents();
    //旧的componentid与pieceid的对应关系
    void resetOldPieceIDtoComponent();
    // 设置行动类型
    void setPlayerAction(ActionType type);
    // 执行玩家的行动
    bool executeAction(int row, int col);
    // 获取当前玩家
    PlayerID getCurrentPlayer() const;
    // 打印棋盘
    void printBoard() const;
    // 下一个轮次
    void nextTurn(); 

    // 根据当前状态自动处理坐标输入：
    // - 如果尚未选中棋子 → 尝试选中
    // - 如果已选中棋子 → 尝试执行当前动作类型（GROW/MOVE/SPORE）
    bool handleCoordinateInput(int row, int col);

    int getOldComponentID(int row, int col);

    std::optional<std::pair<int, int>> getSelectedPiece() const;

    const Board* getBoard() const;

    ActionType getCurrentActionType() const;

    GameState getGameState() const;

    void setGamePieceEventCallback(GamePieceEventCallback callback) {
        m_gamePieceEventCallback = callback;
    }

};