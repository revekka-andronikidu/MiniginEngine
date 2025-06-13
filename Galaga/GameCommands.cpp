#include "GameCommands.h"
#include "LivesComponent.h"
#include "PointsComponent.h"
#include <GameManager.h>
#include "GalagaGame.h"
#include "ShootingComponent.h"
#include <algorithm>



void LooseLiveCommand::Execute() 
{
	GetGameObject()->GetComponent<LivesComponent>()->RemoveLive();
};

void AddScoreCommand::Execute()
{
	GetGameObject()->GetComponent<PointsComponent>()->IncreasePoints(m_Points);
};

void MenuMoveCommand::Execute()
{
	GetGameObject()->GetComponent<MenuComponent>()->ChangeMenuItem(m_direction);
};

void MenuEnterCommand::Execute()
{
	GetGameObject()->GetComponent<MenuComponent>()->EnterMenuItem();
};

void MoveCommand::Execute()
{
	auto transform = GetGameObject()->GetTransform();
	auto translation = m_Direction * m_MoveSpeed * TimeManager::GetInstance().GetDeltaTime() + transform.GetLocalPosition();

	//Clamp X within borders
	translation.x = std::clamp(translation.x, static_cast<float>(m_LeftBorder), static_cast<float>(m_RightBorder));

	GetGameObject()->GetTransform().SetPosition(translation);
};

void ShootCommand::Execute()
{
	auto shootingComp = GetGameObject()->GetComponent<ShootingComponent>();
	if (!shootingComp) return;

	shootingComp->Shoot();
};



void SkipLevelCommand::Execute()
{
	auto* game = dae::GameManager::GetInstance().GetActiveGame();
	if (!game) return; // check nullptr
	GalagaGame* galaga = dynamic_cast<GalagaGame*>(game);
	if (!galaga) return; // cast failed
	galaga->SkipToNextStage();
}

void MuteGameCommand::Execute()
{

}