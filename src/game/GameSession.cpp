// src/core/GameSession.cpp
#include "GameSession.h"


GameSession::GameSession()         
{
    m_board = std::make_unique<Board>(ROWS, COLS);
    
}


GameSession::~GameSession() {
    cleanup();
}
//清理资源
void GameSession::cleanup() {
    
     
}


bool GameSession::initialize() {

    // 重置游戏状态为运行中
    m_gameState = GameState::GAME_RUNING;
    
    // 重置其它必要状态
    m_currentPlayer = PlayerID::P1;
    m_currentActionType = ActionType::GROW;
    m_seletedPiece = std::nullopt;

    // 初始化游戏特定资源（棋盘、棋子等）
    if (!m_board->initialize()) {
        return false;
    }
    m_board->printBoard();
    resetActionableComponents();
    resetOldPieceIDtoComponent();
   
    
    
    // ...
    return true;
}

void GameSession::resetActionableComponents() {
    
    m_actionableComponents = m_board->getAllPlayerComponent(m_currentPlayer);
}

void GameSession::resetOldPieceIDtoComponent() {
    m_oldPieceIDtoComponentID = m_board->getALLPiecetoComponent();
}

void GameSession::setPlayerAction(ActionType type) {
    m_currentActionType = type;
}


bool GameSession::executeAction(int toRow, int toCol) {
    auto [fromRow, fromCol] = *m_seletedPiece;
    if (m_currentActionType == ActionType::GROW) {
        if (Rule::canGrow(m_board.get(), fromRow, fromCol, toRow, toCol, m_currentPlayer)) {
            auto fromPiece = m_board->getPieceAt(fromRow, fromCol);
            auto toPiece = m_board->getPieceAt(toRow, toCol);
            auto fromInfo = fromPiece->getPieceInfo();
            if (fromInfo.HP / 2 == 0) {
                // 因为每个玩家结束全部棋子恢复2HP，所以理论来说是不可能GROW死的
                m_gamePieceEventCallback(GamePieceEvent::REMOVE_PIECE, fromRow, fromCol, -1, -1);
                m_board->removePieceAt(fromRow, fromCol);
                markComponentAsUsed(getOldComponentID(fromRow, fromCol));
                return true;
            }
            fromInfo.HP /= 2;
            fromInfo.ATK += 2;
            m_gamePieceEventCallback(GamePieceEvent::PLACE_PIECE, toRow, toCol, -1, -1);
            m_gamePieceEventCallback(GamePieceEvent::GROW_PIECE, fromRow, fromCol, toRow, toCol);
            m_board->placePieceAt(toRow, toCol, m_currentPlayer);
            m_board->setPieceInfo(fromRow, fromCol, fromInfo);
            m_board->setPieceInfo(toRow, toCol, fromInfo);
            
            // 如果执行了操作就擦除
            markComponentAsUsed(getOldComponentID(fromRow, fromCol));
            return true;
        }
    }
    if (m_currentActionType == ActionType::MOVE) {
        if (Rule::canMove(m_board.get(), fromRow, fromCol, toRow, toCol, m_currentPlayer)) {
            //m_gamePieceEventCallback(GamePieceEvent::REMOVE_PIECE, fromRow, fromCol, -1, -1);
            //m_gamePieceEventCallback(GamePieceEvent::REMOVE_PIECE, toRow, toCol, -1, -1);
            //m_gamePieceEventCallback(GamePieceEvent::MOVE_PIECE, fromRow, fromCol, toRow, toCol);
            auto fromPiece = m_board->getPieceAt(fromRow, fromCol);
            auto toPiece = m_board->getPieceAt(toRow, toCol);

            if (!fromPiece) {
                std::cout << "GameSession: fromPiece is null\n";
                exit(EXIT_FAILURE);
            }
            
            if (!toPiece) {
                m_gamePieceEventCallback(GamePieceEvent::MOVE_PIECE, fromRow, fromCol, toRow, toCol);
                auto fromInfo = fromPiece->getPieceInfo();
                m_gamePieceEventCallback(GamePieceEvent::REMOVE_PIECE, fromRow, fromCol, -1, -1);
                m_board->removePieceAt(fromRow, fromCol);
                m_board->placePieceAt(toRow, toCol, m_currentPlayer);
                m_board->setPieceInfo(toRow, toCol, fromInfo);
                //m_board->changeHP(toRow, toCol, -5);
                markComponentAsUsed(getOldComponentID(fromRow, fromCol));
                return true;
            }

            m_board->changeHP(toRow, toCol, -fromPiece->getATK());
            m_board->changeHP(fromRow, fromCol, -toPiece->getATK() * 0.5);
            if (fromPiece->getHP() > 0 && toPiece->getHP() > 0) {
                m_gamePieceEventCallback(GamePieceEvent::FIGHT_PIECE, fromRow, fromCol, toRow, toCol);
            }
            if (fromPiece->getHP() <= 0) {
                m_gamePieceEventCallback(GamePieceEvent::REMOVE_PIECE, fromRow, fromCol, -1, -1);
                m_board->removePieceAt(fromRow, fromCol);
            }
            if (toPiece->getHP() <= 0) {
                m_gamePieceEventCallback(GamePieceEvent::REMOVE_PIECE, toRow, toCol, -1, -1);
                m_board->removePieceAt(toRow, toCol);

                if (fromPiece->getHP() > 0) {
                    m_gamePieceEventCallback(GamePieceEvent::MOVE_PIECE, fromRow, fromCol, toRow, toCol);
                    
                    auto fromInfo = fromPiece->getPieceInfo();
                    m_board->removePieceAt(fromRow, fromCol);
                    
                    m_board->placePieceAt(toRow, toCol, m_currentPlayer);
                    m_board->setPieceInfo(toRow, toCol, fromInfo);
                }
            }
            
            
            
            markComponentAsUsed(getOldComponentID(fromRow, fromCol));
            return true;
        }
    }
    if (m_currentActionType == ActionType::SPORE) {
        if (Rule::canSpore(m_board.get(), fromRow, fromCol, toRow, toCol, m_currentPlayer)) {
            m_gamePieceEventCallback(GamePieceEvent::REMOVE_PIECE, fromRow, fromCol, -1, -1);
            m_gamePieceEventCallback(GamePieceEvent::PLACE_PIECE, toRow, toCol, -1, -1);
            m_gamePieceEventCallback(GamePieceEvent::MOVE_PIECE, fromRow, fromCol, toRow, toCol);
            
            auto fromPiece = m_board->getPieceAt(fromRow, fromCol);
            auto toPiece = m_board->getPieceAt(toRow, toCol);
            auto fromInfo = fromPiece->getPieceInfo();
            m_board->removePieceAt(fromRow, fromCol);
            m_board->placePieceAt(toRow, toCol, m_currentPlayer);
            m_board->setPieceInfo(toRow, toCol, fromInfo);
            m_board->changeHP(toRow, toCol, -2);
            markComponentAsUsed(getOldComponentID(fromRow, fromCol));
            return true;
        }
    }
    return false;

}

