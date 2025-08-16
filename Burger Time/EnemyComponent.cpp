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

EnemyComponent::EnemyComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
	, m_CurrentDirection(Direction::None)
	, m_pGrid(nullptr)
{
	EventManager::GetInstance().AddListener<CollisionEvent>(pOwner, this);

	//m_pints based on enemy type
	m_Points = 100; 
	
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
			//trigger enemy death event
			GetOwner()->Destroy();
		}
	}
	if (m_CanMove)
	{
		if (m_CalculatePath)
		{
			int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);

			auto player = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag(Tag::PLAYER);
			auto targetPos = player[0]->GetTransform().GetLocalPosition();
			targetPos.y += cellSize; // to get player feet position
			targetPos.x += cellSize / 2; // to get player center position

			auto pos = GetOwner()->GetTransform().GetLocalPosition();
			pos.x += cellSize / 2; // to get enemy center position
			pos.y += cellSize; // to get enemy feet position

			int cellX = pos.x / cellSize;
			int cellY = pos.y / cellSize;

			int cellXPlayer = targetPos.x / cellSize;
			int cellYPlayer = targetPos.y / cellSize;

			CellPos start = CellPos{ cellX, cellY };
			CellPos goal = CellPos{ cellXPlayer, cellYPlayer };

			m_CurrentPath = m_pGrid->FindShortestPath(start, goal);
			for (auto p : m_CurrentPath)
			{
				std::cout << "Path: (" << p.x << ", " << p.y << ")\n";
			}
			m_CalculatePath = false;
			m_NextCellIndex = 1;
		}
		FollowPath();
		Move();
	}
}

void EnemyComponent::FollowPath()
{
	if (m_CurrentPath.empty() || m_NextCellIndex >= m_CurrentPath.size())
		return;

	// Current enemy position in world coordinates
	auto pos = GetOwner()->GetTransform().GetLocalPosition();
	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);
	CellPos currentCell = CellPos{
		static_cast<int>((pos.x + cellSize / 2) / cellSize),
		static_cast<int>((pos.y + cellSize) / cellSize)
	};

	// Target next cell in path
	CellPos nextCell = m_CurrentPath[m_NextCellIndex];

	// Determine direction to next cell
	if (nextCell.x > currentCell.x) m_CurrentDirection = Direction::Right;
	else if (nextCell.x < currentCell.x) m_CurrentDirection = Direction::Left;
	else if (nextCell.y < currentCell.y) m_CurrentDirection = Direction::Up;
	else if (nextCell.y > currentCell.y) m_CurrentDirection = Direction::Down;

	//// Adjust for offset ladders
	//if (m_CurrentDirection == Direction::Up || m_CurrentDirection == Direction::Down)
	//{
	//	auto& gridCell = m_pGrid->GetCell(nextCell.x, nextCell.y);
	//	if (gridCell.HasObject(CellObject::OFFSETLADDER))
	//	{
	//		// Move X to ladder center (half cell offset)
	//		float ladderCenterX = nextCell.x * cellSize + cellSize / 2.0f;
	//		pos.x = ladderCenterX; // enemy X is center
	//		GetOwner()->GetTransform().SetPosition(pos);
	//	}
	//}


	// If enemy reached the next cell, move to the next path node
	if (currentCell == nextCell)
	{
		m_NextCellIndex++;
		if (m_NextCellIndex >= m_CurrentPath.size())
			m_CalculatePath = true; // recalc path at the end
	}
}

Direction EnemyComponent::CalculateDirection()
{
	
	return Direction::None;
}



bool EnemyComponent::CanGoLeft()
{
	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);
	auto player = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag(Tag::PLAYER);

	auto targetPos = player[0]->GetTransform().GetLocalPosition();
	auto pos = GetOwner()->GetTransform().GetLocalPosition();
	float enemyFeet = pos.y + cellSize; // to get enemy feet


	return targetPos.x < pos.x && m_pGrid->IsOnPlatform(pos.x, enemyFeet);
}
bool EnemyComponent::CanGoRight()
{
	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);
	auto player = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag(Tag::PLAYER);

	auto targetPos = player[0]->GetTransform().GetLocalPosition();
	auto pos = GetOwner()->GetTransform().GetLocalPosition();
	float enemyFeet = pos.y + cellSize; // to get enemy feet

	return targetPos.x > pos.x && m_pGrid->IsOnPlatform(pos.x + cellSize, enemyFeet);

}
bool EnemyComponent::CanGoUp()
{
	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);
	auto player = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag(Tag::PLAYER);

	auto targetPos = player[0]->GetTransform().GetLocalPosition();
	auto pos = GetOwner()->GetTransform().GetLocalPosition();
	float enemyFeet = pos.y + cellSize; // to get enemy feet

	return m_pGrid->CanClimb(Direction::Up, pos.x + cellSize / 2, enemyFeet);
}
bool EnemyComponent::CanGoDown()
{
	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);
	auto player = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag(Tag::PLAYER);

	auto targetPos = player[0]->GetTransform().GetLocalPosition();
	auto pos = GetOwner()->GetTransform().GetLocalPosition();
	float enemyFeet = pos.y + cellSize; // to get enemy feet

	return m_pGrid->CanClimb(Direction::Down, pos.x + cellSize / 2, enemyFeet);

}


void EnemyComponent::Move()
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
		int cellY = static_cast<int>(newPos.y + cellSize) / (cellSize);
		newPos.y = static_cast<float>(cellY * (cellSize)-3 * GameSettings::scale.y);
	}
		break;
	case Direction::Right:
	{
		newPos.x += move;
		int cellY = static_cast<int>(newPos.y + cellSize) / cellSize;
		newPos.y = static_cast<float>(cellY * (cellSize)-3 * GameSettings::scale.y);
	}
		break;
	case Direction::Up:
		newPos.y -= move;
		newPos.x = m_pGrid->GetLadderCenterX(pos.x + cellSize / 2, pos.y);
		break;
	case Direction::Down:
		newPos.y += move;
		newPos.x = m_pGrid->GetLadderCenterX(static_cast<int>(pos.x + cellSize / 2), pos.y);
		break;
	}

	GetOwner()->GetTransform().SetPosition(newPos);
}


void EnemyComponent::Pepper()
{
	m_CanMove = false;
	m_IsPeppered = true;
	m_PepperedTimer = 0.f;
	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (!sprite) return;


	sprite->SetAnimation("Peppered");
	sprite->SetAnimate(true);
}
void EnemyComponent::UnPepper()
{
	m_IsPeppered = false;
	m_CanMove = true;

	//later exclude this
	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (!sprite) return;
	sprite->SetAnimation("Down");
	sprite->SetAnimate(false);


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

	auto sprite = GetOwner()->GetComponent<SpriteSheetComponent>();
	if (!sprite) return;
	sprite->SetAnimation("Death");
	sprite->SetAnimate(true);
}

void EnemyComponent::RegisterToIngredient(GameObject* burger)
{
	m_IsRegisteredToBurger = true;
	m_CanMove = false;
	//diable collision
	//GetOwner()->GetTransform().SetPosition(burger->GetTransform().GetWorldPosition());
	GetOwner()->SetParent(burger, true);

}