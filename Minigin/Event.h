#pragma once
#include <typeindex>
#include <memory>

namespace dae
{
    class GameObject;

    // Base for all events — must be polymorphic for dynamic_cast
    class BaseEvent
    {
    public:
        virtual ~BaseEvent() = default;
    };

    // Your event alias (optional)
    using EventPtr = std::shared_ptr<const BaseEvent>;

    // Example collision event
    struct CollisionEvent final : public BaseEvent
    {
        const GameObject& other; // the object this one collided with
        CollisionEvent(const GameObject& otherObj) : other(otherObj) {}
    };
}