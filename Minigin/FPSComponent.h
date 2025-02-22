#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextComponent;

	class FPSComponent final : public BaseComponent
	{
	public:
		void Update() override;
		void FixedUpdate([[maybe_unused]] const float fixedTimeStep) override {};

		FPSComponent(GameObject* pOwner);

		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;

		float GetFPS() const { return m_FPS; };
		void SetUpdateTime(float time) { m_updateTime = time; }

	private:
		float m_updateTime{ 1.0f };
		float m_accumulatedTime{ 0.0f };
		float m_FPS{ 0.0f };


		TextComponent* m_pText;

	};



};
