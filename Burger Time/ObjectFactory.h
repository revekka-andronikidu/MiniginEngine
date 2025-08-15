#pragma once
#include <Singleton.h>
#include <memory>
#include <GameObject.h>
#include <functional>
#include <MenuComponent.h>
#include "Helpers.h"
#include <SDL_ttf.h>
#include <TextComponent.h>

namespace dae
{
    class Font;
    class IngredientComponent;


    class ObjectFactory final : public Singleton<ObjectFactory>
    {
    private:
        friend class Singleton<ObjectFactory>;
        ObjectFactory()= default;

    public:
        std::unique_ptr<dae::GameObject> CreateTexture(const std::string& fileName, const glm::vec3& position = glm::vec3(0, 0, 0), const float scale = 1.f);
        std::unique_ptr<dae::GameObject> CreateText(const std::string& text, std::shared_ptr<Font> font, SDL_Color color, TextComponent::TextAlign aligment = TextComponent::TextAlign::Left, const glm::vec3& position = glm::vec3(0, 0, 0), glm::vec3 scale = { 1.f, 1.f, 1.f });
        std::unique_ptr<dae::GameObject> CreateMainMenu();
        std::shared_ptr<dae::GameObject> CreateMenuItem(const std::string& text, std::shared_ptr<Font> font, const glm::vec3& position, dae::MenuComponent* menu, std::function<void()> action);
        std::shared_ptr<dae::GameObject> CreateMenuArrow(const std::string& textureName, dae::MenuComponent* menu, const float scale = 1.f);

        std::unique_ptr<dae::GameObject> CreatePlayer(glm::vec3 startPos, glm::vec3 scale, int lives);
        std::unique_ptr<dae::GameObject> CreatePepper(Direction direction, glm::vec3 startPos, glm::vec3 scale = GameSettings::scale);
        std::unique_ptr<dae::GameObject> CreateMrHotDog(glm::vec3 startPos, glm::vec3 scale = GameSettings::scale);

        std::unique_ptr<dae::GameObject> CreateShortPlatform(glm::vec3 position, glm::vec3 scale);
        std::unique_ptr<dae::GameObject> CreateLongLeftPlatform(glm::vec3 position, glm::vec3 scale);
        std::unique_ptr<dae::GameObject> CreateLongRightPlatform(glm::vec3 position, glm::vec3 scale);
        std::unique_ptr<dae::GameObject> CreateLadder(glm::vec3 position, glm::vec3 scale);

        std::unique_ptr<dae::GameObject> CreateTray(glm::vec3 position, glm::vec3 scale, int burgerIngredients = 4);
        std::unique_ptr<dae::GameObject> CreateIngredient(glm::vec3 position, glm::vec3 scale, IngredientType type);
        std::shared_ptr<dae::GameObject> CreateIngredientPiece(IngredientType type, int index, IngredientComponent& parent);

        std::unique_ptr<dae::GameObject> CreatePointsDisplay(glm::vec3 position, glm::vec3 scale);

        std::unique_ptr<dae::GameObject> CreateHighScoreEntryKeypad(glm::vec3 position = {0,0,0}, glm::vec3 scale = GameSettings::scale);

        std::unique_ptr<dae::GameObject> CreateAnimatedText(const std::string finalText, float intreval, std::shared_ptr<Font> font, glm::vec3 position = { 0,0,0 }, glm::vec3 scale = GameSettings::scale, SDL_Color color = SDL_Color{ 255, 255, 255, 255 }, TextComponent::TextAlign aligment = TextComponent::TextAlign::Left);

    };

}
