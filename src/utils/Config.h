#pragma once
#include <string>

//  统一管理配置
struct GameConfig {
    /*
        tileSize: 16x16 每个格子的逻辑大小（像素）
        屏幕 = 20 × 11 tiles = 320x176 逻辑分辨率
        640x360 逻辑分辨率（窗口大小的1/2）
        画面更高清晰
    */
    int logicalWidth = 640;
    int logicalHeight = 360;
    std::string windowTitle = "孢子棋";
    bool vsync = true;
    int scale = 2;
    
    /*
        1280x720 scale 4
        1920x1080 scale 6
        2560x1440 scale 8
        3840x2160 scale 12
    */
} ;

// 获取棋盘渲染区域信息（用于坐标转换）
struct BoardArea {
    int x, y;          // 左上角像素坐标
    int cellSize;      // 每格像素大小
    int rows, cols;    // 行列数
};

// ui 相关常量
namespace UI
{
    constexpr int TopBarHeight   = 16;
    constexpr int HotbarHeight   = 32;
    constexpr int ButtonSize     = 16;
    constexpr int SlotSize       = 16;
    constexpr int PanelPadding   = 4;
    constexpr int FontHeight     = 8;
    constexpr int StartWindowWidth = 320 * 4;  // 初始窗口宽度（像素）
    constexpr int StartWindowHeight = 180 * 4; // 初始窗口高度（像素）
    // 字体大小（逻辑像素）
    constexpr int DIALOG_FONT_SIZE = 14;
    constexpr int UI_SMALL_FONT_SIZE = 8;
    constexpr int UI_NORMAL_FONT_SIZE = 12;
    constexpr int UI_LARGE_FONT_SIZE = 16;
}
