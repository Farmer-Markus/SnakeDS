#pragma once
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include <Common.h>
#include <menu/Widget.h>
#include <menu/Button.h>

typedef size_t MenuWidgetID;


class Menu
{
protected:
    static constexpr size_t FOCUS_NONE = 11;
    static constexpr size_t MAX_WIDGETS = FOCUS_NONE - 1;
public:
    Menu();
    virtual ~Menu() = default;

    // Event handlers
    virtual bool MsgButtonDown(const KeyState key);
    virtual bool MsgButtonUp(const KeyState key);
    virtual bool MsgTouchDown(const TouchPos pos);
    virtual bool MsgTouchUp(const TouchPos pos);
    virtual bool MsgTouchMove(const TouchPos newPos);

    virtual inline Widget *AddWidget(std::unique_ptr<Widget> widget);

protected:
    // Careful! Will get deleted with menu
    virtual inline Widget *GetWidgetFocused();
    // Careful! Will get deleted with menu
    virtual Widget *GetWidget(const MenuWidgetID id);
    // Didn't want to do a even more complicated system with custom ID's for every widget
    virtual Widget *MoveFocus(const Direction direction) = 0;
    virtual Widget *SetFocus(const MenuWidgetID id);

    virtual void DrawAll();


    MenuWidgetID m_focusedWidget;
    std::vector<std::unique_ptr<Widget>> m_widgets;
};


inline Widget *Menu::AddWidget(std::unique_ptr<Widget> widget)
{
    m_widgets.push_back(std::move(widget));
    return GetWidget(m_widgets.size() - 1);
}

inline Widget *Menu::GetWidgetFocused()
{
    return GetWidget(m_focusedWidget);
}
