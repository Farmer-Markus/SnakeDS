#include <cstdint>
#include <nds/arm9/sprite.h>
#include <nds/dma.h>

#include <Common.h>
#include <menu/Event.h>
#include <menu/Texture.h>
#include <menu/Button.h>


Button::Button(OamState *oam, const uint8_t oamSlot, const OamTex texture)
                : m_oam(oam), m_oamSlot(oamSlot), m_oamTex(texture)
{
    Show();
    SetClicked(false);
    SetX(0);
    SetY(0);
    SetSizeByTex();
    SetOnClickedCallback(nullptr);

    // Allocate memory in vram
    m_oamTexMem = oamAllocateGfx(m_oam, m_oamTex.size, m_oamTex.colorFormat);
    // Copy texture data to vram
    dmaCopy(texture.tiles, m_oamTexMem, texture.size);
}

Button::~Button()
{
    // Free texture in vram
    oamFreeGfx(m_oam, m_oamTexMem);
    // Clear sprite entry
    oamClearSprite(m_oam, m_oamSlot);
}

void Button::Draw() const
{
    uint16_t offset = IsClicked() ? CLICK_OFFSET : 0;
    oamSet(m_oam, m_oamSlot, m_pos.X, m_pos.Y + offset, !IsClicked(), 0, m_oamTex.size,
        m_oamTex.colorFormat, m_oamTexMem, 0, false, m_hidden, false, false, false);
}

bool Button::MsgButtonDown(const Event::KeyState keyState)
{
    if(keyState & KEY_A && !IsClicked())
    {
        SetClicked(true);
        return true;
    }

    return false;
}

bool Button::MsgButtonUp(const Event::KeyState keyState)
{
    if(keyState & KEY_A && IsClicked())
    {
        // When A was pressed & btn has focus && is clicked -> execute action
        SetClicked(false);
        Callback();
        return true;
    }
    
    return false;
}

bool Button::MsgTouchDown(const Event::TouchPos touchPos)
{
    if(PosOnButton(touchPos))
    {
        SetClicked(true);
        return true;
    }

    return false;
}

bool Button::MsgTouchUp(const Event::TouchPos touchPos)
{
    if(IsClicked() && PosOnButton(touchPos))
    {
        // Execute action when pressed
        Callback();
        return true;
    }
    
    if(IsClicked())
    {
        SetClicked(false);
        return true;
    }

    return false;
}

// There probably is a better way to do this :/
void Button::SetSizeByTex()
{
    switch(m_oamTex.size)
    {
        default:
        case SpriteSize_8x8:
            m_size = Vec2D< uint16_t>{8,8};
            break;
        case SpriteSize_16x16:
            m_size = Vec2D< uint16_t>{16,16};
            break;
        case SpriteSize_32x32:
            m_size = Vec2D< uint16_t>{32,32};
            break;
        case SpriteSize_64x64:
            m_size = Vec2D< uint16_t>{64,64};
            break;
        case SpriteSize_16x8:
            m_size = Vec2D< uint16_t>{16,8};
            break;
        case SpriteSize_32x8:
            m_size = Vec2D< uint16_t>{32,8};
            break;
        case SpriteSize_32x16:
            m_size = Vec2D< uint16_t>{32,16};
            break;
        case SpriteSize_64x32:
            m_size = Vec2D< uint16_t>{64,32};
            break;
        case SpriteSize_8x16:
            m_size = Vec2D< uint16_t>{8,16};
            break;
        case SpriteSize_8x32:
            m_size = Vec2D< uint16_t>{8,32};
            break;
        case SpriteSize_16x32:
            m_size = Vec2D< uint16_t>{16,32};
            break;
        case SpriteSize_32x64:
            m_size = Vec2D< uint16_t>{32,64};
            break;
    }
}

bool Button::PosOnButton(const Vec2D<uint16_t> pos) const
{
    bool isX = pos.X >= m_pos.X && pos.X <= m_pos.X + m_size.X;
    bool isY = pos.Y >= m_pos.Y && pos.Y <= m_pos.Y + m_size.Y;
    return isX && isY;
}

void Button::Callback() const
{
    if(m_callback != nullptr)
        m_callback();
}
