#include "Common.h"
#include <cstdint>
#include <nds.h>
#include <sys/types.h>

typedef Vec2D<uint8_t> Size;
typedef Vec2D<uint8_t> Pos;


// Supposed to run on sub engine only!
class Menu
{
    struct Button
    {
        uint ID;

        Size size;
        Pos pos;
    };


public:
    Menu();
    void Init();

    // Checks touch pos, returns false when game ended
    bool Tick();

    void Show();
    void Hide();

    inline bool IsVisible();

private:

    //  
    // Plane* m_visiblePlane;
    bool m_hidden;
};