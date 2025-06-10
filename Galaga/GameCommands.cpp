#include "GameCommands.h"
#include "LivesComponent.h"
#include "PointsComponent.h"
#include <GameManager.h>
#include "GalagaGame.h"



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
	GetGameObject()->GetTransform().SetPosition(translation);

	/*	glm::vec3 pos = translation;
		std::cout << "Moved to: ("
			+ std::to_string(pos.x) + ", "
			+ std::to_string(pos.y) + ", "
			+ std::to_string(pos.z) + ")"
			<< std::endl;*/

};


void SkipLevelCommand::Execute()
{
	auto* game = dae::GameManager::GetInstance().GetActiveGame();
	if (!game) return; // check nullptr
	GalagaGame* galaga = dynamic_cast<GalagaGame*>(game);
	if (!galaga) return; // cast failed
	galaga->SkipStage();
}

void MuteGameCommand::Execute()
{

}