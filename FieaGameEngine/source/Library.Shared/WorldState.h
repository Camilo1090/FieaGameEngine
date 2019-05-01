#pragma once

namespace FieaGameEngine
{
	class GameTime;

	/// <summary>
	/// Holds the current state of the world, sector, entity and action being processed
	/// </summary>
	class WorldState final
	{
	public:
		/// <summary>
		/// default constructor
		/// </summary>
		/// <param name="gameTime">game time</param>
		explicit WorldState(const std::shared_ptr<GameTime>& gameTime);		
	
		WorldState(const WorldState&) = default;
		WorldState(WorldState&&) = default;		
		WorldState& operator=(const WorldState&) = default;
		WorldState& operator=(WorldState&&) = default;

		/// <summary>
		/// default destructor
		/// </summary>
		~WorldState() = default;	

		/// <summary>
		/// current world
		/// </summary>
		class World* World = nullptr;

		/// <summary>
		/// current sector
		/// </summary>
		class Sector* Sector = nullptr;

		/// <summary>
		/// current entity
		/// </summary>
		class Entity* Entity = nullptr;

		/// <summary>
		/// current action
		/// </summary>
		class Action* Action = nullptr;

		/// <summary>
		/// current game time
		/// </summary>
		std::shared_ptr<GameTime> GameTime = nullptr;
	};
}
