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

#include <fstream>
#include <iostream>
#include "json.hpp"
#include "GridComponent.h"
#include "PlayerComponent.h"

#include "IngredientComponent.h"

#include "PeppersDisplay.h"
#include "LivesDisplay.h"
#include "PointsDisplay.h"
#include "HighScoresManager.h"
#include "KeypadComponent.h"
#include "AnimatedTextComponent.h"

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
	auto& scene = SceneManager::GetInstance().CreateScene(SceneNames::MainMenu);
	auto& objFactory = ObjectFactory::GetInstance();
	auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 8); 
	auto menu = objFactory.CreateMainMenu();
	auto menuComp = menu.get()->GetComponent<MenuComponent>();;

	//auto background = objFactory.CreateTexture("bg_back2.png");

	float scale{ 1.f };
	auto logo = objFactory.CreateTexture("burgerTimeLogo.png", glm::vec3(0, 0, 0), scale);

	auto logoSize = logo.get()->GetComponent<TextureComponent>()->GetTextureSize();
	logo->GetTransform().SetPosition(glm::vec3((GameSettings::windowWidth * GameSettings::scale.x) / 2 - ((logoSize.x * scale) / 2), 70.f, 0));
	logo->GetTransform().SetScale(glm::vec3(scale, scale, scale));
	logo->SetParent(menu.get(), false);

	glm::vec3 position{ GameSettings::windowWidth * GameSettings::scale.x / 2, GameSettings::windowHeight * GameSettings::scale.y / 3, 0.f };

	int gap{ static_cast<int>(GameSettings::cellSize * GameSettings::scale.y) };
	auto onePlayerMenuItem = objFactory.CreateMenuItem("1 PLAYER", font, position, menuComp, [this]()
		{
			m_Game->m_SelectedGameMode = GameMode::Solo;
			m_Game->m_GameModeMachine.EnterState<InGameState>(m_Game->m_SelectedGameMode);
		});

		auto twoPlayersMenuItem = objFactory.CreateMenuItem("2 PLAYERS", font, glm::vec3(position.x,  position.y + gap, 0.f), menuComp, [this]()
		{
				m_Game->m_SelectedGameMode = GameMode::Coop;
				m_Game->m_GameModeMachine.EnterState<InGameState>(m_Game->m_SelectedGameMode);
		});

	auto versusMenuItem = objFactory.CreateMenuItem("VERSUS", font, glm::vec3(position.x, position.y + gap * 2, 0.f), menuComp, [this]()
		{
			m_Game->m_SelectedGameMode = GameMode::Versus;
			m_Game->m_GameModeMachine.EnterState<InGameState>(m_Game->m_SelectedGameMode);
		});

	auto highScoresMenuItem = objFactory.CreateMenuItem("HIGH SCORES", font, glm::vec3(position.x, position.y + gap * 3, 0.f), menuComp, [this]()
		{
			//EnterScene
			m_Game->m_GameModeMachine.EnterState<HighScoresState>();
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


void SceneFactory::CreateLevel(unsigned short stage)
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

	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);
	int yOffset = static_cast<int>(GameSettings::HUDSize * GameSettings::scale.y);
	auto scale = GameSettings::scale;
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

	int numPieces{0};
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
		numPieces = static_cast<int>(sortedIngredients.size());
		for (int i = 0; i < numPieces; ++i)
		{
			const auto& piece = sortedIngredients[numPieces - 1 - i]; // top piece first

			int x = piece["position"][0];
			int y = piece["position"][1];

			IngredientType type = static_cast<IngredientType>(piece["ingredientType"]);

			glm::vec3 pos = { cellSize * x, cellSize * y + cellSize/2 - 2*scale.y, 0.0f };
			auto ingredient = ObjectFactory::GetInstance().CreateIngredient(pos, scale, type);
			ingredient->SetParent(levelObject.get());


			//Add pieces to the scene
			auto comp = ingredient.get()->GetComponent<IngredientComponent>();
			auto pieces = comp->GetPieces();
			for (auto piece : pieces)
			{
				scene.Add(piece);
			}
			scene.Add(std::move(ingredient));
		}
	}

	for (const auto& coord : data["tray"]) 
	{
		int x = coord[0];
		int y = coord[1];

		auto posX = cellSize * x;
		auto posY = cellSize * y;

		auto tray = ObjectFactory::GetInstance().CreateTray({ posX,posY, 0 }, scale, numPieces);
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

		auto moveUp = std::make_unique<MoveCommand>(player.get(), Direction::Up);
		auto moveDown = std::make_unique<MoveCommand>(player.get(), Direction::Down);
		auto moveLeft = std::make_unique<MoveCommand>(player.get(), Direction::Left);
		auto moveRight = std::make_unique<MoveCommand>(player.get(), Direction::Right);

		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_D }, std::move(moveRight));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_A }, std::move(moveLeft));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_S }, std::move(moveDown));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_W }, std::move(moveUp));

		player->SetParent(levelObject.get());
		scene.Add(std::move(player));

	}

	auto pos = levelObject->GetTransform().GetWorldPosition();
	pos.y += yOffset;
	pos.x += GameSettings::xOffset;
	levelObject->GetTransform().SetPosition(pos);
	scene.Add(std::move(levelObject));


	CreateHUD(scene);
}

