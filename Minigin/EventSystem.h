#pragma once
#include "Singleton.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include "Event.h"

namespace dae
{
    //same as Observer
    class IEventListener
    {
    public:
        virtual ~IEventListener() = default;

        //const GameObject& entity, const Event& event
        virtual void OnNotify(const GameObject& entity, const Event& event) = 0;
    };

    class EventSystem final : public Singleton<EventSystem>
    {
    public:

        struct ListenerEntry
        {
            GameObject* target; // nullptr = global listener
            IEventListener* listener;
        };

 
        void AddListener(const Event& eventName, GameObject* target, IEventListener* listener)
        {
            m_Listeners[eventName].push_back({ target, listener });
        }

        void RemoveListener(IEventListener* observer)
        {
            for (auto& [name, vec] : m_Listeners)
            {
                vec.erase(
                    std::remove_if(vec.begin(), vec.end(),
                        [observer](const ListenerEntry& entry) { return entry.listener == observer; }),
                    vec.end()
                );
            }
        }

        void TriggerEvent(const Event& event, const GameObject& sender)
        {
            auto it = m_Listeners.find(event);
            if (it == m_Listeners.end()) return;

            for (auto& entry : it->second)
            {
                if (entry.target == nullptr || entry.target == &sender)
                {
                    entry.listener->OnNotify(sender, event);
                }
            }
        }

      

    private:
        friend class Singleton<EventSystem>;
        EventSystem() = default;
        std::unordered_map<Event, std::vector<ListenerEntry>> m_Listeners;
        //std::unordered_map<std::string, std::vector<IEventListener*>> m_Listeners;
    };
}
