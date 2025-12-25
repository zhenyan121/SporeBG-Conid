#pragma once
#include "scenes/base/Scene.h"
#include "graphics/game/BoardRenderer.h"
#include "graphics/game/CoordinateConverter.h"
#include "game/GameSession.h"
#include "graphics/ui/UIRenderer.h"
#include "ui/managers/GameUIManager.h"
#include <memory>
class GameScene : public Scene {
public:
    GameScene();
    ~GameScene();

    void onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT, UIRenderer* uiRenderer) override;

    void update() override;
    void renderWorld() override; // 逻辑世界
    void renderUI() override;    // 屏幕 UI
    void handleClick(int logicalX, int logicalYY) override;

    

    void restartGame();
private:
    
    std::unique_ptr<BoardRenderer> m_boardRenderer;
    std::unique_ptr<CoordinateConverter> m_CoordinateConverter;
    std::unique_ptr<GameSession> m_gameSession;
    
    std::unique_ptr<GameUIManager> m_gameUIManager;
    
    
   
};  