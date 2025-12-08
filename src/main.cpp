#define SDL_MAIN_USE_CALLBACKS 1  // 启用回调函数模式
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "core/GameApplication.h"

//使用SDL3的appstate进行隔离，避免全局变量




static const int WIDTH = 1600;
static const int HEIGHT = 900;


// 1. 应用程序初始化回调
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    // 设置应用元数据[8](@ref)
    SDL_SetAppMetadata("孢子棋", "1.0.0", "com.zhenyan121.sporebgconid");
    
    

    // 创建游戏实例
    auto gameapp = new GameApplication();
    
    
    if (!gameapp->initialize()) {
        SDL_Log("游戏初始化失败!");
        delete gameapp;
        return SDL_APP_FAILURE;
    }
    
    *appstate = gameapp;
    return SDL_APP_CONTINUE;
}

// 2. 事件处理回调
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    auto gameapp = static_cast<GameApplication*>(appstate);
    
    auto result = gameapp->handleInputEvent(event);
    
    return result;
}

// 3. 主循环迭代回调（每帧调用）
SDL_AppResult SDL_AppIterate(void *appstate) {
    auto gameapp = static_cast<GameApplication*>(appstate);
    if (gameapp) {   //防止空指针
    
    gameapp->run();
    
    
    
    }
    return SDL_APP_CONTINUE;
}

// 4. 应用退出回调（清理资源）
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    delete static_cast<GameApplication*>(appstate);
    TTF_Quit();
    SDL_Quit();
}