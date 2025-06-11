#pragma once
#include "GalagaGame.h"
//#include <windows.h>
#include <SDL.h>
#include <InputManager.h>
#include "ObjectFactory.h"
#include "GameCommands.h"

using namespace dae;

GalagaGame::GalagaGame()
{
	//load game info (high scores, level details)
	//
	//load audio
	//load resources (fonts, audio, textures)

	CreateMainMenu();
	SetDebugCommands();
}

void GalagaGame::SetDebugCommands()
{
	auto& input = dae::InputManager::GetInstance();

	auto skipLevel = std::make_unique<SkipLevelCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_F1 }, std::move(skipLevel));
	auto muteGame = std::make_unique<MuteGameCommand>();
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_F2 }, std::move(muteGame));

	

}

void GalagaGame::CreateMainMenu()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("MainMenu");
	dae::SceneManager::GetInstance().SetActiveScene(scene);

	auto font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);

	//background
	auto background = std::make_shared<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>("bg_back2.png");
	scene.Add(background);

	auto mainMenu = std::make_shared<dae::GameObject>();
	auto menu = mainMenu->AddComponent<dae::MenuComponent>();

	int windowWidth{ 480 };
	int windowHeight{ 512 };
	int gap{ 30 };


	//logo
	auto logo = std::make_shared<dae::GameObject>();
	auto texture = logo->AddComponent<dae::TextureComponent>("Galaga_logo.png");
	auto logoSize = texture->GetTextureSize();
	float scale{ 0.25f };
	logo->GetTransform().SetPosition(glm::vec3(windowWidth / 2 - ((logoSize.x * scale) / 2), 70.f, 0));
	logo->GetTransform().SetScale(glm::vec3(scale, scale, scale));
	logo->SetParent(mainMenu.get(), false);
	scene.Add(logo);

	
	auto onePlayerMenuItem = MakeMenuItem("1 PLAYER", font, glm::vec3(windowWidth / 2 - 60.f, windowHeight / 2, 0.f), menu, [this]()
		{
			//dae::SceneManager::GetInstance().SetActiveScene("OnePlayer");
			EnterSinglePlayer();
		});

	auto twoPlayersMenuItem = MakeMenuItem("2 PLAYERS", font, glm::vec3(windowWidth / 2 - 60.f, windowHeight / 2 + gap, 0.f), menu, []()
		{
			dae::SceneManager::GetInstance().SetActiveScene("Coop");
		});

	auto versusMenuItem = MakeMenuItem("VERSUS", font, glm::vec3(windowWidth / 2 - 60.f, windowHeight / 2 + gap*2, 0.f), menu, []()
		{
			dae::SceneManager::GetInstance().SetActiveScene("Versus");
		});

	auto highScoresMenuItem = MakeMenuItem("HIGH SCORES", font, glm::vec3(windowWidth / 2 - 60.f, windowHeight / 2 + gap * 3, 0.f), menu, []()
		{
			dae::SceneManager::GetInstance().SetActiveScene("HighScores");
		});

	auto menuPointerItem = MakeMenuArrow("arrow.png", menu, 0.1f);



	scene.Add(onePlayerMenuItem);
	scene.Add(twoPlayersMenuItem);
	scene.Add(versusMenuItem);
	scene.Add(highScoresMenuItem);
	scene.Add(menuPointerItem);

	scene.Add(mainMenu);


	//inputs
	auto& input = dae::InputManager::GetInstance();

	auto menuUp = std::make_unique<MenuMoveCommand>(mainMenu.get(), MenuComponent::Direction::Up);
	auto menuDown = std::make_unique<MenuMoveCommand>(mainMenu.get(), MenuComponent::Direction::Down);
	auto menuUpW = std::make_unique<MenuMoveCommand>(mainMenu.get(), MenuComponent::Direction::Up);
	auto menuDownS = std::make_unique<MenuMoveCommand>(mainMenu.get(), MenuComponent::Direction::Down);
	auto menuEnter = std::make_unique<MenuEnterCommand>(mainMenu.get());

	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_W }, std::move(menuUpW));
	input.BindGlobalInput(KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_S }, std::move(menuDownS));
	//input.BindInput(KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_UP }, std::move(menuUp));
	//input.BindInput(KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_DOWN }, std::move(menuDown));
	//input.BindGlobalInput(KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_RETURN }, std::move(menuEnter));

	input.AddController(0);
	input.BindGlobalInput(ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::DPadUp, 0 }, std::move(menuUp));
	//input.BindGlobalInput(ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::DPadDown, 0 }, std::move(menuDown));
	input.BindGlobalInput(ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::ButtonA, 0 }, std::move(menuEnter));

}

void GalagaGame::SkipStage()
{

	//check if in stage and not a menu 

	//skip

#if _DEBUG
	std::cout << "Stage Skipped. Current stage: " << std::to_string(m_CurrentStage) << std::endl;
#endif
}

void GalagaGame::EnterSinglePlayer()
{
	//create all scenes for mode
	auto& scenePlayer = dae::SceneManager::GetInstance().CreateScene("OnePlayer");
	dae::SceneManager::GetInstance().SetActiveScene("OnePlayer");
	CreateSinglePlayerScene();

}

void GalagaGame::CreateSinglePlayerScene()
{
	auto text = std::make_shared<dae::GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	text->AddComponent<dae::TextComponent>("Stage 1 WIP", font);
	text->GetTransform().SetPosition(glm::vec3(80.f, 50.f, 0.f));
	auto& scene = dae::SceneManager::GetInstance().GetScene("OnePlayer");
	scene.Add(text);
}