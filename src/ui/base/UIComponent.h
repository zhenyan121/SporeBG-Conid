#pragma once
#include <SDL3/SDL.h>
#include <string>
/**
 * @brief UI组件抽象基类
 * 
 * 所有UI组件（按钮、标签、面板等）都应继承此类
 * 提供统一的接口用于渲染、更新和事件处理
 */



class UIComponent {
public:
    virtual ~UIComponent() = default;
    
    
    
    /**
     * @brief 更新组件状态
     * @param deltaTime 距离上一帧的时间（秒），用于动画
     */

    virtual void update(float deltaTime) = 0;

    /**
     * @brief 处理SDL事件
     * @param event SDL事件引用
     */

    //virtual void handleEvent

    /**
     * @brief 设置组件位置
     * @param x X坐标（屏幕像素）
     * @param y Y坐标（屏幕像素）
     * @param w 宽度（屏幕像素）
     * @param h 高度（屏幕像素）
     */

    virtual void setRect(int x, int y, int w, int h) {
        m_rect.w = static_cast<float>(w); 
        m_rect.h = static_cast<float>(h);
        m_rect.x = static_cast<float>(x); 
        m_rect.y = static_cast<float>(y); 
    }

    /**
     * @brief 获取组件边界矩形
     * @return SDL_FRect 浮点矩形
     */
    SDL_FRect getBounds() const { return m_rect; }

    /**
     * @brief 设置是否可见
     * @param visible 可见性标志
     */
    void setVisible(bool visible) { m_isVisible = visible; }
    
    /**
     * @brief 检查是否可见
     * @return true 如果组件可见
     */
    bool isVisible() const { return m_isVisible; }
    
    /**
     * @brief 设置是否启用交互
     * @param enabled 启用标志
     */
    void setEnabled(bool enabled) { m_isEnabled = enabled; }
    
    /**
     * @brief 检查是否启用
     * @return true 如果组件启用
     */
    bool isEnabled() const { return m_isEnabled; }

    /** 
     * @brief 获取组件名称的哈希值
     * @return 组件名称的哈希值
     */
    size_t getNameHash() const {
        return std::hash<std::string>{}(m_name);
    }

    /**
     * @brief 设置组件名称
     * @param name 组件名称字符串
     */
    void setName(const std::string& name) {
        m_name = name;
    }

protected:
    UIComponent() = default;
    SDL_FRect m_rect = {0.0f, 0.0f, 0.0f, 0.0f};    ///< 组件位置和尺寸
    bool m_isVisible = true;                      ///< 可见性标志
    bool m_isEnabled = true;                      ///< 启用标志
    std::string m_name;                           ///< 组件名称
};