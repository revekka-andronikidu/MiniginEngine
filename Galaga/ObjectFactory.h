#pragma once
#include <memory>
#include "GameObject.h"
#include "TextComponent.h"
#include "MenuComponent.h"
#include "Scene.h"
#include "ActionComponent.h"


namespace dae
{
    std::shared_ptr<dae::GameObject> MakeMenuItem(
        const std::string& text,
        std::shared_ptr<Font> font,
        const glm::vec3& position,
        dae::MenuComponent* menu,
        dae::Scene& scene,
        std::function<void()> action)
    {
        auto item = std::make_shared<dae::GameObject>();
        item->AddComponent<dae::TextComponent>(text, font);
        item->GetTransform().SetPosition(position);
        item->SetParent(menu->GetOwner(), false);
        item->AddComponent<ActionComponent>(action);
        menu->AddMenuItem(item);
        scene.Add(item);
        return item;
    };
}
