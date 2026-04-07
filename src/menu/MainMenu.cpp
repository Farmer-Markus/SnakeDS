#include <memory>
#include <nds/arm9/sprite.h>
#include <nds/arm9/video.h>
#include <nds/dma.h>

#include <Common.h>
#include <menu/Button.h>
#include <menu/Texture.h>
#include <menu/Widget.h>
#include <menu/MainMenu.h>

#include <data/start_button.h>
#include <data/exit_button.h>
#include <data/menu_palette.h>


MainMenu::MainMenu()
{
    // Copy palette
    dmaCopy(menu_palettePal, SPRITE_PALETTE_SUB + 1, menu_palettePalLen);

    // Create buttons
    OamTex oamTex;
    oamTex.colorFormat = SpriteColorFormat_256Color;
    oamTex.size = SpriteSize_64x32;

    oamTex.tiles = reinterpret_cast<const uint8_t*>(start_buttonTiles);
    oamTex.tilesLen = start_buttonTilesLen;

    Widget *widget = AddWidget(std::unique_ptr<Widget>(new Button(&oamSub, 0, oamTex)));
    widget->SetX(SCREEN_WIDTH / 2 - (widget->GetWidth() / 2));
    widget->SetY(BUTTON_START_Y);
    widget->Draw();

    oamTex.tiles = reinterpret_cast<const uint8_t*>(exit_buttonTiles);
    oamTex.tilesLen = exit_buttonTilesLen;

    widget = AddWidget(std::unique_ptr<Widget>(new Button(&oamSub, 1, oamTex)));
    widget->SetX(SCREEN_WIDTH / 2 - (widget->GetWidth() / 2));
    widget->SetY(BUTTON_EXIT_Y);
    widget->Draw();
}

Widget *MainMenu::MoveFocus(const Direction direction)
{
    if(GetWidgetFocused() == nullptr)
        return SetFocus(0);

    if(direction == Direction::N)
    {
        if(m_focusedWidget > 0)
            return SetFocus(m_focusedWidget - 1);

    } else if(direction == Direction::S)
    {
        if(m_focusedWidget < BUTTON_COUNT - 1)
            return SetFocus(m_focusedWidget + 1);
    }

    return GetWidgetFocused();
}
