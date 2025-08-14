#pragma once
#include "BaseComponent.h"
#include <vector>
#include <glm.hpp>
#include "Helpers.h"

namespace dae
{
    class TextComponent;
	class KeypadComponent final : public BaseComponent
	{
	public:
		KeypadComponent(GameObject* pOwner);
        void HandleMove(Direction dir);
        void Select();


        void Update() override;
        void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

        void AddMenuArrow(std::shared_ptr<dae::GameObject> object)
        {
            m_menuArrowIcon = object;
            SetMenuArrowPosition();
        };

        void SetTextComponent(TextComponent* component)
        {
            m_pTextComponent = component;
        }

    private:
        std::shared_ptr<dae::GameObject> m_menuArrowIcon{};
        TextComponent* m_pTextComponent{};
		bool m_showCursor{ true };
        bool m_isDirty{false};
		bool m_nameEntered{ false };
        int m_CursorX{ 0 };
        int m_CursorY{ 0 };
        int m_charLimit{5};
		float m_blinkTimer{ 0.f };
        float m_transitionTimer{0.f};
		float m_blinkInterval{ 0.5f }; // Blink every 0.5 seconds
        std::string m_name;
        glm::vec3 GetCursorWorldPos() const;
        std::vector<std::vector<char>> m_KeyGrid;
        
  
        void BuildKeyMap();
		void SetMenuArrowPosition();
	};

}
