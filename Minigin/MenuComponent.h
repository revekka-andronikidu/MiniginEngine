#pragma once
#include "BaseComponent.h"
#include <vector>
#include <iostream>

namespace dae
{
	class MenuComponent final : public BaseComponent
	{
	public:
		enum class Direction { Up, Down };

		MenuComponent(GameObject* pOwner) : BaseComponent(pOwner) {};
		~MenuComponent() = default;

		void AddMenuItem(std::shared_ptr<dae::GameObject> item) { m_menuItems.push_back(item); };
		void ChangeMenuItem(Direction direction);
		void AddMenuArrow(std::shared_ptr<dae::GameObject> object) 
		{ 
			m_menuArrowIcon = object; 
			m_dirtyFlag = true;
			void SetMenuArrowPosition();
		};

		void EnterMenuItem();

		void Update() override;
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		

	private:
		std::vector <std::shared_ptr<dae::GameObject>> m_menuItems{};
		int m_currentItem{ 0 };
		std::shared_ptr<dae::GameObject> m_menuArrowIcon; //? pass the whole game obj
		
		bool m_dirtyFlag{false};

		void SetMenuArrowPosition();

	};
}

