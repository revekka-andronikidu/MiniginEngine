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
#include "GalagaGame.h"

std::string GetExeDirectory() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string exePath = buffer;
	size_t pos = exePath.find_last_of("\\/");
	return exePath.substr(0, pos);
}

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

	auto datapath = GetExeDirectory() + "/Data/";

#if _DEBUG
	std::cout << "Looking for data in: " << datapath << std::endl;
#endif

	dae::Minigin engine(datapath);

	auto& game = dae::GameManager::GetInstance().CreateGame<dae::GalagaGame>();
	dae::GameManager::GetInstance().SetActiveGame(&game);

	engine.Run();

#ifdef USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif

	return 0;
}