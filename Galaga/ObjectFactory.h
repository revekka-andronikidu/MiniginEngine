#pragma once
#include <memory>
#include <GameObject.h>
#include <TextComponent.h>
#include <MenuComponent.h>
#include <Scene.h>
#include <ActionComponent.h>
#include <TextureComponent.h>


namespace dae
{
    std::shared_ptr<dae::GameObject> MakeMenuItem(
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

    std::shared_ptr<dae::GameObject> MakeMenuArrow(
        const std::string& textureName,
        dae::MenuComponent* menu,
        const float scale = 1.f
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


    std::shared_ptr<dae::GameObject> MakeTexture(
        const std::string& fileName,
        const glm::vec3& position = glm::vec3(0,0,0),
        const float scale = 1.f
       // dae::Scene& scene
       )
    {
        auto item = std::make_shared<dae::GameObject>();
        auto texture = item->AddComponent<dae::TextureComponent>(fileName);
        auto logoSize = texture->GetTextureSize();
        item->GetTransform().SetPosition(position);
        item->GetTransform().SetScale(glm::vec3(scale, scale, scale));
        //scene.Add(item);
        return item;
    };
}
