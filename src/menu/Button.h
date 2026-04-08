#pragma once
#include <cstdint>
#include <nds/arm9/sprite.h>

#include <Common.h>
#include <menu/Texture.h>
#include <menu/Event.h>
#include <menu/Widget.h>


/*
    Simple button. Activate using touch or A key
    Run callback on click
*/
class Button : public Widget
{
public:
    using OnClickCallback = void (*)();


    Button(OamState *oam, const uint8_t oamSlot, const OamTex texture);
    ~Button();

    void Draw() const override;

    // Event handlers
    bool MsgButtonDown(const Event::KeyState keyState) override;
    bool MsgButtonUp(const Event::KeyState keyState) override;
    bool MsgTouchDown(const Event::TouchPos touchPos) override;
    bool MsgTouchUp(const Event::TouchPos touchPos) override;

    // Set callback function to be executed when button was clicked
    inline Button *SetOnClickedCallback(const OnClickCallback callback);
    inline Button *SetClicked(const bool clicked);

    inline bool IsClicked() const;

private:
    // Y offset used to indicate clicked state
    static constexpr uint16_t CLICK_OFFSET = 3;

    // Set size for boundry checking by texture size
    void SetSizeByTex();
    bool PosOnButton(const Vec2D<uint16_t> pos) const;
    // Return false if no callback set, true otherwise
    bool Callback() const;

    inline bool HasCallback() const;

    
    OamState *m_oam;
    const uint8_t m_oamSlot;

    const OamTex m_oamTex; // Holds pointer to tile data/len
    uint16_t *m_oamTexMem;

    bool m_clicked;
    OnClickCallback m_callback;
};


inline Button* Button::SetOnClickedCallback(const OnClickCallback callback)
{
    m_callback = callback;
    return this;
}

inline Button* Button::SetClicked(const bool clicked)
{
    m_clicked = clicked;
    return this;
}


inline bool Button::IsClicked() const
{
    return m_clicked;
}

inline bool Button::HasCallback() const
{
    return m_callback;
}
