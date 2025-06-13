#pragma once
#include "ObjectFactory.h"

#include "GalagaGame.h"
#include "GameManager.h"
#include "LivesDisplay.h"
#include "LivesComponent.h"
#include "PointsComponent.h"
#include "PointsDisplay.h"
#include "BulletComponent.h"
#include "ShootingComponent.h"


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
    std::function<void()> action)
{
    auto item = std::make_shared<dae::GameObject>();
    item->AddComponent<dae::TextComponent>(text, font);
    item->GetTransform().SetPosition(position);
    item->SetParent(menu->GetOwner(), false);
    item->AddComponent<ActionComponent>(action);
    menu->AddMenuItem(item);
    return item;
};

std::shared_ptr<dae::GameObject> ObjectFactory::CreateMenuArrow(
    const std::string& textureName,
    dae::MenuComponent* menu,
    const float scale
)
{
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
)
{
	auto item = std::make_shared<dae::GameObject>();
	auto texture = item->AddComponent<dae::TextureComponent>(fileName);
	auto logoSize = texture->GetTextureSize();
	item->GetTransform().SetPosition(position);
	item->GetTransform().SetScale(glm::vec3(scale, scale, scale));
	return item;
};

std::shared_ptr<dae::GameObject> ObjectFactory::CreatePlayer(std::string texture, glm::vec3 startPos, glm::vec3 scale)
{
	auto player = std::make_shared<dae::GameObject>();
	player->AddComponent<dae::TextureComponent>(texture);
	player->GetTransform().SetPosition(startPos);
	player->GetTransform().SetScale(scale);	

	/*auto livesComp = */player->AddComponent<dae::LivesComponent>();
	/*auto pointsComp = */player->AddComponent<dae::PointsComponent>();
	/*auto shootingComponent =*/ player->AddComponent<ShootingComponent>();

	return player;
}

std::shared_ptr<dae::GameObject> ObjectFactory::CreateLivesDisplay(std::string texture, std::shared_ptr<dae::GameObject> player, int lives, glm::vec3 position, glm::vec3 scale, bool mirror)
{
	auto livesDisplay = std::make_shared<dae::GameObject>();
	livesDisplay->AddComponent<dae::LivesDisplay>(texture, scale, mirror);
	livesDisplay->GetTransform().SetPosition(glm::vec3(position));

	auto livesComp = player->GetComponent<LivesComponent>(); //catch if no component detected
	livesComp->AddObserver(livesDisplay->GetComponent<LivesDisplay>());

	livesComp->SetLives(lives); //again so the display registers the change

	return livesDisplay;
}

std::shared_ptr<dae::GameObject> ObjectFactory::CreatePointsDisplay(std::shared_ptr<dae::GameObject> player, glm::vec3 position)
{
	auto pointsDisplay = std::make_shared<dae::GameObject>();
	pointsDisplay->AddComponent<dae::PointsDisplay>(); //add custom font to points display
	pointsDisplay->GetTransform().SetPosition(position);

	auto pointsComp = player->GetComponent<PointsComponent>(); //catch if no component detected
	pointsComp->AddObserver(pointsDisplay->GetComponent<PointsDisplay>());

	return pointsDisplay;
}

std::shared_ptr<dae::GameObject> ObjectFactory::CreateTextObject(std::shared_ptr<dae::Font> font, std::string text, glm::vec3 position, SDL_Color color )
{
	auto item = std::make_shared<dae::GameObject>();
	auto textComp = item->AddComponent<dae::TextComponent>(text, font, color);
	item->GetTransform().SetPosition(position);

	return item;
}


std::shared_ptr<dae::GameObject> ObjectFactory::CreateBullet(glm::vec3 position)
{
	float scale{ 2.0f };
	auto bullet = std::make_shared<dae::GameObject>();
	bullet->AddComponent<BulletComponent>(50.f, 1.5f);
	auto texture = bullet->AddComponent<TextureComponent>("fighter-bullet.png");

	glm::vec3 bulletPos = { position.x - (texture->GetTextureSize().x / 2) * scale, position.y - (texture->GetTextureSize().y/2 * scale) , position.z };
	bullet->GetTransform().SetPosition(bulletPos);

	
	bullet->GetTransform().SetScale({ scale,scale,scale });
	//TODO collider
		// Add Collider to game Object
		//bulletObject->AddComponent<dae::ColliderComponent>(bulletObject.get(), glm::vec2(6.f, 14.f));

	return bullet;

}
