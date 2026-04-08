#pragma once
#include <calico/gba/keypad.h>
#include <cstdint>

#include <Common.h>

// Inspired by SDL

/*
    Switch case can be used with 'key' as only one key
    can be pressed per event
*/
struct Event
{
    using KeyState = uint32_t;
    using TouchPos = Vec2D<uint16_t>;

    enum class Type : uint8_t
    {
        BUTTON_DOWN = 1 << 0,
        BUTTON_UP = 1 << 1,
        TOUCH_DOWN = 1 << 2,
        TOUCH_UP = 1 << 3,
        TOUCH_MOVE = 1 << 4
    };

    Type type;

    KeyState key;
    TouchPos touchPos;
};
