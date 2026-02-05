#pragma once
#include "ui/components/Button.h"
#include "ui/components/Label.h"
class UIWidgetFactory {
public:

    static std::unique_ptr<Button> createStandardButton(const std::string& name, const std::string& text, int x, int y, std::function<void()> callback) {
        auto button = std::make_unique<Button>();
        button->setBackgroundColor({0, 150, 255, 255});
        button->setBorder(2, {0, 0, 0, 255});
        button->setRect(x, y, UI::ButtonSize * 6, UI::ButtonSize * 4);
        button->setName(name);
        button->setText(text, {"SourceHanSansSC-Regular.otf", UI::UI_NORMAL_FONT_SIZE, {0, 0, 0, 255}});
        button->setCallback(callback);
        return button;
    }

    static std::unique_ptr<Label> createStandardLabel(const std::string& name, const std::string& text, int x, int y) {
        auto label = std::make_unique<Label>();
        label->setRect(x, y, UI::ButtonSize * 4, UI::ButtonSize * 2);
        label->setText(text, {"SourceHanSansSC-Regular.otf", UI::UI_NORMAL_FONT_SIZE, {0, 0, 0, 255}});
        label->setName(name);
        label->setVisible(true);
        label->setBackgroundColor({0, 0, 0, 0});
        return label;
    }

};