#pragma once
#include <Singleton.h>
#include <memory>
#include <GameObject.h>
#include <TextComponent.h>
#include <MenuComponent.h>
#include <Scene.h>
#include <ActionComponent.h>
#include <TextureComponent.h>
#include <functional>



namespace dae
{
    class GalagaGame;
    class ObjectFactory final : public Singleton<ObjectFactory>
    {
    private:
        friend class Singleton<ObjectFactory>;
        ObjectFactory();

    public:

        //TODO: switch to unique ptrs
        std::shared_ptr<dae::GameObject> CreateMainMenu();
        std::shared_ptr<dae::GameObject> CreateMenuItem(const std::string& text, std::shared_ptr<Font> font, const glm::vec3& position, dae::MenuComponent* menu, std::function<void()> action);
        std::shared_ptr<dae::GameObject> CreateMenuArrow(const std::string& textureName, dae::MenuComponent* menu, const float scale = 1.f);
        
        std::unique_ptr<dae::GameObject> CreatePlayer(std::string texture, glm::vec3 startPos = {0,0,0}, glm::vec3 scale = {1,1,1});
        std::unique_ptr<dae::GameObject> CreateLivesDisplay(std::string texture, GameObject* player, int lives = 3, glm::vec3 position = {0,0,0}, glm::vec3 scale = {1,1,1}, bool mirror = false);
        std::unique_ptr<dae::GameObject> CreatePointsDisplay(GameObject* player, glm::vec3 position);
        
        
        std::unique_ptr<dae::GameObject> CreateTexture(const std::string& fileName, const glm::vec3& position = glm::vec3(0, 0, 0), const float scale = 1.f);
        std::unique_ptr<dae::GameObject> CreateTextObject(std::shared_ptr<dae::Font> font, std::string text = {""}, glm::vec3 position = {0,0,0}, SDL_Color color = { 255,255,255,255 });
        std::unique_ptr<dae::GameObject> CreateBullet(glm::vec3 position = {0,0,0});
        std::unique_ptr<dae::GameObject> CreateEnemyBullet(glm::vec3 position = { 0,0,0 }, glm::vec3 targetPosition = {0,0,0});
        std::unique_ptr<dae::GameObject> CreateBee(glm::vec3 position, const std::vector<glm::vec2>& path);
        std::unique_ptr<dae::GameObject> CreateButterfly(glm::vec3 position, const std::vector<glm::vec2>& path);
        std::unique_ptr<dae::GameObject> CreateBossGalaga(glm::vec3 position, const std::vector<glm::vec2>& path);
        std::unique_ptr<dae::GameObject> CreatePlayerExplosion(glm::vec3 position = {0,0,0});

        GalagaGame* m_Galaga; 

    };
}
