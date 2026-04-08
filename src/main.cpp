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

#include <Grid.h>
#include <menu/Button.h>

#include <data/menu_palette.h>
#include <data/start_button.h>


Menu *menu;
Grid *grid;

bool appRunning;
bool gameRunning;

bool MsgCallback(const Event event)
{
    if(!menu) return false;

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

inline void StartGame()
{
    gameRunning = true;
    grid = new Grid(10);
    grid->Init();
    menu->Hide();
    grid->Draw();
    // Switch to ingame menu
}

inline void QuitApp()
{
    appRunning = false;
    menu->Hide();
}

int main()
{
    cpuStartTiming(0);
    // fatInitDefault(); could help to store data/highscore etc. #include <fat.h>

    appRunning = true;
    gameRunning = false;


    menu = new MainMenu(StartGame, QuitApp);
    EventManager ev(MsgCallback);

    while(pmMainLoop() && appRunning)
    {
        scanKeys();

        // Update event manager(currently just for menus/ui)
        ev.Tick();
        // Update game if running
        if(gameRunning && !grid->Tick())
        {
            gameRunning = false;
            delete grid;
            menu->Show();
            // Return to main menu
        }
            
        threadWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }

    delete menu;
}
