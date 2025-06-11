#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void FixedUpdate(const float fixed_time_step);
		void Render();

		void SetActiveScene(const std::string& name);
		void SetActiveScene(const dae::Scene& scene);

		//void SwitchScene();
		Scene& GetScene(const std::string& name) const;
		Scene& GetActiveScene() const { return *m_pActiveScene; };

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		Scene* m_pActiveScene{nullptr};
	};
}
