#pragma once
#include "PlayerComponent.h"
#include <algorithm>
#include <glm.hpp>
#include <GameObject.h>
#include <TimeManager.h>
#include <SpriteSheetComponent.h>
#include "SceneManager.h"
#include "GridComponent.h"
#include "Scene.h"
#include "LivesComponent.h"
#include "ServiceLocator.h"
#include "BurgerTimeGame.h"
#include "GameState.h"
#include "ObjectFactory.h"

using namespace dae;

dae::PlayerComponent::PlayerComponent(GameObject* owner) : BaseComponent(owner)
{
	EventManager::GetInstance().AddListener<CollisionEvent>(owner, this);
	EventManager::GetInstance().AddGlobalListener<LevelCompleteEvent>(this);
}

dae::PlayerComponent::~PlayerComponent()
{
	if (EventManager::IsAlive())
	{
		EventManager::GetInstance().RemoveListener(this);
	}
}
void dae::PlayerComponent::Update()
{
	Animate();
	m_TimeSinceLastPepper += TimeManager::GetInstance().GetDeltaTime();
	m_IsMoving = false;
}


void PlayerComponent::Move(Direction dir)
{
	if (!m_pGrid || !m_CanMove)
		return;

	m_CurrentDirection = dir;
	m_IsMoving = true;

	const int gridWidth = m_pGrid->GetWidth();;
	const int cellSize{ static_cast<int>(GameSettings::cellSize * GameSettings::scale.x) };
	

	int m_LeftBorder{0};
	int m_RightBorder{cellSize * gridWidth};


	auto pos = GetOwner()->GetTransform().GetLocalPosition();
	int playerFeetY = static_cast<int>(pos.y + cellSize); // bottom of player

	bool isOnPlatform;
	bool isOnLadder;

	glm::vec3 newPos = pos;

	switch (dir)
	{
	case Direction::Left:
	{
		isOnPlatform = m_pGrid->IsOnPlatform(pos.x, playerFeetY);
		if (isOnPlatform)
		{
			float moveX = -m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();
			newPos.x += moveX;

			// Clamp to level boundaries
			newPos.x = std::clamp(newPos.x, static_cast<float>(m_LeftBorder), static_cast<float>(m_RightBorder));

			// Align player feet with platform top
			int cellY = static_cast<int>(newPos.y + cellSize) / (cellSize);
			newPos.y = static_cast<float>(cellY * (cellSize) - 3 * GameSettings::scale.y);
		}
	}
		break;
	

	case Direction::Right:
	{
		//const GridCell& currentCell2 = pGrid->PositionToCell(pos.x + 48, playerFeetY);
		isOnPlatform = isOnPlatform = m_pGrid->IsOnPlatform(pos.x + cellSize, playerFeetY);
		if (isOnPlatform)
		{
			float moveX = m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();
			newPos.x += moveX;
			// Clamp to level boundaries
			newPos.x = std::clamp(newPos.x, static_cast<float>(m_LeftBorder), static_cast<float>(m_RightBorder));

			// Align player feet with platform top
			int cellY = static_cast<int>(newPos.y + cellSize) / cellSize;
			newPos.y = static_cast<float>(cellY * (cellSize) -  3 * GameSettings::scale.y);
		}
	}
		break;
	case Direction::Down:
	{
		isOnLadder = m_pGrid->CanClimb(Direction::Down, static_cast<int>(pos.x + cellSize/2), playerFeetY);
		if (isOnLadder)
		{
			float moveY = m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();
			newPos.y += moveY;

			newPos.x = m_pGrid->GetLadderCenterX(static_cast<int>(pos.x + cellSize/2), pos.y);
		}
	}
	break;

	case Direction::Up:
	{
		isOnLadder = m_pGrid->CanClimb(Direction::Up, pos.x + cellSize / 2, playerFeetY);
		if (isOnLadder)
		{
			float moveY =  -m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();
			newPos.y += moveY;

			newPos.x = m_pGrid->GetLadderCenterX(pos.x +cellSize/2, pos.y);
		}
	}
	break;
	}
	GetOwner()->GetTransform().SetPosition(newPos);
}

