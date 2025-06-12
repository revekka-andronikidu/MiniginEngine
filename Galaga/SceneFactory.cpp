#pragma once
#include "SceneFactory.h"
#include "ObjectFactory.h"
#include "SceneNames.h"
#include "GalagaGame.h"
#include "InputManager.h"
#include "GameCommands.h"

using namespace dae;

SceneFactory::SceneFactory()
{
			auto game = GameManager::GetInstance().GetActiveGame();
			m_Galaga = dynamic_cast<GalagaGame*>(game);
			if (!m_Galaga)
				throw std::runtime_error("Game Not Found");
};

void SceneFactory::CreateMainMenu()
{
	//if (m_MenuCreated) return;

	auto& scene = SceneManager::GetInstance().GetScene(SceneNames::MainMenu);
	auto& objFactory = ObjectFactory::GetInstance();

	auto font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16); //TODO: GET INTEAD OF LOAD

	auto menu = objFactory.CreateMainMenu();
	auto menuComp = menu.get()->GetComponent<MenuComponent>();;

	auto background = objFactory.CreateTexture("bg_back2.png");

	float scale{ 0.25f };
	auto logo = objFactory.CreateTexture("Galaga_logo.png", glm::vec3(0, 0, 0), scale);
	auto logoSize = logo.get()->GetComponent<TextureComponent>()->GetTextureSize();
	logo->GetTransform().SetPosition(glm::vec3(GalagaGame::m_GameWindowWidth / 2 - ((logoSize.x * scale) / 2), 70.f, 0));
	logo->GetTransform().SetScale(glm::vec3(scale, scale, scale));
	logo->SetParent(menu.get(), false);

	int gap{ 30 };
	auto onePlayerMenuItem = objFactory.CreateMenuItem("1 PLAYER", font, glm::vec3(GalagaGame::m_GameWindowWidth / 2 - 60.f, GalagaGame::m_GameWidnowHeight / 2, 0.f), menuComp, [this]()
		{
			//dae::SceneManager::GetInstance().SetActiveScene("OnePlayer");

			m_Galaga->EnterGameMode(std::make_unique<SinglePlayerMode>());
		});
	auto twoPlayersMenuItem = objFactory.CreateMenuItem("2 PLAYERS", font, glm::vec3(GalagaGame::m_GameWindowWidth / 2 - 60.f, GalagaGame::m_GameWidnowHeight / 2 + gap, 0.f), menuComp, [this]()
		{
			m_Galaga->EnterGameMode(std::make_unique<CoopMode>());
		});

	auto versusMenuItem = objFactory.CreateMenuItem("VERSUS", font, glm::vec3(GalagaGame::m_GameWindowWidth / 2 - 60.f, GalagaGame::m_GameWidnowHeight / 2 + gap * 2, 0.f), menuComp, [this]()
		{
			m_Galaga->EnterGameMode(std::make_unique<VersusMode>());
		});

	auto highScoresMenuItem = objFactory.CreateMenuItem("HIGH SCORES", font, glm::vec3(GalagaGame::m_GameWindowWidth / 2 - 60.f, GalagaGame::m_GameWidnowHeight / 2 + gap * 3, 0.f), menuComp, [this]()
		{
			//EnterScene
			dae::SceneManager::GetInstance().SetActiveScene("HighScores");
		});

	auto menuPointerItem = objFactory.CreateMenuArrow("arrow.png", menuComp, 0.1f);


	scene.Add(background);
	scene.Add(logo);
	scene.Add(onePlayerMenuItem);
	scene.Add(twoPlayersMenuItem);
	scene.Add(versusMenuItem);
	scene.Add(highScoresMenuItem);
	scene.Add(menuPointerItem);
	scene.Add(menu);


		//inputs
		auto& input = dae::InputManager::GetInstance();

	auto menuUp = std::make_unique<MenuMoveCommand>(menu.get(), MenuComponent::Direction::Up);
	auto menuDown = std::make_unique<MenuMoveCommand>(menu.get(), MenuComponent::Direction::Down);
	auto menuUpW = std::make_unique<MenuMoveCommand>(menu.get(), MenuComponent::Direction::Up);
	auto menuDownS = std::make_unique<MenuMoveCommand>(menu.get(), MenuComponent::Direction::Down);
	auto menuEnter = std::make_unique<MenuEnterCommand>(menu.get());

	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_W }, std::move(menuUpW));
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_S }, std::move(menuDownS));
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_UP }, std::move(menuUp));
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_DOWN }, std::move(menuDown));
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_RETURN }, std::move(menuEnter));

	input.AddController(0);

	input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::DPadUp }, std::move(menuUp));
	input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::DPadDown }, std::move(menuDown));
	input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::ButtonA }, std::move(menuEnter));
	
	//m_MenuCreated = true;
}
