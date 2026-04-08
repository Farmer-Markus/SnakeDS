#include <cstdint>
#include <nds/arm9/sprite.h>
#include <nds/dma.h>

#include <menu/Texture.h>
#include <menu/ImageView.h>


ImageView::ImageView(OamState *oam, const uint8_t oamSlot, const OamTex texture)
                    : m_oam(oam), m_oamSlot(oamSlot), m_texture(texture)
{
    Show();
    SetX(0);
    SetY(0);

    // Allocate vram -> copy sprite texture into vram
    m_spriteMem = oamAllocateGfx(m_oam, m_texture.size, m_texture.colorFormat);
    dmaCopy(m_texture.tiles, m_spriteMem, m_texture.tilesLen);
}

void ImageView::Draw() const
{
    oamSet(m_oam, m_oamSlot, m_pos.X, m_pos.Y, 0, 0, m_texture.size,
        m_texture.colorFormat, m_spriteMem, -1, false, m_hidden, false, false, false);
}
