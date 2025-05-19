#pragma once
 
namespace dae
{

    class ISoundHandle
    {
    public:
        virtual ~ISoundHandle() = default;
    };

    template<typename T>
    class SoundHandle : public ISoundHandle
    {
    public:
        explicit SoundHandle(std::shared_ptr<T> sound) : m_sound(std::move(sound)) {}
        std::shared_ptr<T> Get() const { return m_sound; }

    private:
        std::shared_ptr<T> m_sound;
    };

}