void SceneFactory::CreateHUD(Scene& scene)
{
	//auto& scene = SceneManager::GetInstance().GetScene(SceneNames::Stage1);

	int cellSize = static_cast<int>(GameSettings::cellSize * GameSettings::scale.x);

	auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 8);
	auto text = ObjectFactory::GetInstance().CreateText("1UP", font, { 255,0,0,255 }, TextComponent::TextAlign::Left, { cellSize / 2, 0, 0 }, GameSettings::scale);
	auto points = ObjectFactory::GetInstance().CreatePointsDisplay({ cellSize * 2 + cellSize/2, cellSize / 2, 0}, GameSettings::scale);
	points->GetComponent<PointsDisplay>()->SetPoints(m_Game->m_Score);

	//HISCORE DISPLAY
	if (m_Game->m_HighScore != 0)
	{
		auto HItext = ObjectFactory::GetInstance().CreateText("HI-SCORE", font, { 255,0,0,255 }, TextComponent::TextAlign::Center, { GameSettings::windowWidth / 2 * GameSettings::scale.x, 0, 0 }, GameSettings::scale);
		auto HIScore = ObjectFactory::GetInstance().CreateText(std::to_string(m_Game->m_HighScore), font, { 255,255,255,255 }, TextComponent::TextAlign::Center, { GameSettings::windowWidth / 2 * GameSettings::scale.x, GameSettings::cellSize/2 * GameSettings::scale.y, 0 }, GameSettings::scale);
	
		scene.Add(std::move(HItext));
		scene.Add(std::move(HIScore));
	}
	
	//numbers here
	// 
	

	//LIVES
	auto livedDisplay = std::make_unique<dae::GameObject>();
	auto livesComp = livedDisplay.get()->AddComponent<LivesDisplay>();
	livesComp->SetLives(m_Game->m_PlayerLives);

	glm::vec3 pos = { 0, GameSettings::windowHeight * GameSettings::scale.y, 0 };
	livedDisplay.get()->GetTransform().SetPosition(pos);
	livedDisplay.get()->GetTransform().SetScale(GameSettings::scale);

	//PEPPERS
	auto peppersDisplay = std::make_unique<dae::GameObject>();
	auto peppersComp = peppersDisplay.get()->AddComponent<PeppersDisplay>();
	peppersComp->SetPeppers(m_Game->m_Peppers);
	pos = { (GameSettings::windowWidth - GameSettings::xOffset/2) * GameSettings::scale.x , 0, 0 };
	peppersDisplay.get()->GetTransform().SetPosition(pos);
	peppersDisplay.get()->GetTransform().SetScale(GameSettings::scale);


	

	scene.Add(std::move(points));
	scene.Add(std::move(text));
	scene.Add(std::move(livedDisplay));
	scene.Add(std::move(peppersDisplay));
}

