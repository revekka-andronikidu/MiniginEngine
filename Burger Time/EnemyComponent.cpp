#include "EnemyComponent.h"
#include <GameObject.h>
#include "SpriteSheetComponent.h"

using namespace dae;

EnemyComponent::EnemyComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
	, m_IsMoving(false)
	, m_Points(100)
	, m_MoveSpeed(45.f * GameSettings::scale.x)
	, m_CurrentDirection(Direction::Down)
	, m_pGrid(nullptr)
{
	EventManager::GetInstance().AddListener<CollisionEvent>(pOwner, this);
	
}

void EnemyComponent::OnNotify(const GameObject& entity, const BaseEvent& event)
{
	if (auto collision = dynamic_cast<const CollisionEvent*>(&event))
	{
		const GameObject& other = collision->other;
		if (other.GetTag() == Tag::BURGER)
		{
			//if burger is falling
			// 
			//if burger above squashed enemy

			//if bellow drop enemy with the burger (check how many enemies on the burger piece

			//m_IsMoving = true;
			//ServiceLocator::GetAudioService().StopAllSounds();
			//ServiceLocator::GetAudioService().PlayEffect(SoundID::EnemyDeath.id, 0.8f, false);

			//EventManager::GetInstance().TriggerEvent(GameEvent::ENEMY_DEFEATED, *GetOwner());
		}
		else if (other.GetTag() == Tag::PEPPER)
		{
			//stop the movement
			//stop collision??
			//play animation
			auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
			if (!sprite) return;

		
					sprite->SetAnimation("Peppered");
					
				sprite->SetAnimate(true);
		}
	}
}