#include "GameScene.h"

GameScene::GameScene() {
    
}

GameScene::~GameScene() {

}

void GameScene::onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT) {
    m_renderer = std::make_unique<GameRenderer>(WIDTH, HEIGHT, renderer);
    m_gameSession = std::make_unique<GameSession>();
    m_CoordinateConverter = std::make_unique<CoordinateConverter>(renderer);
    m_gameSession->initialize();
}

void GameScene::update() {

}

void GameScene::render() {
    m_renderer->beginFrame();
    m_renderer->drawBackground();
    m_renderer->drawBoard();
    m_renderer->endFrame();
}

void GameScene::handleClick(float screenX, float screenY) {
    auto click = m_CoordinateConverter->ScreenToBoard(screenX, screenY, m_renderer->getBoardArea());
        if (click) {
            auto [row, col] = click.value();
            m_gameSession->handleCoordinateInput(row, col);
            m_gameSession->printBoard();
        } else {
            SDL_Log("invail cilck aera!");
        }
}