void GameSession::markComponentAsUsed(int componentID) {
    std::cout << "try erase the componentID is " << componentID <<"\n";
    int num = m_actionableComponents.erase(componentID);
    if (num == 1) {
        std::cout << "erase successful\n";
    } else {
        std::cout << "erase error\n";
    }
}

PlayerID GameSession::getCurrentPlayer() const {
    return m_currentPlayer;
}

void GameSession::printBoard() const {
    m_board->printBoard();
}

void GameSession::nextTurn() {
    std::cout << "GameSession: switch to " << ((m_currentPlayer == PlayerID::P1) ? "P2" : "P1") << "\n";
    m_seletedPiece = std::nullopt;
    m_currentPlayer = (m_currentPlayer == PlayerID::P1) ? PlayerID::P2 : PlayerID::P1;
    resetOldPieceIDtoComponent();
    resetActionableComponents();
    
    m_currentActionType = ActionType::GROW;
    if (m_currentPlayer == PlayerID::P1) {
        m_gameRound++;
        std::cout << "GameSession: Current Round is " << m_gameRound << "\n";
    }
    
    // 回合结束增加生命值

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            //int pieceID = m_board->getPieceID(i, j);
            m_board->changeHP(i, j, Stat::HealthRegenPerTurn);
        }
    }

}

