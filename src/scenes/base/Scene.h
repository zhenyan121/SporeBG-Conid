// ui/Scene.h - 场景基类
#pragma once
#include <SDL3/SDL.h>




class Scene {
public:
    virtual ~Scene() = default;

    // 空实现，可选择性重写
    virtual void onEnter(SDL_Renderer* renderer, int width, int height){};
    virtual void onExit(){};

    virtual void update() = 0;
    // 暂时先直接传入SDLrender，以后在改，理论上来说scene是应该不知道SDlrenderer的
    virtual void render() = 0;
    // 处理点击
    virtual void handleClick(float x, float y) = 0;

};

