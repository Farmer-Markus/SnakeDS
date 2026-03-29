#include <nds.h>

#include <Grid.h>


int main()
{
    // fatInitDefault(); could help to store data/highscore etc. #include <fat.h>
    Grid grid(8);
    grid.Init();

    while(pmMainLoop())
    {
        scanKeys();
        if(!grid.Tick())
            break;
        threadWaitForVBlank();
        oamUpdate(&oamMain);
    }
}
