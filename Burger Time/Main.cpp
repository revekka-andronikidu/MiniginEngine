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
#include <ResourceManager.h>
#include <ServiceLocator.h>
#include <Audio.h>

void load()
{
	//Load audio
	std::unique_ptr<dae::IAudio> audio = std::make_unique<dae::SDLAudio>();
#if _DEBUG
	std::unique_ptr<dae::LoggerAudio> LogAudio = std::make_unique<dae::LoggerAudio>(std::move(audio));
	dae::ServiceLocator::RegisterAudioService(std::move(LogAudio));
#else
	dae::ServiceLocator::RegisterAudioService(std::move(audio));
#endif



	dae::ResourceManager::GetInstance().LoadSound("GalagaTheme.wav"); //0
	//dae::ResourceManager::GetInstance().LoadSound("Fighter-Shot.wav"); //1
	//dae::ResourceManager::GetInstance().LoadSound("Galaga-explosion.wav"); //2
	//dae::ResourceManager::GetInstance().LoadSound("Galaga-kill.wav"); //3

	
	dae::ServiceLocator::GetAudioService().PlayEffect(0, 0.8, true);

}

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

	//auto datapath = GetExeDirectory() + "/Data/";
	//auto datapath = ("../Data/");

#if _DEBUG
	
#endif

	dae::Minigin engine("../Data/");



	//auto& game = dae::GameManager::GetInstance().CreateGame<dae::GalagaGame>();
	//dae::GameManager::GetInstance().SetActiveGame(&game);

	load();

	engine.Run();

#ifdef USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif

	return 0;
}