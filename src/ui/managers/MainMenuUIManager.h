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

    void updateMousePositon(int logicalX, int logicalY) override;

    void handleClick(int logicalX, int logicalY);
    
private:
    SceneEventCallback m_eventCallback;

    
};