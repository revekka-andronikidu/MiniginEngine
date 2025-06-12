#pragma once
#include "GameManager.h"

namespace dae
{
	class Game 
	{
		template <typename T, typename std::enable_if<std::is_base_of<Game, T>::value>::type*>
		friend Game& GameManager::CreateGame();
		
	public:
		//virtual void Update(float elapsed) = 0;

		Game() = default;
		virtual ~Game() = default;

		virtual void Initialize() = 0;

		Game(const Game&) = delete;
		Game(Game&&) = delete;
		Game& operator= (const Game&) = delete;
		Game& operator= (const Game&&) = delete;

		//virtual bool IsGameRunning() = 0;
	};

}



