#pragma once
#include "ui/base/IUIManager.h"

class TextRenderer;

class MainMenuUIManager : public IUIManager {
public: 

    
    MainMenuUIManager(
        SceneEventCallback eventCallback
    );
    ~MainMenuUIManager();

    void init() override;

    const UIRenderData& getUIRenderData() override;
    
    void CollectRenderData() override;

    

    void handleClick(int logicalX, int logicalY);
    
private:
    

    
};