#include <calico/gba/keypad.h>

#include <Common.h>
#include <menu/Event.h>
#include <menu/Widget.h>
#include <menu/Menu.h>

#define SHOWN true
#define HIDDEN false

namespace
{
    static constexpr uint8_t DEFAULT_WIDGET_RESERVE = 2;
}


Menu::Menu()
{
    m_widgets.reserve(DEFAULT_WIDGET_RESERVE);
    m_focusedWidget = FOCUS_NONE;
    m_hidden = false;
}

bool Menu::Show()
{
    if(IsShown())
        return SHOWN;

    m_hidden = false;
    for(auto& widget : m_widgets)
        widget->Show()->Draw();

    return HIDDEN;
}

bool Menu::Hide()
{
    if(!IsShown())
        return HIDDEN;

    m_hidden = true;
    for(auto& widget : m_widgets)
        widget->Hide()->Draw();

    return SHOWN;
}

bool Menu::MsgButtonDown(const Event::KeyState key)
{
    if(!IsShown())
        return false;

    // Pass event down to focused widget
    if(Widget *widget = GetWidgetFocused(); widget != nullptr)
    {
        if(widget->MsgButtonDown(key))
        {
            widget->Draw();
            return true;
        }
    }

    return false;
}

bool Menu::MsgButtonUp(const Event::KeyState key)
{
    if(!IsShown())
        return false;

    if(Widget *widget = GetWidgetFocused(); widget != nullptr)
    {
        if(widget->MsgButtonUp(key))
        {
            widget->Draw();
            return true;
        }
    }

    // If widget did not handle the event, let menu use it
    switch(key)
    {
        case KEY_UP:
            MoveFocus(Direction::N);
            return true;
        case KEY_DOWN:
            MoveFocus(Direction::S);
            return true;
        case KEY_LEFT:
            MoveFocus(Direction::W);
            return true;
        case KEY_RIGHT:
            MoveFocus(Direction::O);
            return true;
    }

    return false;
}

// Only pass initial event to all -> next events just to focused widget
bool Menu::MsgTouchDown(const Event::TouchPos pos)
{
    if(!IsShown())
        return false;
    
    // Inform every widget until one consumes the event
    Widget *widget;
    for(WidgetID id = 0; id < m_widgets.size(); id++)
    {
        widget = GetWidget(id);
        if(widget->MsgTouchDown(pos))
        {
            widget->Draw();
            SetFocus(id);
            return true;
        }
    }

    return false;
}

bool Menu::MsgTouchUp(const Event::TouchPos pos)
{
    if(!IsShown())
        return false;
    
    if(Widget *widget = GetWidgetFocused(); widget != nullptr)
    {
        if(widget->MsgTouchUp(pos))
        {
            widget->Draw();
            return true;
        }            
    }

    return false;
}

bool Menu::MsgTouchMove(const Event::TouchPos newPos)
{
    if(!IsShown())
        return false;
    
    if(Widget *widget = GetWidgetFocused(); widget != nullptr)
    {
        if(widget->MsgTouchMove(newPos))
        {
            widget->Draw();
            return true;
        }
    }

    return false;
}

Widget *Menu::GetWidget(const WidgetID id)
{
    if(id >= MAX_WIDGETS || id >= m_widgets.size())
        return nullptr;

    return m_widgets.at(id).get();
}

Widget *Menu::MoveFocus(const Direction)
{
    return nullptr;
}

Widget *Menu::SetFocus(const WidgetID id)
{
    if(id == FOCUS_NONE)
    {
        m_focusedWidget = FOCUS_NONE;
        return GetWidgetFocused();
    }

    if(id >= MAX_WIDGETS || id >= m_widgets.size())
        return nullptr;

    m_focusedWidget = id;
    Widget *widget = GetWidgetFocused();
    widget->Draw();
    return widget;
}

void Menu::DrawAll()
{
    for(auto& widget : m_widgets)
        widget->Draw();
}
