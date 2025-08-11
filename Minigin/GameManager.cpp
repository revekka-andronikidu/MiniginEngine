#pragma once
#include "Game.h"
#include "GameManager.h"

using namespace dae;


void GameManager::SetActiveGame(Game* game)
{
	//TODO: deactivate the previous game safely
	
	m_ActiveGame = game;

	//TODO: call INIT here?
};