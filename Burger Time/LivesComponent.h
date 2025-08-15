#pragma once
#include "BaseComponent.h"
#include <EventManager.h>	
#include "GameEvents.h"

namespace dae
{
	class LivesComponent : public BaseComponent
	{
	public:
		LivesComponent(GameObject* owner, int health = 3) : BaseComponent(owner)
			, m_CurrentLives(health)
		{
		};

		~LivesComponent() = default;
		void Update() override {};
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) = delete;

		int GetLives() const { return m_CurrentLives; } ;

		void SetLives(int lives)
		{
			if (lives <= 0)
			{
				EventManager::GetInstance().TriggerEvent(GameEvent::PLAYER_DEATH, *GetOwner());
			}
			else
			{
				m_CurrentLives = lives;
				auto event = std::make_shared<dae::LivesUpdatedEvent>(m_CurrentLives);
				EventManager::GetInstance().TriggerEvent(*event, *GetOwner());

			}
		};
		
		void RemoveLife() { SetLives(m_CurrentLives - 1); };

	private:
		int m_CurrentLives;

	};
}

