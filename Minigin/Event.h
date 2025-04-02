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
        virtual std::type_index GetType() const = 0;
    };

    using Event = std::shared_ptr<const BaseEvent>;

    template<typename T>
    class TypedEvent : public BaseEvent
    {
    public:
        explicit TypedEvent(const T& data) : m_data(data) {}
        std::type_index GetType() const override { return typeid(T); }
        const T& GetData() const { return m_data; }

    private:
        T m_data;
    };
}