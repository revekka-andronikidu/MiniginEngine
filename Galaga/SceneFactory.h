#pragma once
#include "Singleton.h"

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
		void CreateLevel();

	};

}
