#pragma once
#include "Event.h"

namespace dae
{
	namespace GameEvents
	{
		struct ObjectDeath{};

		struct LivesUpdated{};

		struct PointsUpdated{};

		struct Collision 
		{
			int width;
			int height;
			const GameObject& entity;
		};
	}

	inline const auto OBJECT_DEATH_EVENT = std::make_shared<TypedEvent<GameEvents::ObjectDeath>>(GameEvents::ObjectDeath{});
	inline const auto LIVES_UPDATED_EVENT = std::make_shared<TypedEvent<GameEvents::LivesUpdated>>(GameEvents::LivesUpdated{});
	inline const auto POINTS_UPDATED_EVENT = std::make_shared<TypedEvent<GameEvents::PointsUpdated>>(GameEvents::PointsUpdated{});
	//inline const auto COLLISION_EVENT = std::make_shared<TypedEvent<GameEvents::Collision>>(GameEvents::Collision{ 0,0, GameObject{} });

	inline Event CreateCollisionEvent(int width, int height, const GameObject& entity) 
	{
		return std::make_shared<TypedEvent<GameEvents::Collision>>(GameEvents::Collision{ width, height, entity });
	}

}