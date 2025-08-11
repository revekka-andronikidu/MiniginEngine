#include "ObjectFactory.h"
#include <TextureComponent.h>
#include <TextComponent.h>
#include <ActionComponent.h>
#include <Font.h>
#include "LivesComponent.h"
#include "PointsComponent.h"
#include "PointsDisplay.h"
#include <ColliderComponent.h>
#include <SpriteSheetComponent.h>
#include "PlayerComponent.h"
#include "IngredientComponent.h"
#include "IngredientPieceComponent.h"
#include <SDL_ttf.h>
#include "TrayComponent.h"

using namespace dae;

std::unique_ptr<dae::GameObject> ObjectFactory::CreateTexture(
	const std::string& fileName,
	const glm::vec3& position,
	const float scale
)
{
	auto item = std::make_unique<dae::GameObject>();
	auto texture = item->AddComponent<dae::TextureComponent>(fileName);
	auto logoSize = texture->GetTextureSize();
	item->GetTransform().SetPosition(position);
	item->GetTransform().SetScale(glm::vec3(scale, scale, scale));
	return item;
};

std::unique_ptr<dae::GameObject> ObjectFactory::CreateText(const std::string& text, std::shared_ptr<Font> font, SDL_Color color, const glm::vec3& position,glm::vec3 scale)
{
	auto item = std::make_unique<dae::GameObject>();
	item->AddComponent<dae::TextComponent>(text,font, color);
	
	item->GetTransform().SetPosition(position);
	item->GetTransform().SetScale(glm::vec3(scale));
	return item;
}

std::unique_ptr <dae::GameObject> ObjectFactory::CreateMainMenu()
{
	auto mainMenu = std::make_unique<dae::GameObject>();
	auto menu = mainMenu->AddComponent<dae::MenuComponent>();

	return mainMenu;
}

std::shared_ptr<dae::GameObject> ObjectFactory::CreateMenuItem(
    const std::string& text, 
    std::shared_ptr<Font> font,
    const glm::vec3& position, 
    dae::MenuComponent* menu, 
    std::function<void()> action
)
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

std::unique_ptr<dae::GameObject> ObjectFactory::CreatePlayer(glm::vec3 startPos, glm::vec3 scale)
{
	std::string texture{"spritesheet.png"};
	int cols{ 15 };
	int rows{11};
	//glm::vec3 scale{1,1,1};

	auto player = std::make_unique<dae::GameObject>();
	player->SetTag(Tag::PLAYER);


	auto animationComp = player->AddComponent<dae::SpriteSheetComponent>(texture, rows, cols);
	animationComp->AddAnimation("Idle", { 1, 1, 6.f });
	animationComp->AddAnimation("Up", {6,3, 6.f});
	animationComp->AddAnimation("Down", {0,3,6.f});
	animationComp->AddAnimation("Left", {3, 3, 6.f});
	//animationComp->AddAnimation("Throw", {15, 1, 1.f});
	animationComp->AddAnimation("Throw", { 16, 1, 1.f });
	//animationComp->AddAnimation("Throw", { 17, 1, 1.f });
	animationComp->AddAnimation("Death", {18, 6, 6.f});


	animationComp->SetAnimation("Idle");

	auto pos = startPos;
	pos.y -= 3 * scale.y;

	player->GetTransform().SetScale(scale);
	player->GetTransform().SetPosition(pos);
	
	//MOVE TO PLAYER COMPONENT?
	auto livesComp = player->AddComponent<dae::LivesComponent>();
	auto pointsComp = player->AddComponent<dae::PointsComponent>();
	//set lives?

	//player texture size
	auto colliderSize = glm::vec3{ (animationComp->GetTextureSize().x - 4) * scale.x, ((animationComp->GetTextureSize().y/4 )* scale.y), 0 * scale.z };
	auto colliserOffest = glm::vec3{ 2 * scale.x, (colliderSize.y * 4 - colliderSize.y) ,0 };
	auto collider = player->AddComponent<ColliderComponent>(colliderSize, colliserOffest);



	auto playerComp = player->AddComponent<PlayerComponent>();
	//collider->AddObserver(playerComp);
	//livesComp->AddObserver(m_Galaga);

	return player;
}

std::unique_ptr<dae::GameObject> ObjectFactory::CreateShortPlatform(glm::vec3 position, glm::vec3 scale)
{
	auto platform = std::make_unique<dae::GameObject>();
	platform.get()->SetTag(Tag::PLATFORM);

	auto texture = platform->AddComponent<dae::TextureComponent>("ShortPlatform.png");

	platform->GetTransform().SetPosition(position);
	platform->GetTransform().SetScale(scale);


	////create custom collider (1 pixels from the bottom, 2 pixels thick, full width)
	//auto colliderSize = glm::vec3{texture->GetTextureSize().x * scale.x, 2 * scale.y, 0 * scale.z };
	//auto colliderOffset = glm::vec3(0, 1 * scale.y, 0);
	//auto collider = platform->AddComponent<ColliderComponent>(colliderSize, colliderOffset);


	return platform;
}

