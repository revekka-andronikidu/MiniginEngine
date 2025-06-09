#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	if(m_pActiveScene)
		m_pActiveScene->Update();
}

void dae::SceneManager::FixedUpdate(const float fixed_time_step)
{
	if (m_pActiveScene)
		m_pActiveScene->FixedUpdate(fixed_time_step);

}

void dae::SceneManager::Render()
{
	if (m_pActiveScene)
		m_pActiveScene->Render();
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetSceneName() == name)
		{
			m_pActiveScene = scene.get();
		}
	}
}

void dae::SceneManager::SetActiveScene(const dae::Scene& scene)
{
	m_pActiveScene = const_cast<Scene*>(&scene);
}

dae::Scene& dae::SceneManager::GetScene(const std::string& name) const
{
	for (const auto& scene : m_scenes)
	{
		if (scene->GetSceneName() == name)
		{
			return *scene;
		}
	}
	throw std::exception("Scene not found");
}

