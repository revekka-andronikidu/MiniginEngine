#pragma once
#include "PlayerComponent.h"
#include <algorithm>
#include <glm.hpp>
#include <GameObject.h>
#include <TimeManager.h>

#include "SceneManager.h"
#include "GridComponent.h"
#include "Scene.h"

using namespace dae;

dae::PlayerComponent::PlayerComponent(GameObject* owner) : BaseComponent(owner)
{

}

void PlayerComponent::Move(Direction dir)
{
	if (!m_pGrid)
		return;

	//GET THESE FROM LEVEL/GAME INFO
	const int gridWidth = m_pGrid->GetWidth();
	const int gridHeight = m_pGrid->GetHeight();


	//get from grid
	//const float scale{ GameSettings::scale.x };
	const int cellSize{ static_cast<int>(GameSettings::cellSize * GameSettings::scale.x) };
	
	//////////////

	//GET FROM LEVEL AS WELL OR MOVE COMMAND?

	int m_LeftBorder{0};
	int m_RightBorder{cellSize * gridWidth};


	auto pos = GetOwner()->GetTransform().GetLocalPosition();


	int playerCenterX = static_cast<int>(pos.x + cellSize/2); // middle of the cell
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
		bool isOnLadder = m_pGrid->CanClimb(Direction::Down, pos.x + cellSize/2, playerFeetY);
		if (isOnLadder)
		{
			float moveY = m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime();
			newPos.y += moveY;

			newPos.x = m_pGrid->GetLadderCenterX(pos.x + cellSize/2, pos.y);
		}
	}
	break;

	case Direction::Up:
	{
		bool isOnLadder = m_pGrid->CanClimb(Direction::Up, pos.x + cellSize / 2, playerFeetY);
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


void PlayerComponent::Animate()
{

}