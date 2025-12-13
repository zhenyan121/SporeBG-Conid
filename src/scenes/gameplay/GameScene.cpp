#include "GameScene.h"

GameScene::GameScene() {
    
}

GameScene::~GameScene() {

}

void GameScene::onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT, UIRenderer* uiRenderer){
    m_renderer = renderer;
    m_uiRenderer = uiRenderer;
    m_gameUIManager = std::make_unique<GameUIManager>(renderer, uiRenderer->getTextRenderer());
    m_gameUIManager->init();
    m_gameRenderer = std::make_unique<GameRenderer>(WIDTH, HEIGHT, renderer);
    m_gameSession = std::make_unique<GameSession>();
    m_CoordinateConverter = std::make_unique<CoordinateConverter>(renderer);
    m_gameSession->initialize();

    m_gameRenderer->setBoard(m_gameSession->getBoard());

    
    
    
}

void GameScene::update() {

}

void GameScene::render() {
    beginFrame();
    m_gameRenderer->drawBackground();
    m_gameRenderer->drawBoard();
    
    m_gameRenderer->drawPiece(m_gameSession->getSelectedPiece());
    m_uiRenderer->renderUI(m_gameUIManager->getUIRenderData());
    endFrame();
}

void GameScene::handleClick(float screenX, float screenY) {
    auto click = m_CoordinateConverter->ScreenToBoard(screenX, screenY, m_gameRenderer->getBoardArea());
        if (click) {
            auto [row, col] = click.value();
            m_gameSession->handleCoordinateInput(row, col);
            m_gameSession->printBoard();
            m_gameUIManager->updateActionType( m_gameSession->getCurrentActionType());
        } else {
            SDL_Log("invail cilck aera!");
        }
}

void GameScene::renderMousePosition(float x, float y) {
    m_gameUIManager->UpdateMousePositon(x, y);
}