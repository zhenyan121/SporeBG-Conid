#pragma once
// 这里存储游戏的各种状态

constexpr int ROWS = 7;
constexpr int COLS = 7;

enum class PlayerID {
    P1,
    P2
};

enum class ActionType {
    GROW,
    MOVE,
    SPORE
};

enum class GameState {
    GAME_RUNING,    // 游戏进行中
    GAME_LOSE,  // 游戏失败
    GAME_WIN,   // 游戏胜利
    GAME_PREGAME  // 游戏未开始
};

enum class GameMode {
    LOCAL_PVP,        // 本地两人对战
    VS_AI,            // 玩家 vs AI
    ONLINE_PVP,       // 网络对战
    TUTORIAL_MODE     // 教程（可视为特殊模式）
};

enum class GamePieceEvent {
    REMOVE_PIECE,
    PLACE_PIECE,
    MOVE_PIECE,
    GROW_PIECE,
    SPORE_PIECE,
    FIGHT_PIECE
};

struct PieceInfo {
    bool hasPiece = false;
    int HP = 0;
    int ATK = 0;
};


namespace Stat {
    constexpr int DefaultHP = 20;
    constexpr int DefaultATK = 10;
    constexpr int HealthRegenPerTurn = 2;
}

