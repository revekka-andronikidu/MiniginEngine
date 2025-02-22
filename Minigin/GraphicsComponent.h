#pragma once
#pragma once
#include <memory>
#include "BaseComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"


namespace dae
{

	class GraphicsComponent : public BaseComponent
	{

	public:
		virtual void Render() const
		{
			if (m_pTexture != nullptr)
			{
				const glm::vec3& pos = GetOwner()->GetTransform().GetPosition();

				Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);


				//::cout << " rendered" << std::endl;

			}
			else
			{
				
				//std::cout << " No texture found" << std::endl;
				return;
				
			}
		};

		virtual void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		void SetTexture(const std::string& fileName) { m_pTexture = ResourceManager::GetInstance().LoadTexture(fileName); };

		virtual ~GraphicsComponent() = default;
		GraphicsComponent(const GraphicsComponent&) = delete;
		GraphicsComponent(GraphicsComponent&&) noexcept = delete;
		GraphicsComponent& operator=(const GraphicsComponent&) = delete;
		GraphicsComponent& operator=(GraphicsComponent&&) noexcept = delete;

	protected:
		explicit GraphicsComponent(GameObject* pOwner) : BaseComponent(pOwner)
		{

		}

		std::shared_ptr<Texture2D> m_pTexture{};
	};


}

