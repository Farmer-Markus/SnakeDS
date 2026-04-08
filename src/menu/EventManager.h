#pragma once
#include <calico/gba/keypad.h>
#include <calico/nds/touch.h>
#include <cstdint>
#include <nds/arm9/sprite.h>
#include <nds/touch.h>

#include <menu/Event.h>


// Generate events of key/touch states
class EventManager
{
public:
    using OnEventCallback = bool (*)(const Event event);

    
    EventManager(const OnEventCallback callback);

    // Should be called once every frame
    void Tick();

private:
    static constexpr uint16_t KEY_COUNT = KEY_DEBUG; // KEY_DEBUG is the last key

    struct TouchPos
    {
        Event::TouchPos posPx;
        Event::TouchPos posRaw;
    };

    struct RawData
    {
        Event::KeyState keys;

        union
        {
            TouchData data;
            TouchPos pos;
        } touch;

        bool touchDown;
    };

    
    RawData m_lastRawData;
    TouchPos m_lastTouchPos;

    OnEventCallback m_callback;
};
