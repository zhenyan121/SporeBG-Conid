#pragma once
#include "ui/base/UIRenderData.h"
#include "ui/base/UIComponent.h"
#include <memory>

// 前向声明，避免在头文件包含过多实现细节
class TextRenderer;
#include <functional>
class Button : public UIComponent{
public:
    // 默认构造（不进行自动测量）
    Button();
    // 可以传入 TextRenderer 指针以便在 setText 时立即计算文字尺寸并更新 rect
    explicit Button(TextRenderer* textRenderer);

    explicit Button(
        const std::string& text,
        TextStyle style = {"SourceHanSansSC-Regular.otf", 48, {0, 0, 0, 255}},
        int x = 0,
        int y = 0,
        TextRenderer* textRenderer = nullptr,
        SDL_Color backgroundColor = {200, 200, 200, 255},
        int borderThickness = 0,
        SDL_Color borderColor = {0, 0, 0, 255}

    );
    ~Button() override = default;

    // 实现UIComponent接口
    
    void update(float deltaTime) override;

    /**
     * @brief 设置按钮文本
     * @param text 按钮文本
     * @param style 文本样式
     */
    void setText(const std::string& text, TextStyle style);

    /**
     * @brief 设置按钮文本（不改变样式）
     * @param text 按钮文本
     */
    void setText(const std::string& text);

     /**
     * @brief 设置背景颜色
     * @param normal 正常状态颜色
     * @param hovered 悬停状态颜色（可选，默认透明）
     * @param pressed 按下状态颜色（可选，默认透明）
     */
    void setBackgroundColor(SDL_Color normal);
    /**
     * @brief 设置边框
     * @param thickness 边框厚度（像素）
     * @param color 边框颜色
     */
    void setBorder(int thickness, SDL_Color color);

    void setCallback(std::function<void()> callback);

    ButtonData& getButtonDate() {
        m_buttonData.rect = m_rect;
        return m_buttonData;
    }

    /**
     * @brief 处理点击事件
     * @param x 点击位置的X坐标
     * @param y 点击位置的Y坐标
     */
    bool handleCilck(int x, int y);

private:

    
            
    std::function<void()> m_callback;
    ButtonData m_buttonData;
    // 用于在 setText 时测量文本尺寸（非拥有）
    TextRenderer* m_textRenderer = nullptr;

    
};
