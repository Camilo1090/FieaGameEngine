#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "WorldState.h"
#include "GameTime.h"
#include "Action.h"

namespace FieaGameEngine
{
	WorldState::WorldState(const std::shared_ptr<FieaGameEngine::GameTime>& gameTime) : GameTime(gameTime)
	{
	}
}
