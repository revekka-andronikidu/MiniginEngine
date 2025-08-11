#pragma once
#include "SceneFactory.h"
#include <SceneManager.h>
#include <GameManager.h>
#include "ObjectFactory.h"
#include <ResourceManager.h>
#include "BurgerTimeGame.h"
#include <TextureComponent.h>
#include <Scene.h>
#include <InputManager.h>
#include "GameCommands.h"
#include "GameScenes.h"

#include <fstream>
#include <iostream>
#include "json.hpp"
#include "GridComponent.h"
#include "PlayerComponent.h"

#include "IngredientComponent.h"



using namespace dae;

SceneFactory::SceneFactory()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	m_Game = dynamic_cast<BurgerTimeGame*>(game);
	if (!m_Game)
		throw std::runtime_error("Game Not Found");
};


void SceneFactory::CreateMainMenu()
{
	auto& scene = SceneManager::GetInstance().GetScene(SceneNames::MainMenu);

	auto& objFactory = ObjectFactory::GetInstance();

	auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 16); 

	auto menu = objFactory.CreateMainMenu();

	auto menuComp = menu.get()->GetComponent<MenuComponent>();;

	//auto background = objFactory.CreateTexture("bg_back2.png");

	float scale{ 1.f };
	auto logo = objFactory.CreateTexture("burgerTimeLogo.png", glm::vec3(0, 0, 0), scale);

	auto logoSize = logo.get()->GetComponent<TextureComponent>()->GetTextureSize();
	logo->GetTransform().SetPosition(glm::vec3(m_Game->m_WindowWidth / 2 - ((logoSize.x * scale) / 2), 70.f, 0));
	logo->GetTransform().SetScale(glm::vec3(scale, scale, scale));
	logo->SetParent(menu.get(), false);

	int gap{ 30 };
	auto onePlayerMenuItem = objFactory.CreateMenuItem("1 PLAYER", font, glm::vec3(m_Game->m_WindowWidth / 2 - 60.f, m_Game->m_WindowHeight / 2, 0.f), menuComp, [this]()
		{
			m_Game->m_GameModeMachine.EnterState<SinglePlayerMode>();
		});

	

	auto twoPlayersMenuItem = objFactory.CreateMenuItem("2 PLAYERS", font, glm::vec3(m_Game->m_WindowWidth / 2 - 60.f, m_Game->m_WindowHeight / 2 + gap, 0.f), menuComp, [this]()
		{
			m_Game->m_GameModeMachine.EnterState<CoopMode>();
		});

	auto versusMenuItem = objFactory.CreateMenuItem("VERSUS", font, glm::vec3(m_Game->m_WindowWidth / 2 - 60.f, m_Game->m_WindowHeight / 2 + gap * 2, 0.f), menuComp, [this]()
		{
			m_Game->m_GameModeMachine.EnterState<VersusMode>();
		});

	auto highScoresMenuItem = objFactory.CreateMenuItem("HIGH SCORES", font, glm::vec3(m_Game->m_WindowWidth / 2 - 60.f, m_Game->m_WindowHeight / 2 + gap * 3, 0.f), menuComp, [this]()
		{
			//EnterScene
			//m_Game->m_GameModeMachine.EnterState<HighScoresState>();
			//dae::SceneManager::GetInstance().SetActiveScene("HighScores");
		});

	auto menuPointerItem = objFactory.CreateMenuArrow("arrow.png", menuComp, 0.1f);


	//scene.Add(std::move(background));
	scene.Add(std::move(logo));
	scene.Add(std::move(onePlayerMenuItem));
	scene.Add(std::move(twoPlayersMenuItem));
	scene.Add(std::move(versusMenuItem));
	scene.Add(std::move(highScoresMenuItem));
	scene.Add(std::move(menuPointerItem));
	


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

	auto menuUpController = std::make_unique<MenuMoveCommand>(menu.get(), MenuComponent::Direction::Up);
	auto menuDownController = std::make_unique<MenuMoveCommand>(menu.get(), MenuComponent::Direction::Down);
	auto menuEnterController = std::make_unique<MenuEnterCommand>(menu.get());

	input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::DPadUp }, std::move(menuUpController));
	input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::DPadDown }, std::move(menuDownController));
	input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::ButtonA }, std::move(menuEnterController));

	scene.Add(std::move(menu));

}


