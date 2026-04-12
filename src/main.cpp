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
#include <nds/timers.h>
#include <nds/touch.h>
#include <menu/IngameMenu.h>

#include <Grid.h>
#include <menu/Button.h>

#include <data/menu_palette.h>
#include <data/start_button.h>
#include <utility>


Menu *currMenu, *subMenu;
Grid *grid;

bool appRunning;
bool gameRunning;

void SwapMenus();



bool MsgCallback(const Event event)
{
    switch(event.type)
    {
        case Event::Type::BUTTON_DOWN: return currMenu->MsgButtonDown(event.key);
        case Event::Type::BUTTON_UP: return currMenu->MsgButtonUp(event.key);
        case Event::Type::TOUCH_DOWN: return currMenu->MsgTouchDown(event.touchPos);
        case Event::Type::TOUCH_UP: return currMenu->MsgTouchUp(event.touchPos);
        case Event::Type::TOUCH_MOVE: return currMenu->MsgTouchMove(event.touchPos);
    }

    return false;
}

inline void StartGame()
{
    gameRunning = true;
    grid = new Grid(10);
    grid->Init();
    grid->Draw();

    SwapMenus();
}

inline void QuitApp()
{
    appRunning = false;
    currMenu->Hide();
}

inline void UpdateScore(size_t score)
{
    static_cast<IngameMenu*>(currMenu)->SetScore(score);
}

void SwapMenus()
{
    currMenu->Hide();
    subMenu = std::exchange(currMenu, subMenu);
    currMenu->Show();
}

void InitGfx()
{
    videoSetMode(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
    vramSetBankB(VRAM_B_MAIN_SPRITE_0x06400000);

    // Init sub graphics system
    videoSetModeSub(MODE_0_2D);
    vramSetBankC(VRAM_C_SUB_BG);
    vramSetBankD(VRAM_D_SUB_SPRITE);

    oamInit(&oamMain, SpriteMapping_1D_32, false);
    oamInit(&oamSub, SpriteMapping_1D_32, false);
}

int main()
{
    cpuStartTiming(0);
    // fatInitDefault(); could help to store data/highscore etc. #include <fat.h>

    appRunning = true;
    gameRunning = false;

    InitGfx();


    currMenu = new MainMenu(StartGame, QuitApp);
    /*subMenu = new IngameMenu();
    subMenu->Hide();*/

    EventManager ev(MsgCallback);

    while(pmMainLoop() && appRunning)
    {
        scanKeys();

        // Update event manager(currently just for menus/ui)
        ev.Tick();
        // Update game if running
        if(gameRunning)
        {
            if(!grid->Tick())
            {
                // Game ended
                gameRunning = false;
                delete grid;
                currMenu->Show();
                // Return to main menu
                SwapMenus();
            } else
            {
                // Game running

            }
        }
            
        threadWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }

    delete currMenu;
    delete subMenu;
}
