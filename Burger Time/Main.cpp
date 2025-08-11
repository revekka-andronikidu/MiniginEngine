#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

//#define USE_STEAMWORKS

#ifdef USE_STEAMWORKS
#include <steam_api.h>
#endif

#include <windows.h>
#include <iostream>
#include "Minigin.h"

#include <GameManager.h>
#include "BurgerTimeGame.h"
#include "Helpers.h"

int main(int, char* [])
{
#ifdef USE_STEAMWORKS
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
		std::cout << "Successfully initialized steam." << std::endl;
#endif


	dae::Minigin engine("../Data/BurgerTime/", dae::GameSettings::windowWidth * dae::GameSettings::scale.x, dae::GameSettings::windowHeight * dae::GameSettings::scale.y);

	auto& gameManager = dae::GameManager::GetInstance();

	auto& game = gameManager.CreateGame<dae::BurgerTimeGame>();
	gameManager.SetActiveGame(&game);
	game.Initialize();

	engine.Run();

#ifdef USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif

	return 0;
}