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

EnemyComponent::~EnemyComponent()
{
	if (EventManager::IsAlive())
	{
		EventManager::GetInstance().RemoveListener(this);
	}
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
			SetNewTarget();
		}
	}
	Animate();
}

void EnemyComponent::SetNewTarget()
{
	if (!m_pGrid) return;

	if (m_CurrentPathPoint >= m_CurrentPath.size() - 1)
	{
		m_CalculatePath = true; // if we reached end of path, recalculate next frame
		//std::cout << "Reached end of path, recalculating" << std::endl;
	}
	if (m_CurrentPathPoint >= 4)
	{
		m_CalculatePath = true;
	}
	if (m_CalculatePath)
	{
		m_NewPath = CalculatePath();
		if (!m_NewPath.size() <= 1)
		{

			if (IsValidNextTarget(m_CurrentDirection, m_CurrentPath[m_CurrentPathPoint], m_NewPath[1]))
			{
				m_CurrentPath = m_NewPath;
				m_CurrentPathPoint = 1; // reset to start of new path
				m_TargetPosition = m_CurrentPath[m_CurrentPathPoint];
				m_CalculatePath = false;
				//std::cout << "New path calculated, starting at point: " << m_CurrentPathPoint << std::endl;
				return;
			}
		
		}
		if (m_CurrentPathPoint < m_CurrentPath.size() - 1)
		{
			m_CurrentPathPoint++;
			m_TargetPosition = m_CurrentPath[m_CurrentPathPoint];
			//std::cout << "Continuing in same direction, but recalculating path" << std::endl;
			return;
		}
		else
		{

		}
	}
	
	//Check if we can change path and climb
	if (m_CurrentPathPoint + 1 < m_CurrentPath.size() && IsSameDirection(m_CurrentDirection, m_CurrentPath[m_CurrentPathPoint], m_CurrentPath[m_CurrentPathPoint+1]))
	{
		//if (m_CurrentDirection == Direction::Left || m_CurrentDirection == Direction::Right)
		//{
		//	glm::vec3 enemy = GetAnchorPosition();
		//	glm::vec3 player = m_CurrentPath.back(); //maybe check for new path?
		//	float cellSize = GameSettings::cellSize* GameSettings::scale.y;
		//	Direction preferredDir = (player.y > enemy.y + cellSize/2) ? Direction::Down : Direction::Up;
		//
		//	Direction altDir = (preferredDir == Direction::Down) ? Direction::Up : Direction::Down;
		//
		//	if (m_pGrid->CanClimb(preferredDir, enemy.x, enemy.y))
		//	{
		//		m_CurrentDirection = preferredDir;
		//		m_TargetPosition.y += (preferredDir == Direction::Down ? cellSize : -cellSize);
		//		m_CalculatePath = true;
		//		std::cout << "Climbing in  direction: " << static_cast<int>(preferredDir) << std::endl;
		//		return;
		//	}
		//	// Otherwise try the opposite
		//	else if (m_pGrid->CanClimb(altDir, enemy.x, enemy.y))
		//	{
		//		m_CurrentDirection = altDir;
		//		m_TargetPosition += (altDir == Direction::Down ? cellSize : -cellSize);
		//		m_CalculatePath = true;
		//		std::cout << "Climbing in direction: " << static_cast<int>(altDir) << std::endl;
		//		return;
		//	}
		//	else
		//	{
		//		m_CurrentPathPoint++;
		//		m_TargetPosition = m_CurrentPath[m_CurrentPathPoint]; // continue in same direction
		//		std::cout << "Continuing in same direction: " << static_cast<int>(m_CurrentDirection) << std::endl;
		//
		//		if (m_CurrentPathPoint >= m_CurrentPath.size() - 2)
		//		{
		//			m_CalculatePath = true; // if we reached end of path, recalculate next frame
		//			std::cout << "Reached end of path, recalculating next frame" << std::endl;
		//			return;
		//		}
		//	}
		//	
		//}
		//else //is climbing 
		{
			m_CurrentPathPoint++;
			m_TargetPosition = m_CurrentPath[m_CurrentPathPoint];
			//std::cout << "Continuing in same direction: " << static_cast<int>(m_CurrentDirection) << std::endl;
			return;
		}
	}
	else
	{
		m_CalculatePath = true;
		//std::cout << "Direction change detected, recalculating path" << std::endl;
	}
	
	

}

