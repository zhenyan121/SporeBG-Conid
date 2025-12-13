#include "Label.h"

Label::Label(const std::string& text, TextStyle style) :
    m_labelData({text, style})
{

}
Label::Label() {
    
}


void Label::setText(const std::string& text, TextStyle style) {
    m_labelData.text = text;
    m_labelData.textstytle = style;
}

void Label::setText(const std::string& text) {
    m_labelData.text = text;
    
}

void Label::setBackgroundColor(SDL_Color normal) {
    m_labelData.backgroundColor = normal;
}

void Label::setBorder(int thickness, SDL_Color color) {
    m_labelData.borderThickness = thickness;
    m_labelData.borderColor = color;
}


void Label::update(float deltaTime) {

}
