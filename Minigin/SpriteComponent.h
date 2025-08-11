#pragma once
#include "GraphicsComponent.h"

namespace dae
{
	class SpriteComponent : public GraphicsComponent
	{
	public:
		virtual void Update() override;
		void Render() const override;

		SpriteComponent(GameObject* pOwner, const std::string& fileName, int rows = 1, int columns = 1, float framesPerSec = 10.f);

		glm::ivec2 GetTextureSize() const override;

		void SetAnimate(bool animate) { m_Animate = animate; };
		

		virtual ~SpriteComponent() = default;
		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) = delete;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) = delete;


	protected:
		int m_Rows{};
		int m_Columns{};
		int m_TotalFrames{0};
		int m_CurrentFrame{0};
		float m_FramesPerSecond{};
		//float m_SecondsPerFrame{};
		float m_ElapsedTime{};
		bool m_Animate;

		virtual void Animate();
		virtual SDL_Rect GetCurrentSrcRect() const;
	};

}

