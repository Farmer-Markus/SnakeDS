#pragma once
#include <cstddef>
#include <nds/arm9/video.h>

#include <Common.h>
#include <menu/Widget.h>
#include <menu/Menu.h>
#include <menu/Button.h>


class MainMenu : public Menu
{
public:
    MainMenu(const Button::OnClickCallback gameStartCallback, const Button::OnClickCallback gameQuitCallback);

    bool Show() override;
    bool Hide() override;
private:
    static constexpr size_t BUTTON_COUNT = 2;
    static constexpr uint16_t BUTTON_START_Y = SCREEN_HEIGHT / 2;
    static constexpr uint16_t BUTTON_EXIT_Y = BUTTON_START_Y + 40;

    Widget *MoveFocus(const Direction direction) override;
    inline void StartGame() const;

    int m_bgID;
};
