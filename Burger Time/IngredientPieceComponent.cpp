#include "IngredientPieceComponent.h"
#include <ResourceManager.h>
#include <EngineEvents.h>
#include "IngredientComponent.h"
#include "ServiceLocator.h"
#include "EnemyComponent.h"

using namespace dae;
IngredientPieceComponent::IngredientPieceComponent(dae::GameObject* owner, IngredientType type, int piece, IngredientComponent& parent) : GraphicsComponent(owner), IEventListener()
, m_SrcRect{}
, m_SteppedOn{false}
, m_ParentIngredient{ parent }
{
	m_pTexture = ResourceManager::GetInstance().GetTexture("spritesheet.png");
		if(!m_pTexture)
		{
			std::cout << " No texture found" << std::endl;
			//throw
		}

		int sheetWidth{30};
		int sheetHeight{22};
		int startX{14};
		int startY{6};

		auto textureSize = m_pTexture.get()->GetSize();
		int sizeX = textureSize.x / sheetWidth;
		int sizeY = textureSize.y / sheetHeight;
		

		m_SrcRect = {startX*sizeX, startY*sizeY, sizeX, sizeY}; // first pos of sthe sprite sheet
		m_SrcRect.y += sizeY * static_cast<int>(type); //ingredient type
		m_SrcRect.x += sizeX * piece; //piece number

		EventManager::GetInstance().AddListener<CollisionEvent>(owner, this);

}

IngredientPieceComponent::~IngredientPieceComponent()
{
	if (EventManager::IsAlive())
	{
		EventManager::GetInstance().RemoveListener(this);
	}
}

void IngredientPieceComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const glm::vec3& pos = GetOwner()->GetTransform().GetWorldPosition();
		const glm::vec3& scale = GetOwner()->GetTransform().GetScale();

		Renderer::GetInstance().RenderTexture(*m_pTexture, m_SrcRect, pos.x, pos.y, scale);
	}
	else
	{

		//std::cout << " No texture found" << std::endl;
		return;

	}
};

void IngredientPieceComponent::OnNotify(const GameObject& entity, const BaseEvent& event)
{

	if (auto collision = dynamic_cast<const CollisionEvent*>(&event))
	{
		GameObject& other = collision->other;

		if (other.HasTag(Tag::PLAYER_FEET))
		{
			if (!m_SteppedOn && !m_ParentIngredient.m_IsBouncing && !m_ParentIngredient.m_IsFalling && !m_ParentIngredient.m_IsWaitingToFall)
			{
				m_SteppedOn = true;
				ServiceLocator::GetAudioService().PlayEffect(SoundID::BurgerStep.id, 0.8f, false);
				m_ParentIngredient.OnSteppedOn(); //maybe register the parent at start
				IncrementNudge();
			}
		}

		else if (other.HasTag(Tag::BURGER))
		{
			auto otherPiece = other.GetComponent<IngredientPieceComponent>();
			auto otherIngredient = &otherPiece->m_ParentIngredient;

			// Only trigger if this ingredient is above the other
			if (m_ParentIngredient.GetOwner()->GetTransform().GetWorldPosition().y > otherIngredient->GetOwner()->GetTransform().GetWorldPosition().y)
			{
				if (!m_ParentIngredient.m_IsOnTheTray && !m_ParentIngredient.m_IsFalling)
				{
					otherIngredient->m_IsBouncing = true;
					otherIngredient->m_BounceTimer = 0.f;
					otherIngredient->m_IsFalling = false;

					m_ParentIngredient.m_IsBouncing = false;
					m_ParentIngredient.Fall();
				}
				else if (m_ParentIngredient.m_IsOnTheTray)
				{
					if (!otherIngredient->m_IsOnTheTray)
						otherIngredient->RegisterToTray(*m_ParentIngredient.m_Tray);
				}
			}
		}
		if (m_ParentIngredient.m_IsFalling)
		{
			if (other.HasTag(Tag::TRAY))
			{
				m_ParentIngredient.RegisterToTray(other);
				//if enemies on burger, get extra points
			}
			else if (other.HasTag(Tag::PLATFORM))
			{
				if (!m_ParentIngredient.ShouldFall())
				{
					ServiceLocator::GetAudioService().PlayEffect(SoundID::BurgerLand.id, 0.8f, false);
					m_ParentIngredient.m_IsFalling = false;
					if (m_ParentIngredient.m_FloorsToDrop > 0)
					{
						m_ParentIngredient.m_FloorsToDrop--;
						m_ParentIngredient.m_IsWaitingToFall = true;
						m_ParentIngredient.m_DelayBeforeFall = 0.3f; // seconds
						//return;
					}
					else
					{
						m_ParentIngredient.KillEnemies();

					}
				}
			}
			else if (other.HasTag(Tag::ENEMY))
			{
				const auto& ingredientPos = GetOwner()->GetTransform().GetWorldPosition();
				const auto& enemyPos = other.GetTransform().GetWorldPosition();

				// Require ingredient to be above enemy
				if (ingredientPos.y <= enemyPos.y)
				{
					other.GetComponent<EnemyComponent>()->Squish();
				}
			}
		}
	}
}

void IngredientPieceComponent::IncrementNudge()
{
	m_Nudge++;
	if (m_SteppedOn)
	{
		auto pos = GetOwner()->GetTransform().GetLocalPosition();
		pos.y = m_Nudge * m_NudgeSize * GameSettings::scale.y;
		GetOwner()->GetTransform().SetPosition(pos);
	}
	else
	{
		auto pos = GetOwner()->GetTransform().GetLocalPosition();
		pos.y = 0;
		GetOwner()->GetTransform().SetPosition(pos);
	}
}

void IngredientPieceComponent::Reset()
{
	m_SteppedOn = false;
	m_Nudge = 0;

	auto pos = GetOwner()->GetTransform().GetLocalPosition();
	pos.y = 0;
	GetOwner()->GetTransform().SetPosition(pos);
}

