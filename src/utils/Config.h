#pragma once
#include <string>
#include<SDL3/SDL.h>
//  统一管理配置
struct WindowConfig {
    /*
        tileSize: 16x16 每个格子的逻辑大小（像素）
        屏幕 = 20 × 11 tiles = 320x176 逻辑分辨率
        640x360 逻辑分辨率（窗口大小的1/2）
        画面更高清晰
    */
    int width = 320 * 4;  // 初始窗口宽度（像素）
    int height = 180 * 4; // 初始窗口高度（像素）
    std::string title = "孢子棋";
    bool vsync = true;
    
    

    /*
        1280x720 scale 4
        1920x1080 scale 6
        2560x1440 scale 8
        3840x2160 scale 12
    */
} ;

struct RenderConfig {
    int logicalWidth = 640;
    int logicalHeight = 360;
    int scale = 2;
};

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
    constexpr int LogicalWidth = 640;
    constexpr int LogicalHeight = 360;
    
    // 字体大小（逻辑像素）
    constexpr int DIALOG_FONT_SIZE = 14;
    constexpr int UI_SMALL_FONT_SIZE = 8;
    constexpr int UI_NORMAL_FONT_SIZE = 14;
    constexpr int UI_LARGE_FONT_SIZE = 16;


    
}
// Viewport 描述的是：
// 「当前这一帧，逻辑画面是如何映射到窗口上的」
//
// ⚠️ 这是只读数据，其它系统只能用，不能改
struct Viewport {
    // ===== 窗口相关 =====
    int windowWidth  = 0;   // 窗口实际宽度（像素）
    int windowHeight = 0;   // 窗口实际高度（像素）

    // ===== 逻辑分辨率 =====
    int logicalWidth  = 0;  // 游戏内部使用的宽度（如 320）
    int logicalHeight = 0;  // 游戏内部使用的高度（如 180）

    // ===== 缩放 =====
    int scale = 1;         // 整数缩放倍数（像素游戏核心）

    // ===== 实际渲染区域 =====
    // 逻辑画面被放大后，在窗口中的位置和大小
    // 用于 SDL_RenderTexture / 输入坐标转换
    SDL_FRect dst{};
};

struct Config {
    WindowConfig window;
    RenderConfig render;
};