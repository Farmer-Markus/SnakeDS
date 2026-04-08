#include <cstdint>
#include <nds/arm9/sprite.h>

#include <menu/Texture.h>
#include <menu/Widget.h>


/*
    Simple image view
    Display simple texture
*/
class ImageView : public Widget
{
public:
    ImageView(OamState *oam, const uint8_t oamSlot, const OamTex texture);
    
    void Draw() const override;

private:

    OamState *m_oam;
    const uint8_t m_oamSlot;

    const OamTex m_texture;
    uint16_t *m_spriteMem;
};