std::unique_ptr<dae::GameObject> ObjectFactory::CreateLongLeftPlatform(glm::vec3 position, glm::vec3 scale)
{
	auto platform = std::make_unique<dae::GameObject>();
	platform.get()->SetTag(Tag::PLATFORM);

	auto texture = platform->AddComponent<dae::TextureComponent>("LongPlatformL.png");

	platform->GetTransform().SetPosition(position);
	platform->GetTransform().SetScale(scale);


	////create custom collider (2 pixels from the bottom, 2 pixels thick, full width)
	auto colliderSize = glm::vec3{ texture->GetTextureSize().x * scale.x * 2, 2 * scale.y, 0 * scale.z };
	auto colliderOffset = glm::vec3(0, texture->GetTextureSize().y * scale.y - (2 * scale.y), 0);
	auto collider = platform->AddComponent<ColliderComponent>(colliderSize, colliderOffset);


	return platform;
}

std::unique_ptr<dae::GameObject> ObjectFactory::CreateLongRightPlatform(glm::vec3 position, glm::vec3 scale)
{
	auto platform = std::make_unique<dae::GameObject>();

	auto texture = platform->AddComponent<dae::TextureComponent>("LongPlatformR.png");

	auto pos = position;
	platform->GetTransform().SetPosition(position);
	platform->GetTransform().SetScale(scale);


	////create custom collider (2 pixels from the bottom, 2 pixels thick, full width)
	//auto colliderSize = glm::vec3{ texture->GetTextureSize().x * scale.x, 2 * scale.y, 0 * scale.z };
	//auto colliderOffset = glm::vec3(0, 2 * scale.y, 0);
	//auto collider = platform->AddComponent<ColliderComponent>(colliderSize, colliderOffset);


	return platform;
}

std::unique_ptr<dae::GameObject> ObjectFactory::CreateLadder(glm::vec3 position, glm::vec3 scale)
{
	auto platform = std::make_unique<dae::GameObject>();

	auto texture = platform->AddComponent<dae::TextureComponent>("Ladder.png");

	platform->GetTransform().SetPosition(position);
	platform->GetTransform().SetScale(scale);


	//REplace texture size with cell size?
	/*auto colliderSize = glm::vec3{ (texture->GetTextureSize().x - (3*2)) * scale.x,  texture->GetTextureSize().y * scale.y, 0 * scale.z};
	auto colliderOffset = glm::vec3(0, 2 * scale.y, 0);
	auto collider = platform->AddComponent<ColliderComponent>(colliderSize, colliderOffset);*/


	return platform;
}

std::unique_ptr<dae::GameObject> ObjectFactory::CreateTray(glm::vec3 position, glm::vec3 scale)
{
	auto tray = std::make_unique<dae::GameObject>();
	tray.get()->AddComponent<TrayComponent>(4);
	tray.get()->SetTag(Tag::TRAY);

	auto texture = tray->AddComponent<dae::TextureComponent>("tray.png");

	tray->GetTransform().SetPosition(position);
	tray->GetTransform().SetScale(scale);


	//create custom collider (8 pixels from the bottom (6 from the top), 2 pixels thick, 16px from each side, 32px wide)
	auto colliderSize = glm::vec3{ (texture->GetTextureSize().x/2 * scale.x), 1 * scale.y, 0 * scale.z };
	auto colliderOffset = glm::vec3(16*scale.x, 6 * scale.y, 0);
	auto collider = tray->AddComponent<ColliderComponent>(colliderSize, colliderOffset);


	return tray;
}

std::unique_ptr<dae::GameObject> ObjectFactory::CreateIngredient(glm::vec3 position, glm::vec3 scale, IngredientType type )
{
	auto ingredient = std::make_unique<dae::GameObject>();

	ingredient->AddComponent<IngredientComponent>(type);



	ingredient->GetTransform().SetPosition(position);
	ingredient->GetTransform().SetScale(scale);


	return ingredient;
}

std::shared_ptr<dae::GameObject> ObjectFactory::CreateIngredientPiece(IngredientType type, int index, IngredientComponent& parent)
{
	auto piece = std::make_shared<dae::GameObject>();
	piece.get()->SetTag(Tag::BURGER);
	auto pieceComp = piece->AddComponent<IngredientPieceComponent>(type, index, parent);


	//int pieceSize = cell/2 * scale;
	int pieceSize = 24;
	int scale = 3; // get from game settings

	glm::vec3 offset = glm::vec3{ index * pieceSize, 0 , 0 };
	piece->GetTransform().SetPosition(offset);
	piece->GetTransform().SetScale({ scale, scale, scale });

	//collision box
	auto colliderSize = glm::vec3{ pieceSize, pieceSize, 0};
	auto collider = piece->AddComponent<ColliderComponent>(colliderSize);
	
	collider->AddObserver(pieceComp);
	//add collider observer?

	return piece;
}

std::unique_ptr<dae::GameObject> ObjectFactory::CreatePointsDisplay(glm::vec3 position, glm::vec3 scale)
{
	auto pointsDisplayScore = std::make_unique<dae::GameObject>();

	pointsDisplayScore.get()->AddComponent<PointsDisplay>();


	pointsDisplayScore->GetTransform().SetPosition(position);

	pointsDisplayScore->GetTransform().SetScale(scale);


	return pointsDisplayScore;
}