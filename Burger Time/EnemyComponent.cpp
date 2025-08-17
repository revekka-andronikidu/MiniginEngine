#include "EnemyComponent.h"
#include <GameObject.h>
#include "SpriteSheetComponent.h"
#include <TimeManager.h>
#include <ColliderComponent.h>
#include <ServiceLocator.h>
#include "GameEvents.h"
#include "GridComponent.h"
#include <SceneManager.h>
#include <Scene.h>
using namespace dae;

EnemyComponent::EnemyComponent(GameObject* pOwner, int points, EnemyType type)
	: BaseComponent(pOwner)
	, m_CurrentDirection(Direction::None)
	, m_pGrid(nullptr)
	, m_Points{points}
	, m_Type{ type }
{
	EventManager::GetInstance().AddListener<CollisionEvent>(pOwner, this);

	auto& transform = GetOwner()->GetTransform();
	auto cellSize = GameSettings::cellSize * GameSettings::scale.x;

	// get world position relative to the level
	glm::vec3 pos = transform.GetWorldPosition();
	m_SpawnPosition = pos;
	

	// Offset if on left half of the screen
	if (pos.x < (GameSettings::windowWidth * GameSettings::scale.x) / 2.0f)
	{
		pos.x -= 2 * cellSize; // move 2 cells to the left
		m_CurrentDirection = Direction::Right; // start moving right
	}
	else
	{
		pos.x += 2 * cellSize;
		m_CurrentDirection = Direction::Left;  // default: move left
	}

	transform.SetPosition(pos);	
}

void EnemyComponent::OnNotify(const GameObject& entity, const BaseEvent& event)
{
	if (auto collision = dynamic_cast<const CollisionEvent*>(&event))
	{
		const GameObject& other = collision->other;
		if (other.GetTag() == Tag::PEPPER)
		{
			Pepper();
		}
	}
}

void EnemyComponent::Update()
{
	if (!m_HasReachedSpawn)
	{
		WalkIn();
		Animate();
		return;
	}
	if (m_IsPeppered && !m_IsRegisteredToBurger)
	{
		m_PepperedTimer += TimeManager::GetInstance().GetDeltaTime();
		if (m_PepperedTimer >= m_PepperedDuration)
		{
			UnPepper();
		}
	}
	if (m_IsDead)
	{
		auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
		if (!sprite) return;

		if (sprite->IsAnimationFinished())
		{
			EventManager::GetInstance().TriggerEvent(GameEvent::ENEMY_DEFEATED, *GetOwner());
			GetOwner()->Destroy();
		}
	}
	if (m_CanMove)
	{
		if (MoveTowardsTarget(m_TargetPosition))
		{
			//m_CanMove = false;
			//SetNewTarget();
		}
	}
	Animate();
}

std::vector<glm::vec3> EnemyComponent::CalculatePath()
{


	auto players = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag(Tag::PLAYER);
	//if players > 1 find the closest player and return player

	auto player = players[0];
	auto playerPos = player->GetTransform().GetLocalPosition();
	playerPos.x += GameSettings::cellSize * GameSettings::scale.x / 2; // center the player position
	playerPos.y += GameSettings::cellSize * GameSettings::scale.y; // feet pos
	auto target = m_pGrid->WorldToCellPos(playerPos);

	auto currentPos = GetOwner()->GetTransform().GetLocalPosition();
	currentPos.x += GameSettings::cellSize * GameSettings::scale.x / 2; // center the enemy position
	currentPos.y += GameSettings::cellSize * GameSettings::scale.y;// to get feet
	auto startCell = m_pGrid->WorldToCellPos(currentPos);

	auto path = m_pGrid->FindShortestPath(startCell, target);
	auto worldPath = m_pGrid->ConvertPathToWorldPositions(path);

	for (const auto& cell : worldPath)
	{
		std::cout << "Cell: " << cell.x << ", " << cell.y << std::endl;
	}

	return worldPath;
}

void EnemyComponent::Activate()
{
	m_CanMove = true;
	m_NewPath = CalculatePath();
	m_TargetPosition = m_NewPath[1]; // Set the first target position
}


void EnemyComponent::Pepper()
{
	m_CanMove = false;
	m_IsPeppered = true;
	m_PepperedTimer = 0.f;
	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (!sprite) return;
}
void EnemyComponent::UnPepper()
{
	m_IsPeppered = false;
	m_CanMove = true;
}

void EnemyComponent::Squish()
{
	if (m_IsSquished) return;

	m_IsSquished = true;
	GetOwner()->GetComponent<ColliderComponent>()->m_IsActive = false;
	ServiceLocator::GetAudioService().PlayEffect(SoundID::EnemySqiushed.id, 0.8f, false);
	EventManager::GetInstance().TriggerEvent(PointsIncreasedEvent{ m_Points }, *GetOwner());

	Die();
}

void EnemyComponent::Die()
{
	m_IsDead = true;
	m_CanMove = false;
}

void EnemyComponent::RegisterToIngredient(GameObject* burger)
{
	m_IsRegisteredToBurger = true;
	m_CanMove = false;
	GetOwner()->SetParent(burger, true);
}

void EnemyComponent::Animate()
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

void EnemyComponent::WalkIn()
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
		Activate();
	}
}

bool EnemyComponent::MoveTowardsTarget(glm::vec3 targetPos)
{
	auto pos = GetOwner()->GetTransform().GetLocalPosition();  //CENTER THIS
	glm::vec3 newPos = pos;

	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);
	float move = m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();

	bool targetReached = false;

	auto target = targetPos;
	target.x -= cellSize / 2; // deCenter the target position	
	target.y -= cellSize / 2; // deCenter the target position
	target.y -= 3  * GameSettings::scale.y; //feet 
	std::cout << "Moving towards target: " << target.x << ", " << target.y << std::endl;
	std::cout << "Current position: " << pos.x << ", " << pos.y << std::endl;


	// Move horizontally
	if (pos.x < target.x)
	{
		newPos.x += move;
		if (newPos.x >= target.x)
		{
			newPos.x = target.x;
			targetReached = true; // Target reached
		}
		m_CurrentDirection = Direction::Right;
	}
	else if (pos.x > target.x)
	{
		newPos.x -= move;
		if (newPos.x <= target.x)
		{
			newPos.x = target.x;
			targetReached = true; // Target reached
		}
		m_CurrentDirection = Direction::Left;
	}
	else if (pos.y < target.y)
	{
		newPos.y += move;
		if (newPos.y >= target.y)
		{
			newPos.y = target.y;
			targetReached = true; // Target reached
		}
		m_CurrentDirection = Direction::Down;
	}
	else if (pos.y > target.y)
	{
		newPos.y -= move;
		if (newPos.y <= target.y)
		{
			newPos.y = target.y;
			targetReached = true; // Target reached
		}
		m_CurrentDirection = Direction::Up;
	}

	GetOwner()->GetTransform().SetPosition(newPos);

	if (targetReached)
	{
		return true;
	}
	return false;
}