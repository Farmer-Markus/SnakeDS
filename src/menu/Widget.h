#pragma once
#include <Common.h>
#include <menu/Event.h>

typedef Vec2D<uint16_t> WidgetPosition;
typedef Vec2D<uint16_t> WidgetSize;


class Widget
{
public:
    virtual ~Widget() = default;

    virtual void Draw() const = 0;

    // Event handlers
    virtual bool MsgButtonDown(const KeyState) { return false; }
    virtual bool MsgButtonUp(const KeyState) { return false; }
    virtual bool MsgTouchDown(const TouchPos) { return false; }
    virtual bool MsgTouchUp(const TouchPos) { return false; }
    virtual bool MsgTouchMove(const TouchPos) { return false; }

    virtual inline Widget *Show();
    virtual inline Widget *Hide();
    virtual inline Widget *SetX(const uint16_t x);
    virtual inline Widget *SetY(const uint16_t y);
    virtual inline Widget *SetWidth(const uint16_t w);
    virtual inline Widget *SetHeight(const uint16_t h);

    virtual inline bool IsShown() const;
    virtual inline uint16_t GetX() const;
    virtual inline uint16_t GetY() const;
    virtual inline uint16_t GetWidth() const;
    virtual inline uint16_t GetHeight() const;

protected:

    bool m_hidden;
    bool m_focus;

    WidgetPosition m_pos;
    WidgetSize m_size;
};


////////////////////////////
// Inline implementations //
////////////////////////////
inline Widget *Widget::Show()
{
    m_hidden = false;
    return this;
}

inline Widget *Widget::Hide()
{
    m_hidden = true;
    return this;
}


inline Widget *Widget::SetX(const uint16_t x)
{
    m_pos.X = x;
    return this;
}

inline Widget *Widget::SetY(const uint16_t y)
{
    m_pos.Y = y;
    return this;
}

inline Widget *Widget::SetWidth(const uint16_t w)
{
    m_size.X = w;
    return this;
}

inline Widget *Widget::SetHeight(const uint16_t h)
{
    m_size.Y = h;
    return this;
}


inline bool Widget::IsShown() const
{
    return !m_hidden;
}

inline uint16_t Widget::GetX() const
{
    return m_pos.X;
}

inline uint16_t Widget::GetY() const
{
    return m_pos.Y;
}

inline uint16_t Widget::GetWidth() const
{
    return m_size.X;
}

inline uint16_t Widget::GetHeight() const
{
    return m_size.Y;
}
