#include "GameCommands.h"
#include <ServiceLocator.h>
#include <TimeManager.h>
#include <algorithm>
#include <SpriteSheetComponent.h>
#include "PlayerComponent.h"

using namespace dae;

void MuteGameCommand::Execute()
{
	ServiceLocator::GetAudioService().Mute();
}

void MenuMoveCommand::Execute()
{
	GetGameObject()->GetComponent<MenuComponent>()->ChangeMenuItem(m_direction);
};

void MenuEnterCommand::Execute()
{
	GetGameObject()->GetComponent<MenuComponent>()->EnterMenuItem();
};

void StopMove::Execute()
{
	auto obj = GetGameObject()->GetComponent<SpriteSheetComponent>();
	obj->SetAnimate(false);
};

void MoveCommand::Execute()
{
	//Create player class or component that has function that manges the animation and call it here instead
	auto player = GetGameObject()->GetComponent<PlayerComponent>();
	player->Move(m_Direction);


	//MOVE THIS TO ANIMATE FUNCTION OF PLAYER

	//player->Animate();

	auto obj = GetGameObject()->GetComponent<SpriteSheetComponent>();

		switch (m_Direction)
		{
		case Direction::Up:  
			obj->SetAnimation("Up");
			break;
		case Direction::Down:  
			obj->SetAnimation("Down");
			break;
		case Direction::Left: 
			obj->SetAnimation("Left");
			break;
		case Direction::Right: 
			obj->SetAnimation("Throw");
			break;
		default: 
			//obj->SetAnimation("Idle");
			obj->SetAnimate(false);
			break;
		}	

	if (obj)
	{
		obj->SetAnimate(true); // This is the key
	}

	/////////////

};

