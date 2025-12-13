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
    m_boardRenderer = std::make_unique<BoardRenderer>(WIDTH, HEIGHT, renderer);
    m_gameSession = std::make_unique<GameSession>();
    m_CoordinateConverter = std::make_unique<CoordinateConverter>(renderer);
    m_gameSession->initialize();

    m_boardRenderer->setBoard(m_gameSession->getBoard());

    
    
    
}

void GameScene::update() {

}

void GameScene::render() {
    beginFrame();
    m_boardRenderer->drawBackground();
    m_boardRenderer->drawBoard();
    
    m_boardRenderer->drawPiece(m_gameSession->getSelectedPiece());
    m_boardRenderer->drawMovementRange();
    m_uiRenderer->renderUI(m_gameUIManager->getUIRenderData());
    endFrame();
}

void GameScene::handleClick(float screenX, float screenY) {
    auto click = m_CoordinateConverter->ScreenToBoard(screenX, screenY, m_boardRenderer->getBoardArea());
        if (click) {
            auto [row, col] = click.value();
            m_gameSession->handleCoordinateInput(row, col);
            m_gameSession->printBoard();
            m_gameUIManager->updateActionType( m_gameSession->getCurrentActionType());
            m_boardRenderer->updateMovementRange(m_gameSession->getSelectedPiece(), m_gameSession->getCurrentActionType());
        } else {
            SDL_Log("invail cilck aera!");
        }
}

void GameScene::renderMousePosition(float x, float y) {
    m_gameUIManager->UpdateMousePositon(x, y);
}