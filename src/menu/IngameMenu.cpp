#include <cstddef>
#include <menu/IngameMenu.h>
#include <nds/arm9/background.h>


#include <data/menu_palette.h>
#include <data/background_sub.h>

#define SHOWN true
#define HIDDEN false


IngameMenu::IngameMenu()
{
    /* TODO: CREATE BACKGROUND & SPRITES!!
    
    // Create background
    m_bgID = bgInitSub(2, BgType_Text8bpp, BgSize_T_256x256, 0, 1);
    // Copy tiles
    uint16_t *bgMem = bgGetGfxPtr(m_bgID);
    dmaCopy(background_subTiles, bgMem, background_subTilesLen);
    // Copy map
    bgMem = bgGetMapPtr(m_bgID);
    dmaCopy(background_subMap, bgMem, background_subMapLen);
    // Copy background palette
    dmaCopy(menu_palettePal, BG_PALETTE_SUB, menu_palettePalLen);
    bgSetPriority(m_bgID, 3);*/

}

bool IngameMenu::Show()
{
    if(Menu::Show() == HIDDEN)
    {
        bgShow(m_bgID);
        return HIDDEN;
    }

    return SHOWN;
}

bool IngameMenu::Hide()
{
    if(Menu::Hide() == SHOWN)
    {
        bgHide(m_bgID);
        return SHOWN;
    }

    return HIDDEN;
}

IngameMenu *IngameMenu::SetScore(const size_t)
{
    // Draw score
    return this;
}