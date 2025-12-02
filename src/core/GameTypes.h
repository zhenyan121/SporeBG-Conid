#pragma once
// 这里存储游戏的各种状态



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
    MAIN_MENU,        // 主菜单（开始界面）
    SETTINGS,         // 设置界面（音量、画质等）
    GAME_SETUP,       // 对局前的设置（选择模式、玩家类型等）
    IN_GAME,          // 正在对局中（核心逻辑运行）
    PAUSED,           // 游戏暂停（可返回菜单或继续）
    GAME_OVER,        // 对局结束（显示胜负/平局）
    TUTORIAL,         // 教程模式（可选）
    EXITING           // 退出确认或正在退出
};

enum class GameMode {
    LOCAL_PVP,        // 本地两人对战
    VS_AI,            // 玩家 vs AI
    ONLINE_PVP,       // 网络对战
    TUTORIAL_MODE     // 教程（可视为特殊模式）
};