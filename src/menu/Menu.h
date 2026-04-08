#pragma once
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include <Common.h>
#include <menu/Widget.h>
#include <menu/Button.h>


class Menu
{
public:
    using WidgetID = size_t;

    Menu();
    virtual ~Menu() = default;

    // Returns previous visibility
    virtual bool Show();
    // Returns previous visibility
    virtual bool Hide();
    virtual inline bool IsShown() const;

    // Event handlers
    virtual bool MsgButtonDown(const Event::KeyState key);
    virtual bool MsgButtonUp(const Event::KeyState key);
    virtual bool MsgTouchDown(const Event::TouchPos pos);
    virtual bool MsgTouchUp(const Event::TouchPos pos);
    virtual bool MsgTouchMove(const Event::TouchPos newPos);

    template<typename T>
    inline T *AddWidget(std::unique_ptr<T> widget);

protected:
    static constexpr size_t FOCUS_NONE = 11;
    static constexpr size_t MAX_WIDGETS = FOCUS_NONE - 1;

    // Careful! Will get deleted with menu
    virtual inline Widget *GetWidgetFocused();
    // Careful! Will get deleted with menu
    virtual Widget *GetWidget(const WidgetID id);
    // Didn't want to do a even more complicated system with custom ID's for every widget
    virtual Widget *MoveFocus(const Direction direction) = 0;
    virtual Widget *SetFocus(const WidgetID id);

    virtual void DrawAll();


    WidgetID m_focusedWidget;
    std::vector<std::unique_ptr<Widget>> m_widgets;

    bool m_hidden;
};


inline bool Menu::IsShown() const
{
    return !m_hidden;
}

template<typename T>
inline T *Menu::AddWidget(std::unique_ptr<T> widget)
{
    m_widgets.push_back(std::move(widget));
    return static_cast<T*>(GetWidget(m_widgets.size() - 1));
}

inline Widget *Menu::GetWidgetFocused()
{
    return GetWidget(m_focusedWidget);
}
