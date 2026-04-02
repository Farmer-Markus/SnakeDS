#include <calico/nds/touch.h>
#include <nds.h>
#include <nds/touch.h>

#include <Grid.h>
#include <Menu.h>


int main()
{
    // fatInitDefault(); could help to store data/highscore etc. #include <fat.h>
    Grid grid(8);
    Menu menu(MenuType::MAIN);

    grid.Init();
    menu.Init();

    while(pmMainLoop())
    {
        scanKeys();
        if(!grid.Tick())
            break;
        threadWaitForVBlank();
        oamUpdate(&oamMain);
    }
    TouchData td;
    touchRead(&td);
}