void SceneFactory::CreateHighScores()
{
	auto& scene = SceneManager::GetInstance().GetScene(SceneNames::HighScores);
	auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 8);

	auto burgerTime = ObjectFactory::GetInstance().CreateText("BURGER TIME", font, { 255,0,0,255 }, TextComponent::TextAlign::Center, { GameSettings::windowWidth / 2 * GameSettings::scale.x, (GameSettings::windowHeight / 4 - GameSettings::cellSize*2)* GameSettings::scale.y, 0 }, GameSettings::scale);
	auto bestPlayers = ObjectFactory::GetInstance().CreateText("BEST FIVE PLAYERS", font, { 255,255,0,255 }, TextComponent::TextAlign::Center, { GameSettings::windowWidth / 2 * GameSettings::scale.x, GameSettings::windowHeight / 4 * GameSettings::scale.y, 0 }, GameSettings::scale);


	auto& hs = HighScoreManager::GetInstance();
	auto scores = hs.GetScores();

	glm::vec3 pos{ GameSettings::cellSize * 2 * GameSettings::scale.x, 0, 0 };
	int yoffset = GameSettings::cellSize * GameSettings::scale.y;

	for (int i = 0; i < 5; ++i)
	{
		int offset = yoffset * (i + 1);
		pos.y = bestPlayers.get()->GetTransform().GetLocalPosition().y + GameSettings::cellSize * GameSettings::scale.y + offset;

		// Default values if not enough scores
		std::string name = "XXX";
		int points = 0;

		if (i < static_cast<int>(scores.size()))
		{
			name = scores[i].name;
			points = scores[i].score;
		}

		// Create texts
		auto numberText = ObjectFactory::GetInstance().CreateText(
			std::to_string(i + 1),
			font,
			{ 255,255,255,255 },
			TextComponent::TextAlign::Left,
			pos,
			GameSettings::scale
		);

		// Name
		auto nameText = ObjectFactory::GetInstance().CreateText(
			name,
			font,
			{ 255,255,255,255 },
			TextComponent::TextAlign::Left,
			{ pos.x + (GameSettings::cellSize *GameSettings::scale.x), pos.y, pos.z },
			GameSettings::scale
		);

		// Points
		auto pointsText = ObjectFactory::GetInstance().CreateText(
			std::to_string(points),
			font,
			{ 255,255,255,255 },
			TextComponent::TextAlign::Right,
			{ (GameSettings::windowWidth - GameSettings::cellSize * 4) * GameSettings::scale.x, pos.y, pos.z },
			GameSettings::scale
		);

		// PTS label
		auto ptsLabel = ObjectFactory::GetInstance().CreateText(
			"PTS",
			font,
			{ 255,255,255,255 },
			TextComponent::TextAlign::Left,
			{ (GameSettings::windowWidth - GameSettings::cellSize * 4 + GameSettings::cellSize/4) * GameSettings::scale.x, pos.y, pos.z },
			GameSettings::scale
		);

		// Add all to scene
		scene.Add(std::move(numberText));
		scene.Add(std::move(nameText));
		scene.Add(std::move(pointsText));
		scene.Add(std::move(ptsLabel));
	}
	auto back = ObjectFactory::GetInstance().CreateText("PRESS ENTER/B TO RETURN", font, { 255,255,255,255 }, TextComponent::TextAlign::Center, { GameSettings::windowWidth / 2 * GameSettings::scale.x, (GameSettings::windowHeight - GameSettings::windowHeight / 4 + GameSettings::cellSize) * GameSettings::scale.y, 0 }, GameSettings::scale);
	auto menu = ObjectFactory::GetInstance().CreateText("TO THE MAIN MENU", font, { 255,255,255,255 }, TextComponent::TextAlign::Center, { GameSettings::windowWidth / 2 * GameSettings::scale.x, (GameSettings::windowHeight - GameSettings::windowHeight / 4 + GameSettings::cellSize * 2) * GameSettings::scale.y, 0 }, GameSettings::scale);

	auto& input = dae::InputManager::GetInstance();
	auto menuBack = std::make_unique<BackToMenuCommand>();
	auto menuBackC = std::make_unique<BackToMenuCommand>();

	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_RETURN }, std::move(menuBack));
	//input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyDown, XboxController::ControllerButton::ButtonB }, std::move(menuBackC));

	scene.Add(std::move(back));
	scene.Add(std::move(menu));
	scene.Add(std::move(bestPlayers));	
	scene.Add(std::move(burgerTime));
}

