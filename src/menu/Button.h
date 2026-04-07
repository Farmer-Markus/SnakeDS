#pragma once
#include <cstdint>
#include <nds/arm9/sprite.h>

#include <Common.h>
#include <menu/Texture.h>
#include <menu/Event.h>
#include <menu/Widget.h>

typedef void (*OnClickCallback)();


class Button : public Widget
{
    static constexpr uint16_t CLICK_OFFSET = 3;

public:
    Button(OamState *oam, const uint8_t oamSlot, const OamTex texture);
    ~Button();

    void Draw() const override;

    // Event handlers
    bool MsgButtonDown(const KeyState keyState) override;
    bool MsgButtonUp(const KeyState keyState) override;
    bool MsgTouchDown(const TouchPos touchPos) override;
    bool MsgTouchUp(const TouchPos touchPos) override;

    // Set callback function to be executed when button was clicked
    inline Button *SetCallback(const OnClickCallback callback);
    inline Button *SetClicked(const bool clicked);

    inline bool IsClicked() const;

private:
    // Set size for boundry checking by texture size
    void SetSizeByTex();
    bool PosOnButton(const Vec2D<uint16_t> pos) const;
    void Callback() const;

    OamState *m_oam;
    const uint8_t m_oamSlot;

    const OamTex m_oamTex; // Holds pointer to tile data/len
    uint16_t *m_oamTexMem;

    bool m_clicked;
    OnClickCallback m_callback;
};


inline Button* Button::SetCallback(const OnClickCallback callback)
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
