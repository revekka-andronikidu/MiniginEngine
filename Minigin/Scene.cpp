#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

std::vector<GameObject*> Scene::GetObjectsWithTag(std::string tag) const
{
	std::vector<GameObject*> objects{};
	objects.reserve(m_objects.size());
	for (const auto& object : m_objects)
	{
		if (object.get()->HasTag(tag))
			objects.emplace_back(object.get());
	}
	return objects;
};
void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::LateUpdate()
{
	DestroyObjects();
}

void Scene::DestroyObjects()
{
	for (const auto& object : m_objects)
	{
		object->RemoveDeadComponents();
	}

	std::erase_if(m_objects,
		[&](const std::shared_ptr<GameObject>& object)
		{
			return object->IsMarkedForDestroy();
		}
	);
}

void Scene::FixedUpdate(const float fixed_time_step)
{
	for (auto& object : m_objects)
	{
		object->FixedUpdate(fixed_time_step);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

