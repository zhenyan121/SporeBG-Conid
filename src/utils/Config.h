#pragma once
#include <string>

//  统一管理配置
struct GameConfig {
    int windowWidth = 1600;
    int windowHeight = 900;
    std::string windowTitle = "孢子棋";
    bool vsync = true;
} ;

// 获取棋盘渲染区域信息（用于坐标转换）
struct BoardArea {
    int x, y;          // 左上角像素坐标
    int cellSize;      // 每格像素大小
    int rows, cols;    // 行列数
};    