#pragma once
#include <nds.h>
#include <cstdint>
#include <vector>

#include <Common.h>

typedef Vec2D<int8_t> Pos;
typedef std::vector<Pos> SnakeBody;


// Uses undefined oamSlots starting at given slot, 4 mat slots
class Snake
{
    static constexpr int SPRITE_SIZE = 32 * 32;
public:
    Snake(OamState *oam, const uint8_t oamSlot, const uint8_t oamMax, const uint8_t matSlot);
    ~Snake();
    // Initializes four 32*32 sprites and 4 mats. Will NOT set palette
    void Init();
    // Set oam for each body part
    void Draw();
    void Move();
    // Set direction
    bool SetDir(const Direction dir);
    void Expand();
    // Usefull for collision detection
    const SnakeBody& GetBody() const;

private:
    inline Direction DirFromPos(const Pos start, const Pos dest) const;

    OamState *m_oam;
    const uint8_t m_oamSlot;
    const uint8_t m_oamMax;
    const uint8_t m_matSlot;
    uint16_t *m_spriteMem[4];

    // 0 tail size is head
    SnakeBody m_body;
    Direction m_direction;

    // SetDir would swap out m_direction and the user could just
    // go 180 degree around(snake hasn't moved yet. multiple
    // inputs between frame)
    Direction m_directionNew;
    bool m_shouldExpand = false;
};
