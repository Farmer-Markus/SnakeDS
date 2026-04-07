#pragma once
#include <cstdint>
#include <nds/arm9/sprite.h>


/*
    Max 64*64px
    Each tile 8*8px (64px)
    So max 64 tiles per oam
*/
struct OamTex
{
    SpriteSize size;
    SpriteColorFormat colorFormat;

    const uint8_t *tiles;
    uint16_t tilesLen;
};

// All tiles are using the same palette!