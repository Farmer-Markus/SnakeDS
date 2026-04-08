#include <cstdlib>
#include <memory>
#include <nds/arm9/background.h>
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
#include <data/background_sub.h>

void ExitGame()
{
    std::exit(0);
}


MainMenu::MainMenu()
{
    // Create background
    m_bgID = bgInitSub(3, BgType_Text8bpp, BgSize_T_256x256, 0, 1);
    // Copy tiles
    uint16_t *bgMem = bgGetGfxPtr(m_bgID);
    dmaCopy(background_subTiles, bgMem, background_subTilesLen);
    // Copy map
    bgMem = bgGetMapPtr(m_bgID);
    dmaCopy(background_subMap, bgMem, background_subMapLen);
    // Copy background palette
    dmaCopy(menu_palettePal, BG_PALETTE_SUB, menu_palettePalLen);
    bgSetPriority(m_bgID, 3);
    

    // Copy Sprite palette
    dmaCopy(menu_palettePal, SPRITE_PALETTE_SUB + 1, menu_palettePalLen);

    // Create buttons
    OamTex oamTex;
    oamTex.colorFormat = SpriteColorFormat_256Color;
    oamTex.size = SpriteSize_64x32;

    // Start btn texture
    oamTex.tiles = reinterpret_cast<const uint8_t*>(start_buttonTiles);
    oamTex.tilesLen = start_buttonTilesLen;

    Button *btn = AddWidget(std::make_unique<Button>(&oamSub, 0, oamTex));
    btn->SetX(SCREEN_WIDTH / 2 - (btn->GetWidth() / 2));
    btn->SetY(BUTTON_START_Y);
    btn->Draw();

    // Exit btn texture
    oamTex.tiles = reinterpret_cast<const uint8_t*>(exit_buttonTiles);
    oamTex.tilesLen = exit_buttonTilesLen;

    btn = AddWidget(std::make_unique<Button>(&oamSub, 1, oamTex));
    btn->SetX(SCREEN_WIDTH / 2 - (btn->GetWidth() / 2));
    btn->SetY(BUTTON_EXIT_Y);
    btn->Draw();
    btn->SetOnClickedCallback(ExitGame);
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
