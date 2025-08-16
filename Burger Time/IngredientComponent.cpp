#include "IngredientComponent.h"
#include "IngredientPieceComponent.h"
#include "ObjectFactory.h"
#include "TimeManager.h"
#include "TrayComponent.h"
#include <EventManager.h>
#include "GameEvents.h"
#include "ServiceLocator.h"
#include "ColliderComponent.h"
#include "EnemyComponent.h"

using namespace dae;


IngredientComponent::IngredientComponent(dae::GameObject* owner, IngredientType type) : BaseComponent(owner)
{

	//create pieces

	GeneratePieces(type);
	m_BounceStartY = GetOwner()->GetTransform().GetLocalPosition().y;

}

void IngredientComponent::Update()
{
	if (m_IsFalling)
	{
		auto newPos = GetOwner()->GetTransform().GetLocalPosition();
		newPos.y += m_FallSpeed * TimeManager::GetInstance().GetDeltaTime();
		GetOwner()->GetTransform().SetPosition(newPos);
	}

	if (m_IsBouncing)
	{
		auto dt = TimeManager::GetInstance().GetDeltaTime();
		m_BounceTimer += dt;
		float t = m_BounceTimer / m_BounceDuration;
		float speed = (m_BounceHeight * 2.f) / m_BounceDuration;


		auto pos = GetOwner()->GetTransform().GetLocalPosition();
		if (t < 0.5f)
		{
			pos -= glm::vec3(0.f, speed * dt, 0.f);
			GetOwner()->GetTransform().SetPosition(pos);
		}
		else if (t < 1.0f)
		{
			pos += glm::vec3(0.f, speed * dt, 0.f);
			GetOwner()->GetTransform().SetPosition(pos);
		}
		else
		{
			m_IsBouncing = false;
			m_BounceTimer = 0.f;

			if (m_IsFalling == false)
				m_IsFalling = true;
		}
	}

	// Handle delayed fall
	if (m_IsWaitingToFall)
	{
		if (m_DelayBeforeFall > 0.f)
		{
			auto dt = TimeManager::GetInstance().GetDeltaTime();
			m_DelayBeforeFall -= dt;

			float t = (m_DelayBeforeFall * (-1)) / m_BounceDuration;
			float speed = (m_BounceHeight) / m_BounceDuration;


			auto pos = GetOwner()->GetTransform().GetLocalPosition();
			if (t < 0.5f)
			{
				pos -= glm::vec3(0.f, speed * dt, 0.f);
				GetOwner()->GetTransform().SetPosition(pos);
			}
			else if (t < 1.0f)
			{
				pos += glm::vec3(0.f, speed * dt, 0.f);
				GetOwner()->GetTransform().SetPosition(pos);
			}

			if (m_DelayBeforeFall <= 0.f)
			{
				m_IsWaitingToFall = false;
				Fall();
			}// continue falling after delay
		}
	}
}


void IngredientComponent::GeneratePieces(IngredientType type)
{

	for (int i = 0; i < 4; ++i)
	{
		auto piece = ObjectFactory::GetInstance().CreateIngredientPiece(type, i, *this);

		piece->SetParent(GetOwner());
		auto comp = piece.get()->GetComponent<IngredientPieceComponent>();
		m_pPiecesComponent.push_back(comp);
		m_pPieces.push_back(piece);
	}

}

void IngredientComponent::KillEnemies()
{
	if (!m_EnemiesRegistred) return;
	if (m_pRegistredEnemies.empty()) return;
	
	int points = 500 * (1 << (m_pRegistredEnemies.size() - 1));
	auto event = std::make_shared<dae::PointsIncreasedEvent>(static_cast<int>(points));
	EventManager::GetInstance().TriggerEvent(*event, *GetOwner());


	for (auto enemy : m_pRegistredEnemies)
	{
		if (!enemy) continue;
		enemy->GetComponent<EnemyComponent>()->Die();
	}
	m_EnemiesRegistred = false;
	m_pRegistredEnemies.clear();

	std::cout << "Enemies killed: " << std::to_string(points) << std::endl;
}

void IngredientComponent::OnSteppedOn()
{
	bool allPieces{ true };
	for (auto& piece : m_pPiecesComponent)
	{
		if (!piece->IsSteppedOn())
			allPieces = false;
		else
			piece->IncrementNudge();
	}
	if (!allPieces) return;
	//If we get this far it means all shards are stepped on
	Fall();
}

void IngredientComponent::Fall()
{
	if (m_IsFalling) return;
	
	
	m_IsFalling = true;
	ServiceLocator::GetAudioService().PlayEffect(SoundID::BurgerFall.id, 0.8f, false);

	auto newPos = GetOwner()->GetTransform().GetLocalPosition();
	m_LastHeight = newPos.y;

	newPos.y += 4 * 3 /* m_NudgeSize * levelscale(3)      */;
	GetOwner()->GetTransform().SetPosition(newPos);
	for (auto& piece : m_pPiecesComponent)
	{
		piece->Reset();
	}

	auto event = std::make_shared<dae::PointsIncreasedEvent>(50);
	EventManager::GetInstance().TriggerEvent(*event, *GetOwner());

	if (!m_EnemiesRegistred)
	{
		auto enemies = RegisterEnemies();

		
		if (enemies.size() > 0)
		{
			m_FloorsToDrop = enemies.size();
			ServiceLocator::GetAudioService().PlayEffect(SoundID::EnemyFall.id, 0.8f, false);
			for (auto enemy : enemies)
			{
				if (!enemy) continue;
				enemy->GetComponent<EnemyComponent>()->RegisterToIngredient(GetOwner());
			}
		}
		//for each enemy add two floors to drop 

		//parent enemies to burger and kill them once burger is done falling
	}
	//check for enemies on top of the burgrer, make the fall too and and add points

}

bool IngredientComponent::ShouldFall() 
{ 
	return (m_MinFallDistance > GetOwner()->GetTransform().GetLocalPosition().y - m_LastHeight); 
};

std::vector<GameObject*> IngredientComponent::RegisterEnemies()
{
	std::unordered_set<GameObject*> uniqueEnemies; // ensures no duplicates

	for (auto& pPiece : m_pPieces)
	{
		if (!pPiece) continue;

		auto pCollider = pPiece->GetComponent<ColliderComponent>();
		if (!pCollider) continue;

		for (auto* obj : pCollider->GetCurrentOverlaps())
		{
			if (!obj) continue;

			if (obj->HasTag(Tag::ENEMY))
			{
				auto enemyComp = obj->GetComponent<EnemyComponent>();
				if (enemyComp && enemyComp->CanBeRegistered())
				{
					uniqueEnemies.insert(obj);
				}
			}
		}
	}

	// Copy unique enemies into the vector
	m_pRegistredEnemies.assign(uniqueEnemies.begin(), uniqueEnemies.end());

	m_EnemiesRegistred = true;
	return m_pRegistredEnemies;
}

void IngredientComponent::RegisterToTray(const GameObject& tray)
{
	m_IsFalling = false;
	m_IsOnTheTray = true;

	auto trayComp = tray.GetComponent<TrayComponent>();
	if (trayComp)
	{
		trayComp->RegisterIngredient(this);
		m_Tray = &tray;
	}

	ServiceLocator::GetAudioService().PlayEffect(SoundID::BurgerLand.id, 0.8f, false);

	if (m_EnemiesRegistred)
	{
		KillEnemies();
	}
}
