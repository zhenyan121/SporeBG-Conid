#include "GameScene.h"

GameScene::GameScene() {
    
}

GameScene::~GameScene() {

}

void GameScene::onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT, UIRenderer* uiRenderer){
    m_renderer = renderer;
    m_uiRenderer = uiRenderer;
    m_gameUIManager = std::make_unique<GameUIManager>(
        [this](const std::string& sceneName) {
            if (m_eventCallback) {
                SceneEvent event{SceneEventType::ChangeScene, sceneName};
                m_eventCallback(event);
            }
        }
    );
    m_gameUIManager->init();
    m_gameUIManager->setCallback([this]() {
        this->restartGame();
    });
    m_boardRenderer = std::make_unique<BoardRenderer>(WIDTH, HEIGHT, renderer);
    m_gameSession = std::make_unique<GameSession>();
    m_CoordinateConverter = std::make_unique<CoordinateConverter>(renderer);
    m_gameSession->initialize();

    m_boardRenderer->setBoard(m_gameSession->getBoard());

    
    
    
}

void GameScene::update() {

}

void GameScene::renderWorld() {
    
    m_boardRenderer->drawBackground();
    m_boardRenderer->drawBoard();
    
    m_boardRenderer->drawPiece(m_gameSession->getSelectedPiece());
    m_boardRenderer->drawMovementRange();
    m_boardRenderer->renderBlackOverlay();
    
    
}

void GameScene::renderUI() {
    m_uiRenderer->renderUI(m_gameUIManager->getUIRenderData());
}

void GameScene::handleClick(int logicalX, int logicalY) {
    if (m_gameUIManager->handleClick(logicalX, logicalY)) {
        return;
    }
    
    if (m_gameSession->getGameState() != GameState::GAME_RUNING) {
        SDL_Log("Game is not running, click ignored.");
        return;
    }
    auto click = m_CoordinateConverter->ScreenToBoard(logicalX, logicalY, m_boardRenderer->getBoardArea());
        if (click) {
            auto [row, col] = click.value();
            m_gameSession->handleCoordinateInput(row, col);
            m_gameSession->printBoard();
            SDL_Log("try to updateActionType\n");
            m_gameUIManager->updateActionType( m_gameSession->getCurrentActionType());
            SDL_Log("tyr to updateMovementRange\n");
            m_boardRenderer->updateMovementRange(m_gameSession->getSelectedPiece(), m_gameSession->getCurrentActionType());
            SDL_Log("tyr to updateGameState\n");
            m_gameUIManager->updateGameState(m_gameSession->getGameState());
            m_boardRenderer->setGameState(m_gameSession->getGameState());
            
        } else {
            SDL_Log("invail cilck aera!");
        }
}


void GameScene::restartGame() {
    m_gameSession = std::make_unique<GameSession>();
    m_gameSession->initialize();
    m_boardRenderer->setBoard(m_gameSession->getBoard());
    m_boardRenderer->updateMovementRange(std::nullopt, ActionType::GROW);
    m_gameUIManager->updateActionType(ActionType::GROW);
    m_gameUIManager->updateGameState(GameState::GAME_RUNING);
    m_boardRenderer->setGameState(GameState::GAME_RUNING);
}