#pragma once
#include <calico/gba/keypad.h>
#include <cstdint>

#include <Common.h>

// Inspired by SDL

typedef uint16_t KeyState;
typedef Vec2D<uint16_t> TouchPos;
static constexpr uint8_t KEY_COUNT = 14; // With hinge & debug button


enum EventType : uint8_t
{
    BUTTON_DOWN = 1 << 0,
    BUTTON_UP = 1 << 1,
    TOUCH_DOWN = 1 << 2,
    TOUCH_UP = 1 << 3,
    TOUCH_MOVE = 1 << 4
};

/*
    Switch case can be used with 'key' as only one key
    can be pressed per event
*/
struct Event
{
    EventType eventType;

    KeyState key;
    TouchPos touchPos;
};

