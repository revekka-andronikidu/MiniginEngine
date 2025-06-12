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


        std::shared_ptr<dae::GameObject> CreateMainMenu();
        std::shared_ptr<dae::GameObject> CreateMenuItem(const std::string& text, std::shared_ptr<Font> font, const glm::vec3& position, dae::MenuComponent* menu, std::function<void()> action);
        std::shared_ptr<dae::GameObject> CreateMenuArrow(const std::string& textureName, dae::MenuComponent* menu, const float scale = 1.f);
        std::shared_ptr<dae::GameObject> CreatePlayer();
        std::shared_ptr<dae::GameObject> CreateLivesDisplay();
        std::shared_ptr<dae::GameObject> CreatePointsDisplay();
        std::shared_ptr<dae::GameObject> CreateTexture(const std::string& fileName, const glm::vec3& position = glm::vec3(0, 0, 0), const float scale = 1.f);
        
        GalagaGame* m_Galaga; 

    };
}