void PlayerComponent::OnNotify(const GameObject& entity, const BaseEvent& event)
{

	if (auto collision = dynamic_cast<const CollisionEvent*>(&event))
	{
		const GameObject& other = collision->other;
		if (other.GetTag() == Tag::ENEMY)
		{
			if (m_IsDead) return; // already dead, ignore further collisions
			// Death animation
			//ENEMY MANAGER RESTART LEVEL
			//std::cout << "Player collided with enemy!" << std::endl;
			m_IsDead = true;
			m_CanMove = false;

			ServiceLocator::GetAudioService().StopAllSounds();
			ServiceLocator::GetAudioService().PlayEffect(SoundID::Death.id, 0.8f, false);
			GetOwner()->GetComponent<LivesComponent>()->RemoveLife();
		}
	}

	if (auto collision = dynamic_cast<const LevelCompleteEvent*>(&event))
	{
		//auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
		//sprite->SetAnimation("Celebration");
		//sprite->SetAnimate(true);
		m_CurrentDirection = Direction::None;
		StopMovement();

		
		//WinAnimation();
	}
}

void PlayerComponent::ResetPlayer(glm::vec3 pos)
{
		m_IsDead = false;
		m_AnimSet = false;
		m_CanMove = true;
		m_CurrentDirection = Direction::Down;
		GetOwner()->GetTransform().SetPosition(pos);
	
	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (sprite)
	{
		sprite->SetAnimation("Idle");
		sprite->SetAnimate(false);
	}
}


void PlayerComponent::Animate()
{
	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (!sprite) return;
	
	if (m_IsDead)
	{

		if (!m_AnimSet)
		{
			sprite->SetAnimation("Death", false, "Dying");
			m_AnimSet = true;
		}
		sprite->SetAnimate(true);
		
		// Reset dead state after animation starts
		return;
	}

	if (!m_IsMoving && m_CanMove)
	{
		sprite->SetAnimate(false);
		return;
	}

	switch (m_CurrentDirection)
	{
	case Direction::Up:    sprite->SetAnimation("Up"); break;
	case Direction::Down:  sprite->SetAnimation("Down"); break;
	case Direction::Left:  sprite->SetAnimation("Left"); break;
	case Direction::Right: sprite->SetAnimation("Left", true); break;
	case Direction::None: sprite->SetAnimation("Celebration"); break;
	default: sprite->SetAnimate(false); return;
	}

	sprite->SetAnimate(true);

}

void PlayerComponent::Pepper()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	auto burgerTime = dynamic_cast<BurgerTimeGame*>(game);

	if (burgerTime->m_Peppers > 0)
	{
		if (m_TimeSinceLastPepper >= m_PepperCooldown)
		{
			burgerTime->m_Peppers--;
			SpawnPepper();
			m_TimeSinceLastPepper = 0.0f;
			EventManager::GetInstance().TriggerEvent(GameEvent::PEPPERS_CHANGED, *GetOwner());
		}
	}
	else
	{
#if DEBUG
		std::cout << "No Peppers left!" << std::endl;
#endif
	}
}

void PlayerComponent::SpawnPepper() 
{

	const auto playerPosition = GetOwner()->GetTransform().GetWorldPosition();


	auto spawnPosition = glm::vec3{ playerPosition.x, playerPosition.y, playerPosition.z };

	switch (m_CurrentDirection)
	{
	case Direction::Left:
		spawnPosition.x -= GameSettings::cellSize * GameSettings::scale.x;
		break;
	case Direction::Right:	
		spawnPosition.x += GameSettings::cellSize * GameSettings::scale.x;
		break;
	case Direction::Up:
		spawnPosition.y -= GameSettings::cellSize * GameSettings::scale.y;
		break;
	case Direction::Down:
		spawnPosition.y += GameSettings::cellSize * GameSettings::scale.y;
		break;
	}
	

	auto pepper = ObjectFactory::GetInstance().CreatePepper(m_CurrentDirection, spawnPosition);

	SceneManager::GetInstance().GetActiveScene().Add(std::move(pepper));
}
