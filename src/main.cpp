#include "menu/Event.h"
#include "menu/EventManager.h"
#include "menu/MainMenu.h"
#include "menu/Menu.h"
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


Menu *menu;

bool DebugMsgCallback(const Event event)
{
    switch(event.type)
    {
        case Event::Type::BUTTON_DOWN: return menu->MsgButtonDown(event.key);
        case Event::Type::BUTTON_UP: return menu->MsgButtonUp(event.key);
        case Event::Type::TOUCH_DOWN: return menu->MsgTouchDown(event.touchPos);
        case Event::Type::TOUCH_UP: return menu->MsgTouchUp(event.touchPos);
        case Event::Type::TOUCH_MOVE: return menu->MsgTouchMove(event.touchPos);
    }

    return false;
}



int main()
{
    // fatInitDefault(); could help to store data/highscore etc. #include <fat.h>

    videoSetModeSub(MODE_0_2D);
    vramSetBankC(VRAM_C_SUB_BG_0x06200000);
    vramSetBankD(VRAM_D_SUB_SPRITE);
    oamInit(&oamSub, SpriteMapping_1D_32, false);

    menu = new MainMenu();
    EventManager ev(DebugMsgCallback);

    while(pmMainLoop())
    {
        scanKeys();

        ev.Tick();

        threadWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
}
