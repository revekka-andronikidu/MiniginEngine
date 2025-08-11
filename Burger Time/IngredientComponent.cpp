#include "IngredientComponent.h"
#include "IngredientPieceComponent.h"
#include "ObjectFactory.h"
#include "TimeManager.h"

using namespace dae;


IngredientComponent::IngredientComponent(dae::GameObject* owner, IngredientType type) : BaseComponent(owner)
{

	//create pieces

	GeneratePieces(type);

}

void IngredientComponent::Update()
{
	if (m_IsFalling)
	{
		auto newPos = GetOwner()->GetTransform().GetLocalPosition();
		newPos.y += m_FallSpeed * TimeManager::GetInstance().GetDeltaTime();
		GetOwner()->GetTransform().SetPosition(newPos);
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

	auto newPos = GetOwner()->GetTransform().GetLocalPosition();
	m_LastHeight = newPos.y;

	newPos.y += 4 * 3 /* m_NudgeSize * levelscale(3)      */;
	GetOwner()->GetTransform().SetPosition(newPos);
	for (auto& piece : m_pPiecesComponent)
	{
		piece->Reset();
	}

	//add 50 points (to the player and player notifies the point display?)

}

bool IngredientComponent::ShouldFall() 
{ 
	return (m_MinFallDistance < GetOwner()->GetTransform().GetLocalPosition().y - m_LastHeight); 
};
