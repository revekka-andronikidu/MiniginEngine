#include "EnemyPlayerComponent.h"
#include <GameObject.h>
#include <ColliderComponent.h>
#include "GameEvents.h"
#include <ServiceLocator.h>
#include <SpriteSheetComponent.h>
#include <TimeManager.h>
#include "GridComponent.h"
#include "Helpers.h"



using namespace dae;

dae::EnemyPlayerComponent::EnemyPlayerComponent(GameObject* owner) : BaseComponent(owner)
{
	//EventManager::GetInstance().AddListener<CollisionEvent>(owner, this);
	EventManager::GetInstance().AddGlobalListener<LevelCompleteEvent>(this);

	auto& transform = GetOwner()->GetTransform();
	auto cellSize = GameSettings::cellSize * GameSettings::scale.x;

	// get world position relative to the level
	glm::vec3 pos = transform.GetWorldPosition();
	m_SpawnPosition = pos;


	// Offset if on left half of the screen

		pos.y -= 4 * cellSize;
		m_CurrentDirection = Direction::Down;  // default: move left
	

	transform.SetPosition(pos);
}


dae::EnemyPlayerComponent::~EnemyPlayerComponent()
{
	if (EventManager::IsAlive())
	{
		EventManager::GetInstance().RemoveListener(this);
	}
}

void EnemyPlayerComponent::Update()
{
	Animate();
	m_IsMoving = false;
}

void EnemyPlayerComponent::OnNotify(const GameObject& entity, const BaseEvent& event)
{
	if (auto collision = dynamic_cast<const LevelCompleteEvent*>(&event))
	{
		m_CurrentDirection = Direction::None;
		StopMovement();

	}
}
void EnemyPlayerComponent::Move(Direction dir)
{
	if (!m_pGrid || !m_CanMove)
		return;

	m_CurrentDirection = dir;
	m_IsMoving = true;

	const int gridWidth = m_pGrid->GetWidth();;
	const int cellSize{ static_cast<int>(GameSettings::cellSize * GameSettings::scale.x) };


	int m_LeftBorder{ 0 };
	int m_RightBorder{ cellSize * gridWidth };


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
			newPos.y = static_cast<float>(cellY * (cellSize)-3 * GameSettings::scale.y);
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
			newPos.y = static_cast<float>(cellY * (cellSize)-3 * GameSettings::scale.y);
		}
	}
	break;
	case Direction::Down:
	{
		isOnLadder = m_pGrid->CanClimb(Direction::Down, static_cast<int>(pos.x + cellSize / 2), playerFeetY);
		if (isOnLadder)
		{
			float moveY = m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();
			newPos.y += moveY;

			newPos.x = m_pGrid->GetLadderCenterX(static_cast<int>(pos.x + cellSize / 2), pos.y);
		}
	}
	break;

	case Direction::Up:
	{
		isOnLadder = m_pGrid->CanClimb(Direction::Up, pos.x + cellSize / 2, playerFeetY);
		if (isOnLadder)
		{
			float moveY = -m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();
			newPos.y += moveY;

			newPos.x = m_pGrid->GetLadderCenterX(pos.x + cellSize / 2, pos.y);
		}
	}
	break;
	}
	GetOwner()->GetTransform().SetPosition(newPos);
}


void EnemyPlayerComponent::Squish()
{
	if (m_IsSquished) return;

	m_IsSquished = true;
	GetOwner()->GetComponent<ColliderComponent>()->m_IsActive = false;
	ServiceLocator::GetAudioService().PlayEffect(SoundID::EnemySqiushed.id, 0.8f, false);
	EventManager::GetInstance().TriggerEvent(PointsIncreasedEvent{ m_Points }, *GetOwner());

	Die();
}

void EnemyPlayerComponent::Die()
{
	m_IsDead = true;
	m_CanMove = false;
}

void EnemyPlayerComponent::RegisterToIngredient(GameObject* burger)
{
	m_IsRegisteredToBurger = true;
	m_CanMove = false;
	GetOwner()->SetParent(burger, true);
}

void EnemyPlayerComponent::ResetPlayer(glm::vec3 pos)
{
	m_IsDead = false;
	m_CanMove = true;
	m_CurrentDirection = Direction::Down;
	GetOwner()->GetTransform().SetPosition(pos);
}

void EnemyPlayerComponent::Animate()
{
	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (!sprite) return;

	if (m_IsDead)
	{
		sprite->SetAnimation("Death");
		sprite->SetAnimate(true);
		return;
	}
	else if (m_IsPeppered)
	{
		sprite->SetAnimation("Peppered");
		sprite->SetAnimate(true);
		return;
	}
	else if (m_CanMove || !m_HasReachedSpawn)
	{
		sprite->SetAnimate(true);
		switch (m_CurrentDirection)
		{
		case Direction::Up:    sprite->SetAnimation("Up"); break;
		case Direction::Down:  sprite->SetAnimation("Down"); break;
		case Direction::Left:  sprite->SetAnimation("Left"); break;
		case Direction::Right: sprite->SetAnimation("Left", true); break;
		default: sprite->SetAnimate(false); return;
		}
	}
	else if (m_IsRegisteredToBurger)
	{
		sprite->SetAnimate(false);
	}
}

void EnemyPlayerComponent::WalkIn()
{
	auto pos = GetOwner()->GetTransform().GetLocalPosition();
	glm::vec3 newPos = pos;
	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);

	float move = m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();


	switch (m_CurrentDirection)
	{
	case Direction::Left:
	{
		newPos.x -= move;
		if (newPos.x <= m_SpawnPosition.x)
		{
			newPos.x = m_SpawnPosition.x;
			m_HasReachedSpawn = true;
		}
	}
	break;
	case Direction::Right:
	{
		newPos.x += move;
		if (newPos.x >= m_SpawnPosition.x)
		{
			newPos.x = m_SpawnPosition.x;
			m_HasReachedSpawn = true;
		}
	}
	break;
	}
	GetOwner()->GetTransform().SetPosition(newPos);
	if (m_HasReachedSpawn)
	{
		m_CanMove = true;
	}
}