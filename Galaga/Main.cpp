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
//#include "SceneManager.h"
//#include "ResourceManager.h"
//#include "Scene.h"
//#include "TextComponent.h"
////#include "FPSComponent.h"
//#include "TextureComponent.h"
////#include "LivesComponent.h"
////#include "LivesDisplay.h"
////#include "PointsComponent.h"
////#include "PointsDisplay.h"
//#include "InputManager.h"
//#include "GameCommands.h"
//#include "MenuComponent.h"
//#include "ActionComponent.h"
//#include "ObjectFactory.h"

//#include "ServiceLocator.h"
//#include "Audio.h"

#include <GameManager.h>
#include "GalagaGame.h"

void LoadGame()
{

	//auto& scene = dae::SceneManager::GetInstance().CreateScene("MainMenu");


	//auto background = std::make_shared<dae::GameObject>();
	//background->AddComponent<dae::TextureComponent>("bg_back2.png");
	//scene.Add(background);


	////auto text = std::make_shared<dae::GameObject>();
	//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	////text->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	////text->GetTransform().SetPosition(glm::vec3(80.f, 50.f, 0.f));
	////scene.Add(text);

	////auto logo = std::make_shared<dae::GameObject>();
	////logo->AddComponent<dae::TextureComponent>("logo.tga");
	////logo->GetTransform().S*/etPosition(glm::vec3(216.f, 180.f, 0));
	////scene.Add(logo);

	//auto fps = std::make_shared<dae::GameObject>();
	//fps->AddComponent<dae::TextComponent>("FPS: ", font);
	//fps->AddComponent<dae::FPSComponent>();
	//scene.Add(fps);

	//auto player1 = std::make_shared<dae::GameObject>();
	//player1->AddComponent<dae::TextureComponent>("galaga.png");
	//player1->GetTransform().SetPosition(glm::vec3(256.f, 300.f, 0));
	////player1->AddComponent<dae::RotatorComponent>(70.f, 2.f);
	//player1->GetTransform().SetScale(glm::vec3(0.5f, 0.5f, 0.5f));


	//auto& input = dae::InputManager::GetInstance();

	//auto moveUp = std::make_unique<MoveCommand>(player1.get(), glm::vec3{ 0, -1, 0 }, 100.f);
	//auto moveDown = std::make_unique<MoveCommand>(player1.get(), glm::vec3{ 0, 1, 0 }, 100.f);
	//auto moveRight = std::make_unique<MoveCommand>(player1.get(), glm::vec3{ 1, 0, 0 }, 100.f);
	//auto moveLeft = std::make_unique<MoveCommand>(player1.get(), glm::vec3{ -1, 0, 0 }, 100.f);
	////removeLive = std::make_unique<LooseLiveCommand>(player1.get());


	//input.BindInput(KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_W }, std::move(moveUp));
	//input.BindInput(KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_S }, std::move(moveDown));
	//input.BindInput(KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_D }, std::move(moveRight));
	//input.BindInput(KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_A }, std::move(moveLeft));


	//auto playerOneLives = std::make_shared<dae::GameObject>();

	//playerOneLives->AddComponent<dae::LivesDisplay>("galaga.png", glm::vec3(0.5f, 0.5f, 1));
	//playerOneLives->GetTransform().SetPosition(glm::vec3(0, 440, 0));
	//scene.Add(playerOneLives);

	//auto livesComp = player1->AddComponent<dae::LivesComponent>(3);

	//livesComp->AddObserver(playerOneLives->GetComponent<LivesDisplay>());
	//livesComp->SetLives(3);

	//auto removeLive = std::make_unique<LooseLiveCommand>(player1.get());

	//input.BindInput(KeyboardInput{ ButtonState::KeyUp,SDL_SCANCODE_X }, std::move(removeLive));




	//auto pointsComp = player1->AddComponent<dae::PointsComponent>();

	//auto playerOnePoints = std::make_shared<dae::GameObject>();
	//playerOnePoints->AddComponent<dae::PointsDisplay>(); //add custom font to points display

	//pointsComp->AddObserver(playerOnePoints->GetComponent<PointsDisplay>());

	//auto increasePoints = std::make_unique<AddScoreCommand>(player1.get(), 100);

	//input.BindInput(KeyboardInput{ ButtonState::KeyUp,SDL_SCANCODE_C }, std::move(increasePoints));

	//playerOnePoints->GetTransform().SetPosition(glm::vec3(200, 300, 0));
	//scene.Add(playerOnePoints);

	//scene.Add(player1);



	//auto player2 = std::make_shared<dae::GameObject>();
	//player2->AddComponent<dae::TextureComponent>("galagaRed.png");
	//player2->GetTransform().SetPosition(glm::vec3(0.f, 0.f, 0));
	////player2->AddComponent<dae::RotatorComponent>(50.f, 2.f, false);
	//player2->GetTransform().SetScale(glm::vec3(0.5f, 0.5f, 0.5f));

	////player2->SetParent(player1.get(), true);


	////input
	//moveUp = std::make_unique<MoveCommand>(player2.get(), glm::vec3{ 0, -1, 0 }, 50.f);
	//moveDown = std::make_unique<MoveCommand>(player2.get(), glm::vec3{ 0, 1, 0 }, 50.f);
	//moveRight = std::make_unique<MoveCommand>(player2.get(), glm::vec3{ 1, 0, 0 }, 50.f);
	//moveLeft = std::make_unique<MoveCommand>(player2.get(), glm::vec3{ -1, 0, 0 }, 50.f);

	////auto addScore100 = std::make_unique<AddScoreCommand>(pacman.get(), 100);*/


	//input.AddController(0);
	//input.BindInput(ControllerInput{ ButtonState::KeyPressed, XboxController::ControllerButton::DPadUp,  0 }, std::move(moveUp));
	//input.BindInput(ControllerInput{ ButtonState::KeyPressed, XboxController::ControllerButton::DPadDown,  0 }, std::move(moveDown));
	//input.BindInput(ControllerInput{ ButtonState::KeyPressed, XboxController::ControllerButton::DPadRight, 0 }, std::move(moveRight));
	//input.BindInput(ControllerInput{ ButtonState::KeyPressed, XboxController::ControllerButton::DPadLeft, 0 }, std::move(moveLeft));

	////input.BindInput(KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_A }, std::move(addScore100));

	//auto playerTwoLives = std::make_shared<dae::GameObject>();

	//playerTwoLives->AddComponent<dae::LivesDisplay>("galagaRed.png", glm::vec3(0.5f, 0.5f, 1), true);
	//playerTwoLives->GetTransform().SetPosition(glm::vec3(400, 440, 0));
	//scene.Add(playerTwoLives);

	//livesComp = player2->AddComponent<dae::LivesComponent>(3);

	//livesComp->AddObserver(playerTwoLives->GetComponent<LivesDisplay>());
	//livesComp->SetLives(3);

	//removeLive = std::make_unique<LooseLiveCommand>(player2.get());
	//input.BindInput(ControllerInput{ ButtonState::KeyUp, XboxController::ControllerButton::ButtonX, 0 }, std::move(removeLive));

	//scene.Add(player2);

	//std::unique_ptr<IAudio> audio = std::make_unique<SDLAudio>();
	//std::unique_ptr<LoggerAudio> LogAudio = std::make_unique<LoggerAudio>(std::move(audio));

	//ServiceLocator::RegisterAudioService(std::move(LogAudio));

	////ResourceManager::GetInstance().LoadSound("BossDeath.mp3");

	////ServiceLocator::GetAudioService().PlayEffect(0, 0.8, true);

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