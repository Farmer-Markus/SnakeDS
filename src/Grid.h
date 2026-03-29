#pragma once
#include <nds.h>
#include <cstdint>
#include <random>

#include <Snake.h>
#include <Common.h>

#define GRID_TO_PIX(val) (val * GRID_SIZE_PX)

// Collision actions
enum class CollAction
{
    NO_ACTION,  // Nothing
    WALL_ACTION,// Crashed into wall
    SELF_ACTION,// Crashed into himself
    EAT_ACTION  // Eat apple
};

static constexpr uint8_t GRID_SIZE_PX = 16;
static constexpr uint8_t GRID_WIDTH = SCREEN_WIDTH / GRID_SIZE_PX;
static constexpr uint8_t GRID_HEIGHT = SCREEN_HEIGHT / GRID_SIZE_PX;

typedef Vec2D<int8_t> Pos;
class Snake;


class Grid
{
    static constexpr Pos GRID_AREA = Pos{GRID_WIDTH, GRID_HEIGHT};
public:
    Grid(const uint8_t snakeInterval);
    ~Grid();

    // Initialize gfx, videomode etc.
    void Init();
    // Main function for input, gfx output and collision detection
    bool Tick();

private:
    CollAction CheckCollisions(const SnakeBody& sb);
    // false if no space left to place apple
    bool RespawnApple(const SnakeBody& sb);
    inline void RedrawApple();

    // Background
    int m_bgID;
    uint16_t *m_bgMapMem;
    uint16_t *m_bgTileMem;

    Snake *m_snake;
    uint8_t m_snakeInterval;
    uint8_t m_snakeTimer;

    // Apple
    uint16_t *m_appleMem;
    Pos m_apple;

    // Randomizer
    std::mt19937 *m_randSeed;
    std::uniform_int_distribution<> *m_randX;
    std::uniform_int_distribution<> *m_randY;
};
