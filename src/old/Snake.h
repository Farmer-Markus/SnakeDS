#include <cstdint>

#include <Common.h>
#include <nds/arm9/sprite.h>
#include <vector>

typedef Vec2D<int8_t> Pos;

// Uses undefined oamSlots starting at given slot, 4 mat slots
class Snake
{
    static constexpr int SPRITE_SIZE = 32 * 32;
    struct BodyPart
    {
        Pos pos;
        Direction dir;
    };
public:
    Snake(OamState *oam, const uint8_t oamSlot, const uint8_t matSlot);
    // Initializes four 32*32 sprites and 4 mats. Will NOT set palette
    void Init();

    // Set oam for each body part
    void Draw();

    void Forward();
    void ChangeDir(const Direction dir);

private:
    Direction GetTurnDir(const Pos start, const Pos dest);

    const uint8_t m_oamSlot;
    const uint8_t m_matSlot;
    OamState *m_oam;
    uint16_t *m_spriteMem[4];

    // 0 tail size is head
    std::vector<BodyPart> m_body;
    bool m_shouldExpand = false;
};

