#include <Snake.h>
#include <Grid.h>

#include <nds/arm9/sprite.h>
#include <nds/dma.h>

#include <data/snake_head.h>
#include <data/snake_body.h>
#include <data/snake_tail.h>
#include <data/snake_turn.h>

namespace {
// turns lookuptable
static constexpr uint8_t TURNS[13] = {
    0, 1, 0, 1,
    3, 0, 0, 0,
    2, 2, 0, 0,
    3
};
}


Snake::Snake(OamState *oam, const uint8_t oamSlot, const uint8_t matSlot) : m_oam(oam), m_oamSlot(oamSlot), m_matSlot(matSlot)
{
    // First last is head
    Pos p{1, 1};
    Direction m_direction = Direction::O;
    m_body.push_back(BodyPart{p, m_direction});
    p.X++;
    m_body.push_back(BodyPart{p, m_direction});
    p.X++;
    m_body.push_back(BodyPart{p, m_direction});
    p.X++;
    m_body.push_back(BodyPart{p, m_direction});
    p.X++;
    m_body.push_back(BodyPart{p, m_direction});
    p.X++;
    m_body.push_back(BodyPart{p, m_direction});
    p.X++;
    m_body.push_back(BodyPart{p, m_direction});
    p.X++;
    m_body.push_back(BodyPart{p, m_direction});
    p.X++;
    m_body.push_back(BodyPart{p, m_direction});
}

void Snake::Init()
{
    // Allocate video memory for 3 sprites
    for(uint8_t i = 0; i < 4; i++)
        m_spriteMem[i] = oamAllocateGfx(m_oam, SpriteSize_16x16, SpriteColorFormat_256Color);

    // Copy all body sprites to video memory
    dmaCopy(snake_headTiles, m_spriteMem[0], SPRITE_SIZE);
    dmaCopy(snake_bodyTiles, m_spriteMem[1], SPRITE_SIZE);
    dmaCopy(snake_tailTiles, m_spriteMem[2], SPRITE_SIZE);
    dmaCopy(snake_turnTiles, m_spriteMem[3], SPRITE_SIZE);


    // Setup rotation mats
    int ninetyDegree = degreesToAngle(90);
    for(uint8_t i = 0; i < 4; i++)
        oamRotateScale(m_oam, m_matSlot + i, ninetyDegree * i, 255, 255);
}

void Snake::Draw()
{
    // Draw tail
    BodyPart bp = m_body[0];
    oamSet(m_oam, m_oamSlot, bp.pos.X * GRID_SIZE_PX, bp.pos.Y * GRID_SIZE_PX, 0, 0, SpriteSize_16x16,
            SpriteColorFormat_256Color, m_spriteMem[2], m_matSlot + TURNS[static_cast<uint8_t>(bp.dir)], false, false, false, false, false);

    // Draw body
    uint8_t i = 1;
    BodyPart pBp = bp; // previous bodyPart
    for(; i < m_body.size() - 1; i++)
    {
        bp = m_body[i];
        if(pBp.dir != bp.dir)
        {
            uint8_t turn = static_cast<uint8_t>(bp.dir) | static_cast<uint8_t>(pBp.dir);

            oamSet(m_oam, m_oamSlot + i + 2, bp.pos.X * GRID_SIZE_PX, bp.pos.Y * GRID_SIZE_PX, 0, 0, SpriteSize_16x16,
            SpriteColorFormat_256Color, m_spriteMem[3], m_matSlot + TURNS[turn], false, false, false, false, false);
        } else
        {
            oamSet(m_oam, m_oamSlot + i + 2, bp.pos.X * GRID_SIZE_PX, bp.pos.Y * GRID_SIZE_PX, 0, 0, SpriteSize_16x16,
            SpriteColorFormat_256Color, m_spriteMem[1], m_matSlot + TURNS[static_cast<uint8_t>(bp.dir)], false, false, false, false, false);
        }

        pBp = bp;
    }

    // Draw head
    bp = m_body[i++];
    oamSet(m_oam, m_oamSlot + 1, bp.pos.X * GRID_SIZE_PX, bp.pos.Y * GRID_SIZE_PX, 0, 0, SpriteSize_16x16,
            SpriteColorFormat_256Color, m_spriteMem[0], m_matSlot + TURNS[static_cast<uint8_t>(bp.dir)], false, false, false, false, false);
}

void Snake::Forward()
{
    // New head position
    BodyPart bp = m_body[m_body.size() - 1];
    Pos nPos = bp.pos;

    switch(bp.dir)
    {
        case Direction::N:
            nPos.Y--;
            break;

        case Direction::O:
            nPos.X++;
            break;
        
        case Direction::S:
            nPos.Y++;
            break;

        case Direction::W:
            nPos.X--;
            break;

        default:
            return;
    }

    m_body.push_back(BodyPart{nPos, bp.dir});

    if(!m_shouldExpand)
        m_body.erase(m_body.begin());
}

void Snake::ChangeDir(const Direction dir)
{
    m_body[m_body.size() - 1].dir = dir;
}