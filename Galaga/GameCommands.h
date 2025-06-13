#pragma once
#include "Command.h"
#include "GameObject.h"
#include "TimeManager.h"
#include "MenuComponent.h"

using namespace dae;

class MoveCommand : public GameObjectCommand
{
public:
	MoveCommand(GameObject* gameObject, glm::vec3 direction, int leftBorder = 0, int rifgtBorder = 500, float speed = 10.f) : GameObjectCommand(gameObject), m_Direction{ direction }, m_MoveSpeed{ speed }, m_LeftBorder{ leftBorder }, m_RightBorder{ rifgtBorder } {};

	void Execute() override;

	void SetDirection(glm::vec3 direction) { m_Direction = direction; };
	void SetSpeed(float speed) { m_MoveSpeed = speed; };

private:
	int m_LeftBorder;
	int m_RightBorder;
	float m_MoveSpeed{};
	glm::vec3 m_Direction{};
};

class MenuMoveCommand : public GameObjectCommand
{
public:
	MenuMoveCommand(GameObject* gameObject, MenuComponent::Direction direction) : GameObjectCommand(gameObject), m_direction{ direction } {};

	void Execute() override;

	

private:
	MenuComponent::Direction m_direction{ MenuComponent::Direction::Up};
	
};

class MenuEnterCommand : public GameObjectCommand
{
public:
	MenuEnterCommand(GameObject* gameObject) : GameObjectCommand(gameObject) {};

	void Execute() override;



private:
	

};


class LooseLiveCommand : public GameObjectCommand
{
public:

	LooseLiveCommand(GameObject* gameObject) : GameObjectCommand(gameObject) {};

	void Execute() override;

};

class AddScoreCommand : public GameObjectCommand
{
public:

	AddScoreCommand(GameObject* gameObject, int points) : GameObjectCommand(gameObject), m_Points{ points } {};

	void Execute() override;

private:
	int m_Points;
};

class ShootCommand : public GameObjectCommand
{
public:

	ShootCommand(GameObject* gameObject) : GameObjectCommand(gameObject) {};

	void Execute() override;

};

class SkipLevelCommand : public Command
{
public:
	SkipLevelCommand() = default;
	void Execute() override;
};

class MuteGameCommand final : public Command
{
public:
	MuteGameCommand() = default;
	void Execute() override;
};