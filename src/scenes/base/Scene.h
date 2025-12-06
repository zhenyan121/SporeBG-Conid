// ui/Scene.h - 场景基类
#pragma once
#include <SDL3/SDL.h>




class Scene {
public:
    virtual ~Scene() = default;

    // 空实现，可选择性重写
    virtual void onEnter(){};
    virtual void onExit(){};

    virtual void update() = 0;
    virtual void render(SDL_Renderer* renderer) = 0;

};

