#include <nds.h>

#include <Common.h>
#include <Snake.h>
#include <Grid.h>

// Sprite data
#include <data/snake_head.h>
#include <data/snake_body.h>
#include <data/snake_tail.h>
#include <data/snake_turn.h>

namespace {
// Rotation lookuptable
static constexpr uint8_t MAT_TABLE[13] = {
    0,3,0,
    0,1,0,
    1,0,2,
    3,0,0,
    2
};
}


Snake::Snake(OamState *oam, const uint8_t oamSlot, const uint8_t oamMax, const uint8_t matSlot) : m_oam(oam), m_oamSlot(oamSlot), m_oamMax(oamMax), m_matSlot(matSlot)
{
    // First last is head
    Pos p{1, 1};
    m_direction = Direction::O;
    m_body.push_back(p);
    p.X++;
    m_body.push_back(p);
}

Snake::~Snake()
{
    for(uint8_t i = 0; i < 4; i++)
        oamFreeGfx(m_oam, m_spriteMem[i]);

    oamClear(m_oam, m_oamSlot, m_oamSlot + m_oamMax);
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
    int ninetyDegree = -degreesToAngle(90); // - to become clockwise
    for(uint8_t i = 0; i < 4; i++)
        oamRotateScale(m_oam, m_matSlot + i, ninetyDegree * i, 255, 255);
}

void Snake::Draw()
{
    // 1 oam slot  = tail
    // 2 oam slot  = head   // 3* oam slot = body

    // Draw tail
    Direction spDir = DirFromPos(m_body[0], m_body[1]); // Sprite direction
    oamSet(m_oam, m_oamSlot, GRID_TO_PIX(m_body[0].X), GRID_TO_PIX(m_body[0].Y), 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
        m_spriteMem[2], MAT_TABLE[static_cast<uint8_t>(spDir)], false, false, false, false, false);

    Direction nxDir, prDir; // Next direction, previous direction(inverted)
    uint8_t i = 1;
    for(; i < m_body.size() - 1; i++)
    {
        Pos pPos = m_body[i - 1];   // Previous body pos
        Pos cPos = m_body[i];       // Current body pos
        Pos nPos = m_body[i + 1];   // Next body pos
        nxDir = DirFromPos(cPos, nPos); // Dir compared to next part
        prDir = DirFromPos(cPos, pPos); // Reverse dir compared to prev part
        spDir = DirFromPos(pPos, cPos); // Dir compared to prev part

        // Turn or not?
        if(spDir == nxDir)
        {
            // No turn
            oamSet(m_oam, m_oamSlot + i + 1, GRID_TO_PIX(cPos.X), GRID_TO_PIX(cPos.Y), 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
                m_spriteMem[1], MAT_TABLE[static_cast<uint8_t>(spDir)],false,false,false,false,false);
            continue;
        }

        // Turn
        uint8_t spRot = static_cast<uint8_t>(prDir) | static_cast<uint8_t>(nxDir); // Get sprite rotation index
        oamSet(m_oam, m_oamSlot + i + 1, GRID_TO_PIX(cPos.X), GRID_TO_PIX(cPos.Y), 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
            m_spriteMem[3], MAT_TABLE[static_cast<uint8_t>(spRot)],false,false,false,false,false);
    }

    // Draw head
    oamSet(m_oam, m_oamSlot + 1, GRID_TO_PIX(m_body[i].X), GRID_TO_PIX(m_body[i].Y), 0, 0, SpriteSize_16x16, SpriteColorFormat_256Color,
        m_spriteMem[0], MAT_TABLE[static_cast<uint8_t>(m_direction)], false, false, false, false, false);
}

void Snake::Move()
{
    m_direction = m_directionNew;
    // New head position
    Pos nPos = m_body[m_body.size() - 1];

    switch(m_direction)
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

    m_body.push_back(nPos);

    // Do not expand over sprite limit
    if(!m_shouldExpand || m_body.size() >= m_oamMax)
        m_body.erase(m_body.begin());

    if(m_shouldExpand)
        m_shouldExpand = false;
}

bool Snake::SetDir(const Direction dir)
{
    uint8_t d = static_cast<uint8_t>(m_direction) | static_cast<uint8_t>(dir);
    
    if(d == 5 || d == 10)
        return false;

    m_directionNew = dir;
    return true;
}

void Snake::Expand()
{
    m_shouldExpand = true;
}

const SnakeBody& Snake::GetBody() const
{
    return m_body;
}

inline Direction Snake::DirFromPos(const Pos start, const Pos dest) const
{
    if(start.X < dest.X) return Direction::O;
    if(start.X > dest.X) return Direction::W;
    if(start.Y < dest.Y) return Direction::S;
    return Direction::N;
}
