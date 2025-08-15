#include "IngredientPieceComponent.h"
#include <ResourceManager.h>
#include <EngineEvents.h>
#include "IngredientComponent.h"
#include "ServiceLocator.h"

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
		const GameObject& other = collision->other;

		if (other.HasTag(Tag::PLAYER_FEET))
		{
			if (!m_SteppedOn)
			{
				m_SteppedOn = true;
				ServiceLocator::GetAudioService().PlayEffect(SoundID::BurgerStep.id, 0.8f, false);
				m_ParentIngredient.OnSteppedOn(); //maybe register the parent at start
				IncrementNudge();
			}
		}

		else if (other.HasTag(Tag::BURGER))
		{
			//if not on the tray
			if (!m_ParentIngredient.m_IsOnTheTray)
				m_ParentIngredient.Fall();
			else
			{
				auto ingredient = other.GetParent()->GetComponent<IngredientComponent>();
				if(!ingredient->m_IsOnTheTray)
				ingredient->RegisterToTray(*m_ParentIngredient.m_Tray);
			}
		}
		else if (other.HasTag(Tag::TRAY))
		{
			if (m_ParentIngredient.m_IsFalling)
			{
				m_ParentIngredient.RegisterToTray(other);
			}
		}
		else if (other.HasTag(Tag::PLATFORM))
		{
			if (m_ParentIngredient.m_IsFalling)
			{
				if (!m_ParentIngredient.ShouldFall())
				{
					m_ParentIngredient.m_IsFalling = false;
					ServiceLocator::GetAudioService().PlayEffect(SoundID::BurgerLand.id, 0.8f, false);
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

