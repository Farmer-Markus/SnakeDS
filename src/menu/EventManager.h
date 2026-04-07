#pragma once
#include <calico/nds/touch.h>
#include <cstdint>
#include <nds/arm9/sprite.h>

#include <menu/Event.h>

typedef void (*OnEventCallback)(const Event event);


// Generate events of key/touch states
class EventManager
{
    struct RawData
    {
        uint32_t currKeys;

        union
        {
            TouchData data;

            struct
            {
                TouchPos posPx;
                TouchPos posRaw;
            } pos;
        } touch;

        bool touchDown;
    };

public:
    EventManager(const OnEventCallback callback);

    // Should be called once every frame
    void Tick();

private:

    RawData m_lastData;

    OnEventCallback m_callback;
};
