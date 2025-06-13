#pragma once
#include "Singleton.h"
#include <string>

namespace dae
{
	class GalagaGame;
	class SceneFactory final : public Singleton<SceneFactory>
	{
	private:
		friend class Singleton<SceneFactory>;
		SceneFactory();

		GalagaGame* m_Galaga;
		
		

	public:
		void CreateMainMenu();
		void CreateStageScene(std::string sceneName);
		void CreatePlayer(std::string sceneName, int playerNumber);
	};

}
