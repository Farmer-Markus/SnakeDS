#include <cstdint>
#include <nds.h>
#include <nds/arm9/background.h>
#include <nds/arm9/video.h>

#include <Menu.h>

#include <data/background-sub.h>
#include <data/menu_palette.h>


Menu::Menu(const MenuType type) : m_currMenu(type)
{
    // Draw the menu
};

void Menu::Init()
{
    // Setup static background
    videoSetModeSub(MODE_0_2D);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_SUB_SPRITE);

    m_bgID = bgInitSub(0, BgType_Text8bpp, BgSize_B16_256x256, 0, 1);
    uint16_t *mem = bgGetMapPtr(m_bgID);
    dmaCopy(background_subMap, mem, background_subMapLen);
    mem = bgGetGfxPtr(m_bgID);
    dmaCopy(background_subTiles, mem, background_subTilesLen);
    dmaCopy(menu_palettePal, BG_PALETTE_SUB, menu_palettePalLen);


}

bool Menu::Tick()
{

    return true;
}

void Menu::ChangeMenu(const MenuType type)
{
    m_currMenu = type;
}