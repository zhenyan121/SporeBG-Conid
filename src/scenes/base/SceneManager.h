/**
 * @file    SceneManager.h
 * @brief   场景管理器类，负责场景的推入/弹出与切换
 * @details 场景可以被缓存（按名字）以便重用。
 * @author  zhenyan121
 * @date    2025-12-13
 */
#pragma once
#include "Scene.h"
#include "scenes/gameplay/GameScene.h"
#include "scenes/menu/MainMenuScene.h"
#include <SDL3/SDL.h>
#include <string>
#include <stack>
#include <memory>
#include <unordered_map>
#include <functional>

/**
 * @class   SceneManager
 * @brief   场景管理器
 * @details 负责管理游戏中的不同场景（如菜单、游戏玩法等），支持场景的推入、弹出和切换。
 *          场景可以按名称缓存以便重用，使用工厂函数动态创建场景实例。
 */
class SceneManager {
public:
    /** 
     * @brief   构造函数
     * @param   renderer SDL 渲染器指针，用于传递给场景进行渲染
     * @param   uiRenderer UI 渲染器指针，用于传递给场景进行 UI 渲染
     * @param   window SDL 窗口指针，用于获取窗口尺寸
     */
    SceneManager(SDL_Renderer* renderer, UIRenderer* uiRenderer, SDL_Window* window);
    /** 
     * @brief   析构函数
     * @details 清理场景栈和缓存，释放资源
     */
    ~SceneManager();
    
    /**
     * @brief 初始化场景管理器
     * @return 初始化成功返回 true，否则返回 false
     */
    bool initialize();
    /**
     * @brief 注册所有默认场景工厂
     */
    void registerAllScene();
    /**
     * @brief 创建场景实例并缓存
     * @param sceneName 场景名称
     */
    void createScene(const std::string& sceneName);

    /**
     * @brief 注册场景工厂函数，用于按名字动态创建场景实例
     * @param sceneName 场景名称
     * @param factory 返回新场景的函数，返回类型为 std::shared_ptr<Scene>
     */
    void registerSceneFactory(const std::string& sceneName, std::function<std::shared_ptr<Scene>()> factory);

    /**
     * @brief 注销已注册的场景工厂
     */
    void unregisterSceneFactory(const std::string& sceneName);

    /**
     * @brief   将新场景推入场景栈顶端并作为当前场景
     * @param   sceneName 场景名称，用于缓存查找。若缓存中不存在，则通过工厂函数动态创建
     * @details 前一个当前场景将接收 onExit() 回调并保留在栈中。新场景成为当前场景并接收 onEnter() 回调
     */
    void pushScene(const std::string& sceneName);

    /**
     * @brief   弹出当前场景并恢复前一个场景
     * @details 当前场景将接收 onExit() 回调，然后从栈中移除。如果栈不为空，则弹出的上一个场景成为新的当前场景并接收 onEnter() 回调
     */
    void popScene();

    /**
     * @brief   替换当前场景
     * @param   sceneName 场景名称，用于缓存查找或工厂创建
     * @details 当前场景将接收 onExit() 回调并被替换。若缓存中存在该名称的场景则复用，
     *          否则通过工厂函数创建新场景。新场景成为当前场景并接收 onEnter() 回调
     */
    void changeScene(const std::string& sceneName);

    /**
     * @brief   处理当前场景的点击事件
     * @param   clickOn 点击位置坐标对 {x, y}
     * @details 将点击事件转发给当前场景的 handleClick() 方法进行处理
     */
    void handleClickCurrent(std::pair<int, int> clickOn);

    /**
     * @brief   处理当前场景的鼠标位置事件
     * @param   mousePosition 鼠标位置坐标对 {x, y}
     * @details 将鼠标位置事件转发给当前场景的 renderMousePosition() 方法进行处理
     */
    void handleMousePosition(std::pair<int, int> mousePosition);

    /**
     * @brief   更新当前场景
     * @details 调用当前场景的 update() 方法进行每帧逻辑更新
     */
    void updateCurrent();

    /**
     * @brief   渲染当前场景
     * @details 调用当前场景的 renderWorld() 方法进行每帧渲染
     */
    void renderWorld();

    /**
     * @brief   渲染当前场景的 UI
     * @details 调用当前场景的 renderUI() 方法进行 UI 渲染
     */
    void renderUI();
    /**
     * @brief   处理场景事件的回调函数
     * @param   event 场景事件对象
     */
    void handleSceneEvent(const SceneEvent& event);

private:
    SDL_Renderer* m_renderer;                                                      ///< SDL 渲染器指针
    SDL_Window* m_window;                                                          ///< SDL 窗口指针
    UIRenderer* m_uiRenderer;                                                      ///< UI 渲染器指针 
    std::shared_ptr<Scene> m_scene;                                                ///< 当前场景（使用 shared_ptr 便于在缓存和栈中共享）
    std::stack<std::shared_ptr<Scene>> m_scenes;                                   ///< 场景栈，存储场景层级关系（使用 shared_ptr）    
    std::unordered_map<std::string, std::shared_ptr<Scene>> m_sceneCache;          ///< 场景缓存，按名字缓存场景以便切换时复用
    std::unordered_map<std::string, std::function<std::shared_ptr<Scene>()>> m_sceneFactories; ///< 场景工厂映射，按名字动态创建场景实例

    /**
     * @brief   获取窗口尺寸的辅助方法
     * @return  返回 {宽度, 高度}，如果获取失败则返回默认值 {1600, 900}
     */
    std::pair<int, int> getWindowDimensions() const;
};