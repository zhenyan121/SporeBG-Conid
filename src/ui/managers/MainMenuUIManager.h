#pragma once
#include "ui/base/IUIManager.h"

class TextRenderer;

class MainMenuUIManager : public IUIManager {
public: 

    using SceneEventCallback = std::function<void(const std::string&)>;
    MainMenuUIManager(SDL_Renderer* renderer,
        TextRenderer* textRenderer,
        SceneEventCallback eventCallback
    );
    ~MainMenuUIManager();

    void init() override;

    const UIRenderData& getUIRenderData() override;
    
    void CollectRenderData() override;

    void UpdateMousePositon(float x, float y) override;

    void handleClick(float x, float y);
    
private:
    SceneEventCallback m_eventCallback;

    
};