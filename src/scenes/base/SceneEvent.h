/**
 * @file    SceneEvent.h
 * @brief   定义场景事件类型和结构体
 * @author  zhenyan121
 * @date    2025-12-13
 */
#pragma once
#include <string>
/**
 * @enum    SceneEventType
 * @brief   定义场景事件的类型
 */
enum class SceneEventType {
    ChangeScene,    ///< 切换场景事件
    PushScene,   ///< 推入新场景事件
    PopScene,   ///< 弹出当前场景事件
    QuitGame    ///< 退出游戏事件
};

enum class SceneType {

    MainMenuScene,
    GameScene,
    OnlineGameScene
};

// 自定义哈希器
struct SceneTypeHash {
    size_t operator()(SceneType s) const noexcept {
        return static_cast<size_t>(s);
    }
};


/**
 * @struct  SceneEvent
 * @brief   场景事件结构体，包含事件类型和相关数据
 */
struct SceneEvent {
    SceneEventType type;    ///< 事件类型
    SceneType sceneType;  ///< 只在 ChangeScene/PushScene 时使用
};

