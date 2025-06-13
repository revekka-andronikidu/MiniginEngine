#pragma once
#include "Event.h"

namespace dae
{
	namespace GameEvents
	{
		struct ObjectDeath{};

		struct LivesUpdated{};

		struct PointsUpdated{};

		
		
	}

	inline const auto OBJECT_DEATH_EVENT = std::make_shared<TypedEvent<GameEvents::ObjectDeath>>(GameEvents::ObjectDeath{});
	inline const auto LIVES_UPDATED_EVENT = std::make_shared<TypedEvent<GameEvents::LivesUpdated>>(GameEvents::LivesUpdated{});
	inline const auto POINTS_UPDATED_EVENT = std::make_shared<TypedEvent<GameEvents::PointsUpdated>>(GameEvents::PointsUpdated{});

}