void SceneFactory::CreateHighScoreEntry()
{
	auto& scene = SceneManager::GetInstance().GetScene(SceneNames::HighScoreEntry);
	auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 8);

	auto keypad = ObjectFactory::GetInstance().CreateHighScoreEntryKeypad(glm::vec3(GameSettings::xOffset / 4 * GameSettings::scale.x, 0, 0));
	auto keypadComp = keypad.get()->GetComponent<KeypadComponent>();

	auto arrow = std::make_shared<dae::GameObject>();
	arrow->SetParent(keypad.get(), true);
	arrow->AddComponent<dae::TextureComponent>("HIarrow.png");
	arrow->GetTransform().SetScale({ GameSettings::scale.x/2, GameSettings::scale.y/2,0 });
	keypadComp->AddMenuArrow(arrow);
	
	
	auto& input = dae::InputManager::GetInstance();

	auto menuUp = std::make_unique<KeypadMoveCommand>(keypad.get(), Direction::Up);
	auto menuDown = std::make_unique<KeypadMoveCommand>(keypad.get(), Direction::Down);
	auto menuLeft = std::make_unique<KeypadMoveCommand>(keypad.get(), Direction::Left);
	auto menuRight = std::make_unique<KeypadMoveCommand>(keypad.get(), Direction::Right);
	auto menuEnter = std::make_unique<KeypadSelectCommand>(keypad.get());

	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_W }, std::move(menuUp));
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_S }, std::move(menuDown));
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_D }, std::move(menuRight));
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_A }, std::move(menuLeft));
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_RETURN }, std::move(menuEnter));

	
	scene.Add(std::move(keypad));
	scene.Add(std::move(arrow));

	auto& hs = HighScoreManager::GetInstance();
	auto scores = hs.GetScores(); 

	// Create the new entry with placeholder name
	//int playerScore{ m_Game->m_Score };
	std::string currentName = "_____";
	HighScore newEntry;
	newEntry.score = m_Game->m_Score;

	auto it = std::find_if(scores.begin(), scores.end(), [&](const HighScore& s) {
		return newEntry.score > s.score;
		});

	// Find where our new entry will be inserted BEFORE inserting
	int insertIndex = static_cast<int>(std::distance(scores.begin(), it));

	// Now safe to insert
	scores.insert(it, newEntry);

	// Trim to top 5
	if (scores.size() > 5) {
		scores.resize(5);
	}

	glm::vec3 pos{ GameSettings::cellSize / 2 * 5 * GameSettings::scale.x, (GameSettings::cellSize / 2 * 19 - 2) * GameSettings::scale.y, 0 };
	int yOffset = GameSettings::cellSize * GameSettings::scale.y;

	int pointsOffset = (GameSettings::cellSize / 2 * 25);

	for (int i = 0; i < 5; ++i)
	{
		std::string name = "XXX";
		std::string points = "0";

		if (i < scores.size())
		{
			name = scores[i].name;
			points = std::to_string(scores[i].score);
		}

		// If this is the slot we are entering, replace name with currentName
		if (i == insertIndex) 
		{
			//name diplayed here
			auto nameObj = std::make_unique<dae::GameObject>();
			nameObj.get()->GetTransform().SetScale(GameSettings::scale);
			auto nameTextComp = nameObj->AddComponent<dae::TextComponent>(
				currentName, font, SDL_Color{ 255, 255, 255, 255 },
				TextComponent::TextAlign::Left
			);
			nameObj->GetTransform().SetPosition({ pos.x, pos.y + yOffset * i, 0 });
			scene.Add(std::move(nameObj));

			// store a pointer in keypad so it can update while typing
			keypadComp->SetTextComponent(nameTextComp);

		
			
			auto pointsText = ObjectFactory::GetInstance().CreateText(points + " PTS", font, { 255,255,255,255 }, TextComponent::TextAlign::Right, glm::vec3((pointsOffset)*GameSettings::scale.x, pos.y + yOffset * i, pos.z), GameSettings::scale);
			scene.Add(std::move(pointsText));
			continue;
		}

		auto nameText = ObjectFactory::GetInstance().CreateText(name, font, { 255,255,255,255 }, TextComponent::TextAlign::Left, { pos.x, pos.y + yOffset * i, 0 }, GameSettings::scale);
		auto pointsText = ObjectFactory::GetInstance().CreateText(points + " PTS", font, { 255,255,255,255 }, TextComponent::TextAlign::Right, glm::vec3((pointsOffset) * GameSettings::scale.x, pos.y + yOffset * i, pos.z), GameSettings::scale);

		scene.Add(std::move(nameText));
		scene.Add(std::move(pointsText));

	}
}

void SceneFactory::CreateGameOver()
{
	auto& scene = SceneManager::GetInstance().CreateScene(SceneNames::GameOver);
	auto font = ResourceManager::GetInstance().GetFont("emulogic.ttf", 8);

	glm::vec3 pos{ GameSettings::windowWidth / 2 * GameSettings::scale.x,(GameSettings::windowHeight / 2 - GameSettings::cellSize * 2) * GameSettings::scale.y, 0 } ;
	glm::vec3 scale{GameSettings::scale.x * 2, GameSettings::scale.y * 2, 0 };

	auto obj = ObjectFactory::GetInstance().CreateAnimatedText("GAME OVER", 0.3f, font, pos, scale, SDL_Color{ 255, 255, 255, 255 }, TextComponent::TextAlign::Center);
	auto anim = obj->GetComponent<AnimatedTextComponent>();
	anim->Restart();

	scene.Add(std::move(obj));
}