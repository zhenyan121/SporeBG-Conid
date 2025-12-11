#include "Button.h"

Button::Button()
                                                                     
{

}


void Button::setText(const std::string& text, TextStyle style) {
    m_buttonData.text = text;
    m_buttonData.textstytle = style;
}

void Button::setBackgroundColor(SDL_Color normal) {
    m_buttonData.backgroundColor = normal;
}

void Button::setBorder(int thickness, SDL_Color color) {
    m_buttonData.borderThickness = thickness;
    m_buttonData.borderColor = color;
}


void Button::update(float deltaTime) {

}






