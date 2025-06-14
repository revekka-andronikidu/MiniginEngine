#pragma once
#include "BaseComponent.h"
#include "Subject.h"
#include "GameEvents.h"

namespace dae
{
	class LivesComponent : public BaseComponent, public Subject
	{
	public:
		LivesComponent(GameObject* owner, int health) : BaseComponent(owner)
			, Subject()
			, m_CurrentLives(health)
		{
		};

		LivesComponent(GameObject* owner) : BaseComponent(owner)
			, Subject()
			, m_CurrentLives{}
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
				Notify(*GetOwner(), OBJECT_DEATH_EVENT);
			}
			else
			{
				m_CurrentLives = lives;
				Notify(*GetOwner(), LIVES_UPDATED_EVENT);

			}
		};
		
		void RemoveLive() { SetLives(m_CurrentLives - 1); };

	private:
		int m_CurrentLives;

	};
}

