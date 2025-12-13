#pragma once
#include "ui/base/UIComponent.h"
#include "ui/base/UIRenderData.h"
class Label : public UIComponent {
public:
    Label(const std::string& text, TextStyle style);
    ~Label() override = default;
    Label();
    // 实现UIComponent接口
    
    void update(float deltaTime) override;

    /**
     * @brief 设置按钮文本
     * @param text 按钮文本
     * @param style 文本样式
     */
    void setText(const std::string& text, TextStyle style);

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


    LabelData& getLabelDate() {
        m_labelData.rect = m_rect;
        return m_labelData;
    }

private:

    
            
    
    LabelData m_labelData;

    
};