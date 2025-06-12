#pragma once
#include "ObjectFactory.h"

#include "GalagaGame.h"
#include "GameManager.h"
#include "LivesDisplay.h"
#include "LivesComponent.h"
#include "PointsComponent.h"
#include "PointsDisplay.h"


using namespace dae;

ObjectFactory::ObjectFactory()
{
	auto game = GameManager::GetInstance().GetActiveGame();
	m_Galaga = dynamic_cast<GalagaGame*>(game);
	if (!m_Galaga)
		throw std::runtime_error("Game Not Found");

};

std::shared_ptr<dae::GameObject> ObjectFactory::CreateMenuItem(
    const std::string& text,
    std::shared_ptr<Font> font,
    const glm::vec3& position,
    dae::MenuComponent* menu,
    //dae::Scene& scene,
    std::function<void()> action)
{
    auto item = std::make_shared<dae::GameObject>();
    item->AddComponent<dae::TextComponent>(text, font);
    item->GetTransform().SetPosition(position);
    item->SetParent(menu->GetOwner(), false);
    item->AddComponent<ActionComponent>(action);
    menu->AddMenuItem(item);
    //scene.Add(item);
    return item;
};

std::shared_ptr<dae::GameObject> ObjectFactory::CreateMenuArrow(
    const std::string& textureName,
    dae::MenuComponent* menu,
    const float scale
    // dae::Scene& scene
)
{
    //menu pointer item
    auto item = std::make_shared<dae::GameObject>();
    item->SetParent(menu->GetOwner(), false);
    item->AddComponent<dae::TextureComponent>(textureName);
    item->GetTransform().SetScale(glm::vec3(scale, scale, scale));
    menu->AddMenuArrow(item);
    return item;
};

std::shared_ptr<dae::GameObject> ObjectFactory::CreateMainMenu()
{
	auto mainMenu = std::make_shared<dae::GameObject>();
	auto menu = mainMenu->AddComponent<dae::MenuComponent>();

	return mainMenu;
}

std::shared_ptr<dae::GameObject> ObjectFactory::CreateTexture(
	const std::string& fileName,
	const glm::vec3& position, 
	const float scale
	// dae::Scene& scene
)
{
	auto item = std::make_shared<dae::GameObject>();
	auto texture = item->AddComponent<dae::TextureComponent>(fileName);
	auto logoSize = texture->GetTextureSize();
	item->GetTransform().SetPosition(position);
	item->GetTransform().SetScale(glm::vec3(scale, scale, scale));
	return item;
};

std::shared_ptr<dae::GameObject> ObjectFactory::CreatePlayer()
{
	//from init list
	//start position ? from initialor list
	//player number
	glm::vec3 startPos{ glm::vec3(256.f, 300.f, 0) };
	std::string texture{"galaga.png"};
	glm::vec3 scale{ 0.5f, 0.5f, 0.5f };
	//pass game info
	int lives{ 3 };




	auto player = std::make_shared<dae::GameObject>();
	player->AddComponent<dae::TextureComponent>(texture);
	player->GetTransform().SetPosition(startPos);
	player->GetTransform().SetScale(scale);	

	auto livesComp = player->AddComponent<dae::LivesComponent>(lives);
	auto pointsComp = player->AddComponent<dae::PointsComponent>();

	
	

	return player;

}

std::shared_ptr<dae::GameObject> ObjectFactory::CreateLivesDisplay()
{
	//make from initiliyer list
	std::string texture{ "galaga.png" };
	glm::vec3 scale{ 0.5f, 0.5f, 0.5f };
	glm::vec3 position{ 0, 440, 0 };
	std::shared_ptr<dae::GameObject> player{};
	int lives = 3;
	////////

	auto livesDisplay = std::make_shared<dae::GameObject>();
	livesDisplay->AddComponent<dae::LivesDisplay>(texture, scale);
	livesDisplay->GetTransform().SetPosition(glm::vec3(0, 440, 0));

	auto livesComp = player->GetComponent<LivesComponent>(); //catch if no component detected
	livesComp->AddObserver(livesDisplay->GetComponent<LivesDisplay>());

	livesComp->SetLives(lives); //again so the display registers the change
	
	//later elsewhere 
	auto& scene = SceneManager::GetInstance().GetScene("SoloStage1");

	return livesDisplay;
}

std::shared_ptr<dae::GameObject> ObjectFactory::CreatePointsDisplay()
{
	//make from initiliyer list
	//std::string texture{ "galaga.png" };
	//glm::vec3 scale{ 0.5f, 0.5f, 0.5f };
	glm::vec3 position{200, 300, 0 };
	std::shared_ptr<dae::GameObject> player{};
	
	////////
	auto pointsDisplay = std::make_shared<dae::GameObject>();
	pointsDisplay->AddComponent<dae::PointsDisplay>(); //add custom font to points display
	pointsDisplay->GetTransform().SetPosition(position);

	auto pointsComp = player->GetComponent<PointsComponent>(); //catch if no component detected
	pointsComp->AddObserver(pointsDisplay->GetComponent<PointsDisplay>());


	//later elsewhere 
	auto& scene = SceneManager::GetInstance().GetScene("SoloStage1");

	return pointsDisplay;
}

