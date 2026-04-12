#include <chrono>
#include <ctime>
#include <nds.h>
#include <nds/arm9/sprite.h>
#include <nds/timers.h>
#include <random>

#include <Grid.h>
#include <Snake.h>
#include <Common.h>

// Sprites
#include <data/background.h>
#include <data/apple.h>
#include <data/palette.h>


/* bool screenMain = true */
Grid::Grid(const uint8_t snakeInterval) : m_snakeInterval(snakeInterval)
{
    m_snakeTimer = 0;
    m_apple = Pos{};
}

Grid::~Grid()
{
    delete m_snake;
    delete m_randSeed;
    delete m_randX;
    delete m_randY;

    oamFreeGfx(&oamMain, m_appleMem);
    oamClearSprite(&oamMain, 0);
}

void Grid::Init()
{
    // Allocate memory for background
    m_bgID = bgInit(0, BgType_Text8bpp, BgSize_T_256x256, 0, 1);
    m_bgMapMem = bgGetMapPtr(m_bgID);
    m_bgTileMem = bgGetGfxPtr(m_bgID);
    
    // Copy background map, tiles and color palette
    dmaCopy(backgroundMap, m_bgMapMem, backgroundMapLen);
    dmaCopy(backgroundTiles, m_bgTileMem, backgroundTilesLen);
    dmaCopy(backgroundPal, BG_PALETTE, backgroundPalLen);

    // color palette (all sprites will use same palette)
    dmaCopy(palettePal, SPRITE_PALETTE, palettePalLen);

    // Initialize snake. Oam slot 1 is used for apple. No mats used here so use first slot
    m_snake = new Snake(&oamMain, 1, SPRITE_COUNT - 1, 0);
    m_snake->Init();

    // Copy apple sprite to vram
    m_appleMem = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
    dmaCopy(appleTiles, m_appleMem, appleTilesLen);

    // Initialize random generator
    time_t seed = static_cast<time_t>(timerTick(0) ^ time(NULL));
    m_randSeed = new std::mt19937(seed);
    m_randX = new std::uniform_int_distribution<>(0, GRID_WIDTH - 1);
    m_randY = new std::uniform_int_distribution<>(0, GRID_HEIGHT - 1);

    RespawnApple(m_snake->GetBody());
}

bool Grid::Tick()
{
    uint32_t keys = keysDown();

    if(keys & KEY_UP)
        m_snake->SetDir(Direction::N);
    else if(keys & KEY_DOWN)
        m_snake->SetDir(Direction::S);
    else if(keys & KEY_LEFT)
        m_snake->SetDir(Direction::W);
    else if(keys & KEY_RIGHT)
        m_snake->SetDir(Direction::O);

    // Exit
    if(keys & KEY_SELECT)
        return false;

    if(m_snakeTimer >= m_snakeInterval)
    {
        m_snake->Move();
        // Handle collisions
        switch(CheckCollision(m_snake->GetBody()))
        {
            case CollAction::WALL_ACTION:
            case CollAction::SELF_ACTION:
                return false; // currently just exit
                break;

            case CollAction::EAT_ACTION:
                m_snake->Expand(); // Grow snake
                // Reposition & redraw apple
                RespawnApple(m_snake->GetBody());
                RedrawApple();
                break;

            case CollAction::NO_ACTION:
                break;
        }

        m_snake->Draw();
        m_snakeTimer = 0;
    }
    m_snakeTimer++;

    return true;
}

void Grid::Draw() const
{
    m_snake->Draw();
    RedrawApple();
}

CollAction Grid::CheckCollision(const SnakeBody& sb)
{
    const Pos head = sb.at(sb.size() - 1);
    if(head == m_apple)
        return CollAction::EAT_ACTION;

    Pos part;
    uint8_t i = 0;
    for(; i < sb.size() - 1; i++) // Skip head
    {
        part = sb.at(i);
        // Collision with screen edges
        if(part < 0 || part >= GRID_AREA)
            return CollAction::WALL_ACTION;

        // Snake head collision with body
        if(part == head)
            return CollAction::SELF_ACTION;
    }

    // Now check head
    part = sb.at(i);
    if(part < 0 || part >= GRID_AREA)
        return CollAction::WALL_ACTION;

    return CollAction::NO_ACTION;
}

bool Grid::RespawnApple(const SnakeBody&)
{
    // TODO: Do NOT spawn apple inside of snake

    Pos newPos;
    newPos.X = (*m_randX)(*m_randSeed);
    newPos.Y = (*m_randY)(*m_randSeed);

    m_apple = newPos;

    return true;
}

inline void Grid::RedrawApple() const
{
    oamSet(&oamMain, 0, GRID_TO_PIX(m_apple.X), GRID_TO_PIX(m_apple.Y), 0, 0, SpriteSize_16x16,
        SpriteColorFormat_256Color, m_appleMem, -1, false, false, false, false, false);
}
