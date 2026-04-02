#include <cstdint>


enum class MenuType : uint8_t
{
    MAIN,
    GAME
};

class Menu
{
public:
    Menu(const MenuType type);
    void Init();
    // False when game should quit
    bool Tick();

    // Switch to another menu
    void ChangeMenu(const MenuType type);

private:

    int m_bgID;
    MenuType m_currMenu;
};