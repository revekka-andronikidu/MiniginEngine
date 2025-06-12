#pragma once
#include "Singleton.h"
#include <string>
#include <vector>
#include <memory>

namespace dae
{
	class Game;
	class GameManager final : public Singleton<GameManager>
	{
	public:
		template <typename T, typename std::enable_if<std::is_base_of<Game, T>::value>::type* = nullptr>
		Game& CreateGame();

		void SetActiveGame(Game* game);
		Game* GetActiveGame() const { return m_ActiveGame; }

		//void Update(float elapsed);
		//bool IsGameRunning();



	private:
		friend class Singleton<GameManager>;
		GameManager() = default;

		std::vector<std::shared_ptr<Game>> m_Games;
		Game* m_ActiveGame{nullptr};
	};


	template <typename T, typename std::enable_if<std::is_base_of<Game, T>::value>::type*>
	Game& GameManager::CreateGame()
	{
		auto game = std::make_shared<T>();
		//m_ActiveGame = game.get();
		//auto tempCopy = game;
		m_Games.push_back(game);
		return *game;
	}

};

