#pragma once
#include "SceneFactory.h"
#include "ObjectFactory.h"
#include "SceneNames.h"
#include "GalagaGame.h"
#include "InputManager.h"
#include "GameCommands.h"
#include "LivesDisplay.h"


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

			m_Galaga->m_GameModeMachine.EnterState<SinglePlayerMode>();
		});
	auto twoPlayersMenuItem = objFactory.CreateMenuItem("2 PLAYERS", font, glm::vec3(GalagaGame::m_GameWindowWidth / 2 - 60.f, GalagaGame::m_GameWidnowHeight / 2 + gap, 0.f), menuComp, [this]()
		{
			m_Galaga->m_GameModeMachine.EnterState<CoopMode>();
		});

	auto versusMenuItem = objFactory.CreateMenuItem("VERSUS", font, glm::vec3(GalagaGame::m_GameWindowWidth / 2 - 60.f, GalagaGame::m_GameWidnowHeight / 2 + gap * 2, 0.f), menuComp, [this]()
		{
			m_Galaga->m_GameModeMachine.EnterState<VersusMode>();
		});

	auto highScoresMenuItem = objFactory.CreateMenuItem("HIGH SCORES", font, glm::vec3(GalagaGame::m_GameWindowWidth / 2 - 60.f, GalagaGame::m_GameWidnowHeight / 2 + gap * 3, 0.f), menuComp, [this]()
		{
			//EnterScene
			m_Galaga->m_GameModeMachine.EnterState<GameModeNull>();
			m_Galaga->m_StateMachine.EnterState<HighScoresState>();
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

void SceneFactory::CreateStageScene(std::string sceneName)
{
	auto& scene = SceneManager::GetInstance().GetScene(sceneName);
	auto& objFactory = ObjectFactory::GetInstance();

	// 1. Load level data from file (pseudo-code)
	//LevelData data = LevelLoader::Load(sceneName + ".txt");
	
	//glm::vec3 startPos{ glm::vec3(256.f, 300.f, 0) };
	//std::string texture{ "galaga.png" };
	//glm::vec3 scale{ 0.5f, 0.5f, 0.5f };


	// 2. Create background
	auto background = objFactory.CreateTexture("bg_back2.png");
	scene.Add(background);

	

	// 3. Create players based on game mode
	if (sceneName.find("Solo") != std::string::npos) 
	{
		//scene.Add(objFactory.CreatePlayer(texture, startPos, scale));
		CreatePlayer(sceneName, 1);
	}

	else if (sceneName.find("Coop") != std::string::npos) {
		CreatePlayer(sceneName, 1);
		CreatePlayer(sceneName, 2);
	}
	// ... Versus logic

	// 4. Create enemies from level data
	//for (const auto& enemySpawn : data.enemies) {
	//	scene.Add(CreateEnemy(enemySpawn.type, enemySpawn.position));
	auto bee = ObjectFactory::GetInstance().CreateBee(glm::vec3{ 200, 80, 0 });

	scene.Add(bee);

	//// 5. Create HUD elements
	//scene.Add(CreateLivesDisplay());
	//scene.Add(CreatePointsDisplay());
}

void SceneFactory::CreatePlayer(std::string sceneName, int playerNumber)
{
	auto& scene = SceneManager::GetInstance().GetScene(sceneName);

	std::string texture{};
	if (playerNumber == 2)
	{
		texture = "galagaRed.png" ;
	}
	else
	{
		texture = "galaga.png" ; // change if player 2
	}

	glm::vec3 scale{ 0.5f, 0.5f, 0.5f };	
	int height = m_Galaga->m_GameWidnowHeight;
	int width = m_Galaga->m_GameWindowWidth;
	int lives = 4; // get from galaga

	glm::vec3 startPos;
	if (playerNumber == 2)
	{
		startPos = { glm::vec3(width / 2 + 80, height - 80, 0) }; //load from file?
	}
	else
	{
		startPos = { glm::vec3(width / 2, height - 80, 0) };
	}

	auto& objFactory = ObjectFactory::GetInstance();


	//PLAYER
	auto player = objFactory.CreatePlayer(texture, startPos, scale);
	
	//LIVES DISPLAY
	std::shared_ptr<GameObject> livesDisplay;
	glm::vec3 position;
	glm::ivec2 logoSize;
	int padding{ 10 };

	if (playerNumber == 2)
	{
		livesDisplay = objFactory.CreateLivesDisplay(texture, player, lives, { 0,0,0 }, scale, true);
		logoSize = livesDisplay.get()->GetComponent<LivesDisplay>()->GetTextureSize();

		position = { width - (logoSize.x * scale.x)- padding, height - (logoSize.y * scale.y), 0 };
	}
	else
	{
		livesDisplay = objFactory.CreateLivesDisplay(texture, player, lives, { 0,0,0 }, scale);
		logoSize = livesDisplay.get()->GetComponent<LivesDisplay>()->GetTextureSize();

		position = { padding, height - (logoSize.y * scale.y), 0 };
	}
	livesDisplay->GetTransform().SetPosition(position);

	
	//POINTS DISSPLAY
	auto font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);
	auto textPoints = objFactory.CreateTextObject(font, "1UP", { 0,0,0 }, { 255,0,0,255 });
	glm::vec3 pos{0, 20, 0};
	auto pointsDisplay = objFactory.CreatePointsDisplay(player, pos);


	//Add to scene
	scene.Add(player);
	scene.Add(livesDisplay);
	scene.Add(pointsDisplay);
	scene.Add(textPoints);


	//INPUT
	auto& input = dae::InputManager::GetInstance();
	if (playerNumber == 1)
	{
		auto moveRight = std::make_unique<MoveCommand>(player.get(), glm::vec3{ 1, 0, 0 }, 0, m_Galaga->m_GameWindowWidth - (logoSize.x * scale.x), 150.f);
		auto moveLeft = std::make_unique<MoveCommand>(player.get(), glm::vec3{ -1, 0, 0 }, 0, m_Galaga->m_GameWindowWidth, 150.f);
		auto shoot = std::make_unique<ShootCommand>(player.get());
		

		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_D }, std::move(moveRight));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyPressed, SDL_SCANCODE_A }, std::move(moveLeft));
		input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyDown, SDL_SCANCODE_SPACE }, std::move(shoot));

		input.AddController(0);
		//input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyPressed, XboxController::ControllerButton::DPadRight }, std::move(moveRight));
		//input.BindSceneInput(&scene, ControllerInput{ ButtonState::KeyPressed, XboxController::ControllerButton::DPadLeft }, std::move(moveLeft));

	}




#if _DEBUG
	auto removeLive = std::make_unique<LooseLiveCommand>(player.get());
	input.BindSceneInput(&scene,KeyboardInput{ ButtonState::KeyUp,SDL_SCANCODE_X }, std::move(removeLive));
	auto increasePoints = std::make_unique<AddScoreCommand>(player.get(), 100);
	input.BindSceneInput(&scene, KeyboardInput{ ButtonState::KeyUp,SDL_SCANCODE_C }, std::move(increasePoints));
#endif
}