void SceneFactory::CreateLevel(GameState::GameModeType mode, unsigned short stage)
{
	const auto path = dae::ResourceManager::GetInstance().GetDataPath();
	const std::string fileName = "level" + std::to_string(stage) + ".json";
	const auto fullPath = path / fileName;

	std::ifstream file(fullPath);
	if (!file.is_open()) {
		std::cerr << "Failed to open: " << fullPath << "\n";
		return;
	}

	nlohmann::json data;
	file >> data;

	std::string name{ "Stage" + std::to_string(stage) };

	auto& scene = SceneManager::GetInstance().GetScene(name);

	//Everething that has to be external
	int kRows{ 13 }; //grid rows
	int kCols{ 13 }; //grid cols

	int cellSize = m_Game->m_CellSize;
	int yOffset = m_Game->m_HUDSize;
	auto scale = m_Game->m_Scale;
	////////////////////////////////////////

	auto levelObject = std::make_unique<dae::GameObject>();
	auto gridComp = levelObject->AddComponent<GridComponent>(kRows, kCols);


	for (const auto& coord : data["ladder"]) 
	{
		int x = coord[0];
		int y = coord[1];

		gridComp->AddObjectToCell(x, y, CellObject::LADDER);

		auto posX = cellSize * x;
		auto posY = cellSize * y;

		auto ladder = ObjectFactory::GetInstance().CreateLadder({ posX,posY, 0 }, scale);
		ladder->SetParent(levelObject.get());

		scene.Add(std::move(ladder));
	}

	for (const auto& coord : data["offsetLadder"]) 
	{
		float xf = coord[0];
		int y = coord[1];

		int x = static_cast<int>(floor(xf));
		gridComp->AddObjectToCell(x, y, CellObject::OFFSETLADDER);

		auto posX = cellSize * xf;
		auto posY = cellSize * y;

		auto ladder = ObjectFactory::GetInstance().CreateLadder({ posX,posY, 0 }, scale);
		ladder->SetParent(levelObject.get());

		scene.Add(std::move(ladder));
	}

	for (const auto& coord : data["short"]) 
	{
		int x = coord[0];
		int y = coord[1];

		auto posX = cellSize * x;
		auto posY = cellSize * y;

		auto platform = ObjectFactory::GetInstance().CreateShortPlatform({ posX,posY, 0 }, scale);
		platform->SetParent(levelObject.get());

		scene.Add(std::move(platform));

		gridComp->AddObjectToCell(x, y, CellObject::PLATFORM);
	}

	for (const auto& coord : data["longLeft"]) 
	{
		int x = coord[0];
		int y = coord[1];

		gridComp->AddObjectToCell(x, y, CellObject::PLATFORM);
		gridComp->AddObjectToCell(x + 1, y, CellObject::PLATFORM);

		auto posX = cellSize * x;
		auto posY = cellSize * y;

		auto platformL = ObjectFactory::GetInstance().CreateLongLeftPlatform({ posX,posY, 0 }, scale);
		auto platformR = ObjectFactory::GetInstance().CreateLongRightPlatform({ posX + cellSize ,posY, 0 }, scale);

		platformL->SetParent(levelObject.get());
		platformR->SetParent(levelObject.get());

		scene.Add(std::move(platformL));
		scene.Add(std::move(platformR));
	}

	int burgerIndex = 0;
	for (const auto& burger : data["burgers"])
	{
		// Step 1: Copy ingredients and sort by Y position descending (so bottom is first)
		std::vector<nlohmann::json> sortedIngredients = burger;
		std::sort(sortedIngredients.begin(), sortedIngredients.end(),
			[](const nlohmann::json& a, const nlohmann::json& b)
			{
				return a["position"][1] > b["position"][1]; // descending Y
			});

		// Step 2: Loop in reverse (bottom to top), assign index from top (0) to bottom (N-1)
		int numPieces = static_cast<int>(sortedIngredients.size());
		for (int i = 0; i < numPieces; ++i)
		{
			const auto& piece = sortedIngredients[numPieces - 1 - i]; // top piece first

			int x = piece["position"][0];
			int y = piece["position"][1];
			IngredientType type = static_cast<IngredientType>(piece["ingredientType"]);

			glm::vec3 pos = { cellSize * x, cellSize * y + cellSize/2 - 2*scale.y, 0.0f };
			int index = i;

			auto ingredient = ObjectFactory::GetInstance().CreateIngredient(pos, scale, type);
			ingredient->SetParent(levelObject.get());


			auto comp = ingredient.get()->GetComponent<IngredientComponent>();

			auto pieces = comp->GetPieces();

			for (auto piece : pieces)
			{
				scene.Add(piece);
			}

			scene.Add(std::move(ingredient));
		}

		++burgerIndex;
	}

	for (const auto& coord : data["tray"]) 
	{
		int x = coord[0];
		int y = coord[1];

		gridComp->AddObjectToCell(x + 1, y, CellObject::TRAY);
		gridComp->AddObjectToCell(x + 2, y, CellObject::TRAY);


		auto posX = cellSize * x;
		auto posY = cellSize * y;

		auto tray = ObjectFactory::GetInstance().CreateTray({ posX,posY, 0 }, scale);
		tray->SetParent(levelObject.get());

		scene.Add(std::move(tray));
	}

	if (!data.contains("peter") || data["peter"].empty()) {
		std::cerr << "No 'peter' data found\n";
		return;
	}
	else
	{
		auto coord = data["peter"][0];
		float x = coord[0];
		float y = coord[1];

		std::cout << "Peter is at: (" << x << ", " << y << ")\n";

		auto posX = cellSize * x;
		auto posY = cellSize * y;

		auto player = ObjectFactory::GetInstance().CreatePlayer({ posX,posY, 0 }, scale);
		auto playerComponent = player->GetComponent<PlayerComponent>();
		playerComponent->SetGrid(gridComp);

		auto& input = dae::InputManager::GetInstance();

		auto moveUp = std::make_unique<MoveCommand>(player.get(), Direction::Up, 0, 600, 150.f);
		auto moveDown = std::make_unique<MoveCommand>(player.get(), Direction::Down, 0, 600, 150.f);
		auto moveLeft = std::make_unique<MoveCommand>(player.get(), Direction::Left, 0, 600, 150.f);
		auto moveRight = std::make_unique<MoveCommand>(player.get(), Direction::Right, 0, 600, 150.f);

		auto stop = std::make_unique<StopMove>(player.get());
		auto stop1 = std::make_unique<StopMove>(player.get());
		auto stop2 = std::make_unique<StopMove>(player.get());
		auto stop3 = std::make_unique<StopMove>(player.get());



		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_D }, std::move(moveRight));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_A }, std::move(moveLeft));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_S }, std::move(moveDown));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_W }, std::move(moveUp));

		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_D }, std::move(stop1));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_A }, std::move(stop));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_S }, std::move(stop2));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyUp, SDL_SCANCODE_W }, std::move(stop3));

		player->SetParent(levelObject.get());
		scene.Add(std::move(player));

	}

	auto pos = levelObject->GetTransform().GetWorldPosition();
	pos.y += yOffset;
	levelObject->GetTransform().SetPosition(pos);
	scene.Add(std::move(levelObject));

	auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 8);
	auto text = ObjectFactory::GetInstance().CreateText("1UP", font, { 255,0,0,255 }, { cellSize/2, 0, 0 }, scale);
	auto points = ObjectFactory::GetInstance().CreatePointsDisplay({ cellSize, cellSize/2, 0}, scale);
	scene.Add(std::move(points));
	scene.Add(std::move(text));
}