#include "PepperComponent.h"
#include "Helpers.h"
#include <GameObject.h>
#include <SpriteSheetComponent.h>
#include <ServiceLocator.h>
#include <ColliderComponent.h>
#include <TimeManager.h>

using namespace dae;

PepperComponent::PepperComponent(GameObject* pOwner, Direction direction) : BaseComponent(pOwner)
, m_Direction{ direction }

{
	SetAnimation();
	ServiceLocator::GetAudioService().PlayEffect(SoundID::PepperShake.id, 0.8f, false); //move this to pepper component
}


void PepperComponent::Update() 
{ 
	if (m_Duration <= 0.f)
	{
		GetOwner()->GetComponent<ColliderComponent>()->m_IsActive = false; // Disable collider after duration
	}
	m_Duration -= TimeManager::GetInstance().GetDeltaTime();


	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (!sprite) return;

	if (sprite->IsAnimationFinished())
	{
		GetOwner()->Destroy();
	}
}

void PepperComponent::SetAnimation()
{
	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (!sprite) return;

	switch (m_Direction)
	{
	case Direction::Up:    sprite->SetAnimation("Up"); break;
	case Direction::Down:  sprite->SetAnimation("Down"); break;
	case Direction::Left:  sprite->SetAnimation("Right", true); break;
	case Direction::Right: sprite->SetAnimation("Right"); break;

	default: sprite->SetAnimate(false); return;
	}

	sprite->SetAnimate(true);
}