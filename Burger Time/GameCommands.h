#pragma once
#include <Command.h>
#include "Helpers.h"
#include <MenuComponent.h>

namespace dae
{
	class MuteGameCommand final : public Command
	{
	public:
		MuteGameCommand() = default;
		void Execute() override;
	};

	class SkipLevelCommand final : public Command
	{
	public:
		SkipLevelCommand() = default;
		void Execute() override;
	};

	class MenuMoveCommand : public GameObjectCommand
	{
	public:
		MenuMoveCommand(GameObject * gameObject, MenuComponent::Direction direction) : GameObjectCommand(gameObject), m_direction{ direction } {};
		void Execute() override;

	private:
		MenuComponent::Direction m_direction{ MenuComponent::Direction::Up};
	};

	class PepperCommand : public GameObjectCommand
	{
	public:
		PepperCommand(GameObject* gameObject) : GameObjectCommand(gameObject) {};

		void Execute() override;
	};

	class MenuEnterCommand : public GameObjectCommand
	{
	public:
		MenuEnterCommand(GameObject* gameObject) : GameObjectCommand(gameObject) {};
		void Execute() override;
	};

	class KeypadMoveCommand : public GameObjectCommand
	{
	public:
		KeypadMoveCommand(GameObject* gameObject, Direction direction) : GameObjectCommand(gameObject), m_direction{ direction } {};
		void Execute() override;
	private:
		Direction m_direction{};
	};

	class KeypadSelectCommand : public GameObjectCommand
	{
	public:
		KeypadSelectCommand(GameObject* gameObject) : GameObjectCommand(gameObject) {};
		void Execute() override;
	};

	class BackToMenuCommand : public Command
	{
	public:
		BackToMenuCommand() = default;
		void Execute() override;
	};


	class MoveCommand : public GameObjectCommand
	{
	public:
		MoveCommand(GameObject* gameObject, Direction direction) : GameObjectCommand(gameObject), m_Direction{ direction } {};

		void Execute() override;
		void SetDirection(Direction direction) { m_Direction = direction; };

	private:
		Direction m_Direction{};
	};

	class EnemyMoveCommand : public GameObjectCommand
	{
	public:
		EnemyMoveCommand(GameObject* gameObject, Direction direction) : GameObjectCommand(gameObject), m_Direction{ direction } {};

		void Execute() override;
		void SetDirection(Direction direction) { m_Direction = direction; };

	private:
		Direction m_Direction{};
	};
}


