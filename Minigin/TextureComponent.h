#pragma once
#include <string>
#include "ResourceManager.h"
#include "GameObject.h"
#include "GraphicsComponent.h"

namespace dae
{
	class TextureComponent final : public GraphicsComponent
	{
	public:
		virtual void Update() override {};

		TextureComponent(GameObject* pOwner, const std::string& fileName) : GraphicsComponent(pOwner) 
		{ m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName); };

		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;
	};
	
}

