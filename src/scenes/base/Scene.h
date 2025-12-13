/**
 * @file    Scene.h
 * @brief   定义场景基类
 * @author  zhenyan121
 * @date    2025-12-13
 */
#pragma once
#include <SDL3/SDL.h>
#include "graphics/ui/UIRenderer.h"
#include "scenes/base/SceneEvent.h"
#include <functional>
/**
 * @class   Scene
 * @brief   场景基类
 * @details 定义场景的基本接口和生命周期方法，供具体场景类继承和实现。
 */
class Scene {
public:
    
    using EventCallback = std::function<void(const SceneEvent&)>; ///< 场景事件回调函数类型
    /** 
     * @brief   析构函数
     */
    virtual ~Scene() = default;

    /** 
     * @brief   场景进入回调
     * @param   renderer SDL 渲染器指针
     * @param   width 窗口宽度
     * @param   height 窗口高度
     * @param   uiRenderer UI 渲染器指针
     */
    virtual void onEnter(SDL_Renderer* renderer, int width, int height, UIRenderer* uiRenderer){};
    /** 
     * @brief   场景退出
     */
    virtual void onExit(){};
    /** 
     * @brief   每帧更新
     */
    virtual void update() = 0;
    /** 
     * @brief   每帧渲染
     */
    virtual void render() = 0;
    /** 
     * @brief   处理点击事件
     * @param   x 点击位置的 X 坐标
     * @param   y 点击位置的 Y 坐标
     */
    virtual void handleClick(float x, float y) = 0;
    /** 
     * @brief   渲染鼠标位置相关内容
     * @param   x 鼠标位置的 X 坐标
     * @param   y 鼠标位置的 Y 坐标
     */
    virtual void renderMousePosition(float x, float y){};
    /** 
     * @brief   设置场景事件回调函数
     * @param   callback 场景事件回调函数
     */
    void setEventCallback(EventCallback callback) {
        m_eventCallback = std::move(callback);
    }
protected:
    // UIRenderer应该由上层管理
    UIRenderer* m_uiRenderer;   ///< UI 渲染器指针
    SDL_Renderer* m_renderer;   ///< SDL 渲染器指针
    EventCallback m_eventCallback;  ///< 场景事件回调函数

    void beginFrame() {
        if (!m_renderer) {
            SDL_Log("Renderer is null in beginFrame!");
            return;
        }
        // 清屏为白色色背景
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255); 
        SDL_RenderClear(m_renderer);
        //std::cout << "begin frame\n";
    }

    void endFrame() {
        // 提交到屏幕
        //std::cout << "end frame\n";
        SDL_RenderPresent(m_renderer);
    }
};

