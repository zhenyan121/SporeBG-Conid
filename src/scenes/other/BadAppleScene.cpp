#include "BadAppleScene.h"
#include "core/Time.h"
#include <iostream>
static constexpr int ROW = 22;
static constexpr int COL = 30;
static constexpr int FPS = 30;
static constexpr float FrameTime = 1 / static_cast<float>(FPS);
BadAppleScene::BadAppleScene() {

}

BadAppleScene::~BadAppleScene() {

}

void BadAppleScene::onEnter(SDL_Renderer* renderer, int WIDTH, int HEIGHT, UIRenderer* uiRenderer, TextureManager* textureManager, CoreData* coreData) {
    m_renderer = renderer;
    m_uiRenderer = uiRenderer;
    m_coreData = coreData;
    m_Width = WIDTH;
    m_Height = HEIGHT;
    m_cellSize = std::min(WIDTH, HEIGHT) / std::min(ROW, COL);
    m_area = {
        (m_Width - m_cellSize * COL) / 2,
        (m_Height - m_cellSize * ROW) / 2,
        m_cellSize,
        ROW,
        COL
    };

    m_file.open("assets/BadApple.txt");
    if (!m_file.is_open()) {
        std::cerr << "BadAppleScene: fail to open BadApple.txt\n";
    }
}

void BadAppleScene::update() {

    if (m_coreData->inputState.isQ) {
        m_coreData->inputState.isQ = false;
        m_eventCallback({SceneEventType::ChangeScene, SceneType::MainMenuScene});
        
    }

    if (m_coreData->inputState.isR) {
        m_coreData->inputState.isR = false;
        if (!m_file.is_open()) {
             m_file.open("assets/BadApple.txt");
            if (!m_file.is_open()) {
                std::cerr << "BadAppleScene: fail to open BadApple.txt\n";
                return;
            }
        }
        m_file.clear();
        m_file.seekg(0);
    }

    if (!m_file.is_open() || m_file.eof()) {
        return;
    }
    m_currentTime += Time::deltaTime();
    if (m_currentTime >= FrameTime) {
        m_currentTime = 0;
        updateFrame();
    }
}

void BadAppleScene::updateFrame() {
    if (!readFrame()) {
        std::cerr << "BadAppleScene: fail to read frame\n";
        return;
    }
    if (m_file.eof()) {
        return;
    }

}

bool BadAppleScene::readFrame() {
    m_frame.clear();
    for (int i = 0; i <= ROW; i++) {
        std::string line;

        if(!std::getline(m_file, line)) {
            if (m_file.eof()) {
                std::cout << "BadAppleScene: read all frame successfully\n";
                return true;
            } else {
                std::cerr << "BadAppleScene: unknown error while readiing frame\n";
                m_file.close();
                return false;
            }
        }
        
        if (i == 0) {
            continue;
        }
        if (line.length() != COL) {
            std::cerr << "BadAppleScene: file is corrupted\n";
            m_file.close();
            return false;
        }
        m_frame.push_back(line);
        
    }
    return true;
}

void BadAppleScene::renderWorld() {
    if (m_frame.empty()) {
        return;
    }
    // 绘制格子
    SDL_SetRenderDrawColor(m_renderer, 80, 80, 80, 255); // 浅灰边框
    for (int row = 0; row < m_area.rows; ++row) {
        for (int col = 0; col < m_area.cols; ++col) {
            // 使用 SDL_FRect（浮点数）
            SDL_FRect rect{
                static_cast<float>(m_area.x + col * m_area.cellSize),
                static_cast<float>(m_area.y + row * m_area.cellSize),
                static_cast<float>(m_area.cellSize),
                static_cast<float>(m_area.cellSize)
            };

            // SDL3: RenderFillRect 接受 const SDL_FRect*
            SDL_RenderRect(m_renderer, &rect);
        }
    }
    // 绘制棋子
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255); // 纯黑填充
    for (int row = 0; row < m_area.rows; ++row) {
        for (int col = 0; col < m_area.cols; ++col) {
            if (m_frame[row][col] == ' ') {
                SDL_FRect rect{
                static_cast<float>(m_area.x + col * m_area.cellSize),
                static_cast<float>(m_area.y + row * m_area.cellSize),
                static_cast<float>(m_area.cellSize),
                static_cast<float>(m_area.cellSize)
                };
            
                SDL_RenderFillRect(m_renderer, &rect);
            }

            

        }
    }
    
}

void BadAppleScene::renderUI() {

}

void BadAppleScene::handleClick(int logicalX, int logicalYY) {

}