bool GameSession::handleCoordinateInput(int row, int col) {
    if (!m_board) {
        std::cout << "board is null\n";
        return false;
    }
    if (m_gameState != GameState::GAME_RUNING) {
        std::cout << "game is not running\n";
        return false;
    }
    // 如果当前没有选择棋子就选择棋子
    if (m_seletedPiece == std::nullopt) {
        if (!Rule::canbeSelect(m_board->getPieceAt(row, col), m_currentPlayer)) {
            std::cout << "sorry you can't select the piece\n";
            return false;
        }
        m_seletedPiece = {row, col};
        std::cout << "selcte piece successful\n";
        return true;
    }
    std::cout << "selectedpiece is " << m_seletedPiece->first << " " << m_seletedPiece->second << "\n";
    // 如果点击了选择的棋子就切换行动方式
    std::pair<int ,int> newSelectedPiece = {row, col}; 
    if (m_seletedPiece ==  newSelectedPiece) {
        switch (m_currentActionType)
        {
        case ActionType::GROW:
            m_currentActionType = ActionType::MOVE;
            std::cout << "switch actiontype to MOVE successful\n";
            break;
        case ActionType::MOVE:
            m_currentActionType = ActionType::SPORE;
            std::cout << "switch actiontype to SPORE successful\n";
            break;
        case ActionType::SPORE:
            m_currentActionType = ActionType::GROW;
            std::cout << "switch actiontype to GROW successful\n";
            break;
        default:
            std::cout << "switch actiontype wrong\n";
            return false;
            
        }
        
        return true;
    }
    //如果点击了属于当前玩家且没有行动的的其它棋子，就切换选择棋子到其它棋子
    int selectComponentID = getOldComponentID(row, col);
    if (m_actionableComponents.find(selectComponentID) != m_actionableComponents.end()) {
        m_seletedPiece = newSelectedPiece;
        std::cout << "switch the selectedpiece to " << m_seletedPiece->first << " " << m_seletedPiece->second << "\n";
        return true;
    }
    
    // 如果点击了已经行动的棋子，则不进行处理,已行动的棋子如果是在一个单独的区域不用处理，但如果是在一个未行动的组件中欧是可以再次行动的，
    //但是如果点击同一块 也无需处理，因为 在m_actionableComponents已经不存在了，直接尝试执行行动，但因为rule处理了所以不用管

    // 其它情况则执行行动
    if(!executeAction(row, col)) {
        std::cout << "action pos invaild!\n";
        return false;
    }
    
    auto opponent = (m_currentPlayer == PlayerID::P1) ? PlayerID::P2 : PlayerID::P1;;
    //检查胜利条件
    if (m_board->getAllPlayerComponent(opponent).empty()) {
        m_gameState = GameState::GAME_WIN;
        m_actionableComponents.clear();
        m_seletedPiece = std::nullopt;
        std::cout << "Player " << ((m_currentPlayer == PlayerID::P1) ? "P1" : "P2") << " wins!\n";
        return true;
    }
    // 执行完之后检查是否m_actionableComponents为空，

    // m_actionableComponents只保存了ID，但是在执行棋子之后会处理组件的连通性，rule会获取到新的连通性，不过这样更符合逻辑
    
    //如果不是空的这默认将m_seletedPiece切换到这个组件之中
    //如果是空的则进行下一轮切换玩家
    if (m_actionableComponents.empty()) {
        nextTurn();
        return true;
    } else {
        for (auto leftcomponentID : m_actionableComponents) {
            std::cout << "leftcomponentID is " << leftcomponentID << "\n";
        }  
        auto leftcomponent =  m_actionableComponents.begin();
        m_seletedPiece = m_board->getCoordFromID(*leftcomponent);
        std::cout << "switch the selectedpiece to " << m_seletedPiece->first << " " << m_seletedPiece->second << "\n";
        return true; 
    }
    

}

int GameSession::getOldComponentID(int row, int col) {
    int pieceID = m_board->getPieceID(row, col);
    auto it = m_oldPieceIDtoComponentID.find(pieceID);
    //for (auto [pieceID, y] : m_oldPieceIDtoComponentID)
    return (it != m_oldPieceIDtoComponentID.end()) ? it->second : -1;
}

 std::optional<std::pair<int, int>> GameSession::getSelectedPiece() const {
    if (m_gameState != GameState::GAME_RUNING) {
        return std::nullopt;
    }
    return m_seletedPiece;
 }

 const Board* GameSession::getBoard() const {
    return m_board.get();
 }

 ActionType GameSession::getCurrentActionType() const {
    return m_currentActionType;
 }

 GameState GameSession::getGameState() const {
    return m_gameState;
 }

 PieceInfo GameSession::getPieceInfo(int row, int col) const {
    PieceInfo pieceInfo;

    auto piece = m_board->getPieceAt(row, col);
    
    if (!piece) {
        pieceInfo.hasPiece = false;
        return pieceInfo;
    }
    pieceInfo.hasPiece = true;
    pieceInfo.HP = piece->getHP();
    pieceInfo.ATK = piece->getATK();

    return pieceInfo;

 }