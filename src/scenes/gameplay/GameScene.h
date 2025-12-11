#pragma once
#include "scenes/base/Scene.h"
#include "graphics/GameRenderer.h"
#include "graphics/CoordinateConverter.h"
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
    void render() override;
    void handleClick(float screenX, float screenY) override;
private:
    
    std::unique_ptr<GameRenderer> m_gameRenderer;
    std::unique_ptr<CoordinateConverter> m_CoordinateConverter;
    std::unique_ptr<GameSession> m_gameSession;
    
    std::unique_ptr<GameUIManager> m_gameUIManager;
    
    
   
};  