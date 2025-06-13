#include "BeeComponent.h"
#include "GameCommands.h"
#include "SceneManager.h"
#include "Scene.h"

using namespace dae;

void BeeComponent::AddPoints()
{

	auto players = SceneManager::GetInstance().GetActiveScene().GetObjectsWithTag("player");
	//TODO: register with an even which player made the shot for multiplayera
	//if(players)

	if (m_IsAttacking)
	{
		auto addScore = std::make_unique<AddScoreCommand>(players[0], 100); // or based on enemy type
		addScore->Execute(); // execute immediately

	}
	else
	{
		auto addScore = std::make_unique<AddScoreCommand>(players[0], 50); // or based on enemy type
		addScore->Execute();
	}
}
