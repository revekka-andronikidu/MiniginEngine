#pragma once
#include <BaseComponent.h>

namespace dae
{
	class TextComponent;
	class AnimatedTextComponent final : public BaseComponent
    {
    public:
        AnimatedTextComponent(GameObject* pOwner, TextComponent* pText, std::string fullText, float interval = 0.5f);
        
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        void Restart()
        {
            m_index = 0;
            m_timer = 0.f;
            m_displayed.clear();
            m_active = true;
        }

        void Update() override;
        void SetText(const std::string& text) { m_fullText = text; }

    private:
        dae::TextComponent* m_pText{ nullptr };
        std::string m_fullText;
        std::string m_displayed;
        size_t m_index{ 0 };
        float m_timer{ 0.f };
        float m_interval{ 0.5f }; // seconds per letter
        bool m_active{ false };
    };

}