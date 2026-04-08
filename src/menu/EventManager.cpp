#include <calico/nds/touch.h>
#include <nds/arm9/input.h>
#include <nds/arm9/sprite.h>
#include <vector>

#include <menu/Event.h>
#include <menu/EventManager.h>


EventManager::EventManager(const OnEventCallback callback) : m_callback(callback)
{
    m_lastRawData = RawData{};
}

void EventManager::Tick()
{
    // Read in newest touch/key data
    RawData data;
    data.touchDown = touchRead(&data.touch.data);
    data.keys = keysCurrent();

    // Generate events
    std::vector<Event> eventQue;
    eventQue.reserve(KEY_COUNT);

    // Touchpos gets set to 0 when finger goes up
    // but we always want touch position
    if(!data.touchDown)
        data.touch.pos = m_lastTouchPos;
    else // Save new touch pos for next event
        m_lastTouchPos = data.touch.pos;

    // First check touch down/up event
    if(data.touchDown != m_lastRawData.touchDown)
    {
        Event ev{};
        ev.touchPos = data.touch.pos.posPx;

        if(data.touchDown)
            ev.type = Event::Type::TOUCH_DOWN;
        else
            ev.type = Event::Type::TOUCH_UP;

        eventQue.push_back(ev);
    }

    // Then touch move event
    if(data.touch.pos.posPx != m_lastRawData.touch.pos.posPx)
    {
        Event ev{};
        ev.type = Event::Type::TOUCH_MOVE;
        ev.touchPos = data.touch.pos.posPx;
        eventQue.push_back(ev);
    }

    // Now key events
    // Get all events that have changed since last tick
    Event::KeyState kChanged = data.keys ^ m_lastRawData.keys;
    Event::KeyState keyMask;
    for(Event::KeyState i = 0; i < KEY_COUNT; i++)
    {
        keyMask = 1 << i;
        // if key changed state
        if(kChanged & keyMask)
        {
            // Generate event with current state
            Event ev{};
            ev.type = (data.keys & keyMask) ? Event::Type::BUTTON_DOWN : Event::Type::BUTTON_UP;
            ev.key = keyMask;
            eventQue.push_back(ev);
        }
    }

    // Update last data for next tick
    m_lastRawData = data;

    // Trigger events
    for(Event ev : eventQue)
        m_callback(ev);
}
