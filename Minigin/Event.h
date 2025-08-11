#pragma once
#include <typeindex>
#include <memory>

namespace dae
{
    class GameObject;

    class BaseEvent 
    {
    public:
        virtual ~BaseEvent() = default;
    };

    using Event = std::shared_ptr<const BaseEvent>;

}