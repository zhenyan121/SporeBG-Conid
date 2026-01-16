#include "GameScene.h"

GameScene::GameScene() {
    
}

GameScene::~GameScene() {

}

std::unique_ptr<GameUIManager> GameScene::createUIManager() {
    // 默认创建普通的GameUIManager
    return std::make_unique<GameUIManager>(
        [this](const std::string& sceneName) {
            if (m_eventCallback) {
                SceneEvent event{SceneEventType::ChangeScene, sceneName};
                m_eventCallback(event);
            }
        }
    );
}


bool GameScene::preHandleClick(int logicalX, int logicalY) {
    // 默认实现：先处理UI点击
    if (m_gameUIManager && m_gameUIManager->handleClick(logicalX, logicalY)) {
        return true; // UI已处理点击
    }
    
    // 检查游戏状态
    if (m_gameSession->getGameState() != GameState::GAME_RUNING) {
        SDL_Log("Game is not running, click ignored.");
        return true; // 游戏未运行，不处理点击
    }
    
    return false; // 继续处理游戏点击
}

void GameScene::postHandleClick() {

     // 默认实现：更新UI和渲染器状态
    m_gameUIManager->updateActionType(m_gameSession->getCurrentActionType());
    m_boardRenderer->updateMovementRange(m_gameSession->getSelectedPiece(), m_gameSession->getCurrentActionType());
    m_gameUIManager->updateGameState(m_gameSession->getGameState());
    m_boardRenderer->setGameState(m_gameSession->getGameState());
    m_gameUIManager->updatePlayerTurn(m_gameSession->getCurrentPlayer());

}

void GameScene::handleBoardClick(int row, int col) {
    // 处理棋盘点击逻辑
    m_gameSession->handleCoordinateInput(row, col);
    m_gameSession->printBoard();
}

void GameScene::onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT, UIRenderer* uiRenderer, TextureManager* textureManager){
    m_renderer = renderer;
    m_uiRenderer = uiRenderer;

     // 使用工厂方法创建UIManager
    m_gameUIManager = createUIManager();
    m_gameUIManager->init();
    m_gameUIManager->setCallback([this]() {
        this->restartGame();
    });

    m_boardRenderer = std::make_unique<BoardRenderer>(WIDTH, HEIGHT, renderer, textureManager);
    m_gameSession = std::make_unique<GameSession>();
    m_CoordinateConverter = std::make_unique<CoordinateConverter>(renderer);
    m_gameSession->initialize();

    m_boardRenderer->setBoard(m_gameSession->getBoard());

    m_gameSession->setGamePieceEventCallback(
        [this](GamePieceEvent evnet, int row, int col) {
            std::cout << "GameScene: recevie the event piece at " << row << " " << col << "\n";
            m_boardRenderer->handleGamePieceEvent(evnet, row, col);
        }
    );
    
    
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
    // 1. 预处理（UI点击等）
    if (preHandleClick(logicalX, logicalY)) {
        return;
    }
    
    if (m_gameSession->getGameState() != GameState::GAME_RUNING) {
        SDL_Log("Game is not running, click ignored.");
        return;
    }
    auto click = m_CoordinateConverter->ScreenToBoard(logicalX, logicalY, m_boardRenderer->getBoardArea());
        if (click) {
            auto [row, col] = click.value();
            handleBoardClick(row, col);
           
            // 3. 后处理（更新UI状态等）
            postHandleClick();
            /*SDL_Log("try to updateActionType\n");
            m_gameUIManager->updateActionType( m_gameSession->getCurrentActionType());
            SDL_Log("tyr to updateMovementRange\n");
            m_boardRenderer->updateMovementRange(m_gameSession->getSelectedPiece(), m_gameSession->getCurrentActionType());
            SDL_Log("tyr to updateGameState\n");
            m_gameUIManager->updateGameState(m_gameSession->getGameState());
            m_boardRenderer->setGameState(m_gameSession->getGameState());
            */
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

    m_gameSession->setGamePieceEventCallback(
        [this](GamePieceEvent evnet, int row, int col) {
            std::cout << "GameScene: recevie the event piece at " << row << " " << col << "\n";
            m_boardRenderer->handleGamePieceEvent(evnet, row, col);
        }
    );

}