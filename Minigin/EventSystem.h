#pragma once
#include "Singleton.h"
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Event.h"

namespace dae
{
    //same as Observer
    class IEventListener
    {
    public:
        virtual ~IEventListener() = default;
        virtual void OnNotify(const GameObject& entity, const BaseEvent& event) = 0;
    };

    class EventSystem final : public Singleton<EventSystem>
    {
    public:
        static bool IsAlive() { return s_Alive; }

        struct ListenerEntry
        {
            GameObject* target; // nullptr = global listener
            IEventListener* listener;
        };

        template<typename EventType>
        void AddListener(GameObject* target, IEventListener* listener)
        {
            m_Listeners[std::type_index(typeid(EventType))].push_back({ target, listener });
        }

        template<typename EventType>
        void AddGlobalListener(IEventListener* listener)
        {
            m_Listeners[std::type_index(typeid(EventType))]
                .push_back({ nullptr, listener });
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

        void TriggerEvent(const BaseEvent& event, const GameObject& sender)
        {
            auto it = m_Listeners.find(std::type_index(typeid(event)));
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
        EventSystem() { s_Alive = true; }
        ~EventSystem() { s_Alive = false; }
        static inline bool s_Alive = false;
        std::unordered_map<std::type_index, std::vector<ListenerEntry>> m_Listeners;
    };
}
