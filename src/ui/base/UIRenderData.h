#pragma once
#include "graphics/font/Textstyle.h"
#include <vector>
#include <string>
struct ButtonData
{
    std::string text;
    TextStyle textstytle;
    SDL_FRect rect;
    SDL_Color backgroundColor;

    int borderThickness;
    SDL_Color borderColor;
};

// 可以通过索引方式实现，及直接传递Button指针, 在UIRender里面调用getData
struct UIRenderData
{
    std::vector<ButtonData> buttons;
};
