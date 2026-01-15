#pragma once
#include "utils/Config.h"
#include "game/GameTypes.h"
#include <SDL3/SDL.h>
#include <optional>
#include <utility>
#include <unordered_set>
#include "graphics/texture/TextureManager.h"
#include "game/GameTypes.h"
struct PlayerColors {
    SDL_Color P1 = {255, 0, 0, 255};    // 红色
    SDL_Color P2 = {0, 0, 255, 255};    // 蓝色
    SDL_Color selected = {255, 255, 0, 255}; // 黄色（选中状态）
};


class Board;

class BoardRenderer
{
private:
    SDL_Renderer* m_renderer = nullptr; 
    int m_Width;
    int m_Height;
    int m_cellSize;
    int m_boardRow = 7;
    int m_boardCOL = 7;

    const Board* m_board = nullptr;

    // 棋子绘制相关
    float m_pieceRadiusRatio = 0.8f; // 棋子半径相对于格子大小的比例
    PlayerColors m_colors;
    std::unordered_set<int> m_currentDrawRange;

    GameState m_gameState = GameState::GAME_RUNING;

    TextureManager* m_textureManager;

    std::optional<std::pair<int, int>> m_lastSelected = std::nullopt;

public:
    BoardRenderer(int WIDTH, int HEIGHT, SDL_Renderer* renderer, TextureManager* textureManager);  
    
    ~BoardRenderer();
    // 渲染画面
    // 初始化加载纹理
    bool initialize();

    // 设置棋盘
    void setBoard(const Board* board);

    // 渲染函数
    // 绘制背景
    void drawBackground();
    //绘制棋盘
    void drawBoard();
    // 绘制棋子
    void drawPiece(std::optional<std::pair<int, int>> selectedPiece = std::nullopt);

    void updateMovementRange(std::optional<std::pair<int, int>> selectedPiece = std::nullopt,  ActionType type = ActionType::GROW);

    void drawMovementRange();
    BoardArea getBoardArea() const;

    void setGameState(GameState state);
       
    void renderBlackOverlay();

    void handleGamePieceEvent(GamePieceEvent event, int row, int col);
  
};


