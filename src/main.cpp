#include "menu/Event.h"
#include "menu/EventManager.h"
#include "menu/Texture.h"
#include <calico/nds/touch.h>
#include <cstdio>
#include <nds.h>
#include <nds/arm9/console.h>
#include <nds/arm9/sprite.h>
#include <nds/touch.h>

#include <Grid.h>
#include <menu/Button.h>

#include <data/menu_palette.h>
#include <data/start_button.h>


void DebugMsgCallback(const Event event)
{
    switch(event.key)
    {
        case KEY_UP:
            fprintf(stderr, "UP\n");
            break;

        case KEY_LEFT:
            fprintf(stderr, "LEFT\n");
            break;

        case KEY_DOWN:
            fprintf(stderr, "DOWN\n");
            break;

        case KEY_RIGHT:
            fprintf(stderr, "RIGHT\n");
            break;
    }
}


int main()
{
    // fatInitDefault(); could help to store data/highscore etc. #include <fat.h>

    videoSetModeSub(MODE_0_2D);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    oamInit(&oamSub, SpriteMapping_1D_32, false);

    dmaCopy(menu_palettePal, SPRITE_PALETTE_SUB + 1, menu_palettePalLen);

    OamTex btnTex;
    btnTex.size = SpriteSize_64x32;
    btnTex.colorFormat = SpriteColorFormat_256Color;
    btnTex.tiles = reinterpret_cast<const uint8_t*>(start_buttonTiles);
    btnTex.tilesLen = start_buttonTilesLen;
    Button btn(&oamSub, 0, btnTex);

    btn.SetX(75);

    // consoleDemoInit();
    // EventManager ev(DebugMsgCallback);



    //int i = 0;
    while(pmMainLoop())
    {
        scanKeys();

        btn.Draw();

        threadWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
}
