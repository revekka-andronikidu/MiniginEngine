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

#include <ResourceManager.h>
#include <ServiceLocator.h>
#include <Audio.h>
#include "ObjectFactory.h"
#include <SceneManager.h>
#include <Scene.h>

void load()
{
	//Load audio
	

	//Load resources


	auto& scene = dae::SceneManager::GetInstance().CreateScene("Stage1");
	dae::SceneManager::GetInstance().SetActiveScene("Stage1");


	//background
	auto& objFactory = dae::ObjectFactory::GetInstance();
	auto background = objFactory.CreateTexture("level1.png", {0,0,0}, 2.5f);


	scene.Add(std::move(background));


	dae::ResourceManager::GetInstance().LoadSound("BGM.wav"); //0
	//dae::ServiceLocator::GetAudioService().PlayEffect(0, 0.8f, true);


	//gamecode

}

//std::string GetExeDirectory() {
//	char buffer[MAX_PATH];
//	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
//	std::string exePath = buffer;
//	size_t pos = exePath.find_last_of("\\/");
//	return exePath.substr(0, pos);
//}

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

	const int cellSize{ 16 };
	float scale{ 3.f };
	const int windowWidth{ 208 };
	const int HUDSize{ cellSize * 2 };
	const int windowHeight{ windowWidth + HUDSize };

	dae::Minigin engine("../Data/BurgerTime/", windowWidth * scale, windowHeight * scale);

	
	//load();
	auto& game = dae::GameManager::GetInstance().CreateGame<dae::BurgerTimeGame>();

	dae::GameInitParams initParams{};
	initParams.windowWidth = windowWidth * scale;
	initParams.windowHeight = windowHeight * scale;
	initParams.cellSize = cellSize * scale;
	initParams.scale = {scale, scale, scale};
	initParams.HUDSize = HUDSize * scale;


	dae::GameManager::GetInstance().SetActiveGame(&game);
	game.Initialize(initParams);

	engine.Run();

#ifdef USE_STEAMWORKS
	SteamAPI_Shutdown();
#endif

	return 0;
}