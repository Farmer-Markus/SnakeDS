#include <cstddef>
#include <menu/Menu.h>


class IngameMenu : public Menu
{
public:
    IngameMenu();

    bool Show() override;
    bool Hide() override;

    // Set score to be displayed
    IngameMenu *SetScore(const size_t score);

private:

    int m_bgID;
};