bool EnemyComponent::IsSameDirection(Direction currentDir, const glm::vec3& currentTarget, const glm::vec3& nextTarget) const
{
	glm::vec3 delta = nextTarget - currentTarget;

	switch (currentDir)
	{
	case Direction::Left:   return delta.x < 0 && fabs(delta.y) < fabs(delta.x);
	case Direction::Right:  return delta.x > 0 && fabs(delta.y) < fabs(delta.x);
	case Direction::Up:     return delta.y < 0 && fabs(delta.x) < fabs(delta.y);
	case Direction::Down:   return delta.y > 0 && fabs(delta.x) < fabs(delta.y);
	default:                return false;
	}
}

bool EnemyComponent::IsValidNextTarget(Direction currentDir, const glm::vec3& currentTarget, const glm::vec3& nextTarget) const
{
	glm::vec3 delta = nextTarget - currentTarget;

	// Horizontal
	if (delta.x > 0 && currentDir == Direction::Left)  return false; // would reverse
	if (delta.x < 0 && currentDir == Direction::Right) return false;

	// Vertical
	if (delta.y > 0 && currentDir == Direction::Up)    return false;
	if (delta.y < 0 && currentDir == Direction::Down)  return false;

	return true; // either same direction or neutral
}

glm::vec3 EnemyComponent::GetAnchorPosition() const
{
	glm::vec3 currentPos = GetOwner()->GetTransform().GetLocalPosition();
	currentPos.y += GameSettings::cellSize * GameSettings::scale.y; // feet pos
	currentPos.x += GameSettings::cellSize * GameSettings::scale.x / 2; // center the enemy position

	return currentPos;
}

std::vector<glm::vec3> EnemyComponent::CalculatePath()
{


	auto players = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag(Tag::PLAYER);
	//if players > 1 find the closest player and return player

	auto player = players[0];
	auto playerPos = player->GetTransform().GetLocalPosition();
	playerPos.x += (GameSettings::cellSize / 3) * GameSettings::scale.x; // center the player position shift to left for offsett ladderse
	playerPos.y += GameSettings::cellSize * GameSettings::scale.y; // feet pos
	auto target = m_pGrid->WorldToCellPos(playerPos);

	auto pos = GetAnchorPosition();
	pos.x -= (GameSettings::cellSize / 4) * GameSettings::scale.x; // center the enemy position shift to left for offsett ladderse -= Gsm
	auto startCell = m_pGrid->WorldToCellPos(pos);
	

	auto path = m_pGrid->FindShortestPath(startCell, target);
	auto worldPath = m_pGrid->ConvertPathToWorldPositions(path);

	//for (const auto& cell : worldPath)
	//{
	//	std::cout << "Cell: " << cell.x << ", " << cell.y << std::endl;
	//}

	return worldPath;
}

void EnemyComponent::Activate()
{
	m_CanMove = true;
	m_NewPath = CalculatePath();
	m_TargetPosition = m_NewPath[m_CurrentPathPoint]; // Set the first target position
	m_CurrentPath = m_NewPath;
	m_CalculatePath = false; // Start with the calculated path
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

	bool targetReached;

	auto target = targetPos;
	target.x -= cellSize / 2; // deCenter the target position	
	target.y -= cellSize / 2; // deCenter the target position
	target.y -= 3  * GameSettings::scale.y; //feet 
	//std::cout << "Moving towards target: " << target.x << ", " << target.y << std::endl;
	//std::cout << "Current position: " << pos.x << ", " << pos.y << std::endl;

	constexpr float epsilon = 2.0f;

	if (pos.x < target.x - epsilon)
	{
		newPos.x += move;
		m_CurrentDirection = Direction::Right;
	}
	else if (pos.x > target.x + epsilon)
	{
		newPos.x -= move;
		m_CurrentDirection = Direction::Left;
	}
	else
	{
		newPos.x = target.x; // snap if within tolerance
		// Vertical movement
		if (pos.y < target.y - epsilon)
		{
			newPos.y += move;
			m_CurrentDirection = Direction::Down;
		}
		else if (pos.y > target.y + epsilon)
		{
			newPos.y -= move;
			m_CurrentDirection = Direction::Up;
		}
		else
		{
			newPos.y = target.y; // snap if within tolerance
		}
	}

	

	GetOwner()->GetTransform().SetPosition(newPos);

	if (newPos.x == target.x && newPos.y == target.y)
	{
		return true;
	}
	return false;
}