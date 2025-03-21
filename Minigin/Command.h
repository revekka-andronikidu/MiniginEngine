#pragma once
#include "GameObject.h"
namespace dae
{

	class Command
	{
	public:
		virtual ~Command() {}
		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute() = 0;

	protected:
		Command() = default;
	};

	class GameObjectCommand : public Command
	{
	public:
		GameObjectCommand(GameObject* gameObject)
			: m_pGameObject{ gameObject } 
		{
		};

	protected:
		GameObject* GetGameObject() const { return m_pGameObject; }

	private:
		GameObject* m_pGameObject;

	};


};

