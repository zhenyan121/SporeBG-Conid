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

    void onEnter(CoreData* coreData) override;

    void update() override;
    void renderWorld() override; // 逻辑世界
    void renderUI() override;    // 屏幕 UI
    void handleClick(int logicalX, int logicalYY) override;

    

    void restartGame();
protected:
    // 创建UIManager的工厂方法，子类可以重写
    virtual std::unique_ptr<GameUIManager> createUIManager();

    // // 点击处理的钩子方法
    virtual bool preHandleClick(int logicalX, int logicalY);
    virtual void postHandleClick();

    virtual void handleBoardClick(int row, int col);

    void updatePieceInfo();

    // 公共成员，子类可以直接访问
    std::unique_ptr<BoardRenderer> m_boardRenderer;
    std::unique_ptr<CoordinateConverter> m_CoordinateConverter;
    std::unique_ptr<GameSession> m_gameSession;
    std::unique_ptr<GameUIManager> m_gameUIManager;
    
    
   
};  