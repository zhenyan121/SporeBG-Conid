#define SDL_MAIN_USE_CALLBACKS 1  // 启用回调函数模式
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "core/Game.h"


//使用SDL3的appstate进行隔离，避免全局变量
struct AppState {
    std::unique_ptr<Game> game;
};



static const int WIDTH = 1600;
static const int HEIGHT = 900;


// 1. 应用程序初始化回调
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    // 设置应用元数据[8](@ref)
    SDL_SetAppMetadata("孢子棋", "1.0.0", "com.zhenyan121.sporechess");
    
    // 初始化SDL视频子系统[6,7](@ref)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL初始化失败: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    // 创建游戏实例
    auto state = new AppState();
    state->game = std::make_unique<Game>(WIDTH, HEIGHT);
    
    
    if (!state->game->initialize()) {
        SDL_Log("游戏初始化失败!");
        delete state;
        return SDL_APP_FAILURE;
    }
    
    
    return SDL_APP_CONTINUE;
}

// 2. 事件处理回调
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    auto s = static_cast<AppState*>(appstate);
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    
    
    return SDL_APP_CONTINUE;
}

// 3. 主循环迭代回调（每帧调用）
SDL_AppResult SDL_AppIterate(void *appstate) {
    auto s = static_cast<AppState*>(appstate);
    if (s->game) {   //防止空指针
    // 更新游戏逻辑
    
    
    // 渲染帧
    s->game->render();
    }
    return SDL_APP_CONTINUE;
}

// 4. 应用退出回调（清理资源）
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    delete static_cast<AppState*>(appstate);
    SDL_Quit();
}