#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <optional>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update();
		void FixedUpdate(const float fixed_time_step);
		void LateUpdate();
		void Render();

		void SetActiveScene(const std::string& name);
		void SetActiveScene(const dae::Scene& scene);

		//void SwitchScene();
		Scene& GetScene(const std::string& name) const;
		bool HasScene(const std::string& name) const;
		
		Scene& GetActiveScene() const { return *m_pActiveScene; };
		void DestroyScene(const std::string& name);

		void QueueSceneChange(const std::string& newScene, const std::string& destroyScene);
		

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		struct PendingSceneChange 
		{
			std::string newScene;
			std::string destroyScene;
		};

		std::vector<std::shared_ptr<Scene>> m_scenes;
		Scene* m_pActiveScene{nullptr};
		std::optional<PendingSceneChange> m_pendingSceneChange;
	};
}
