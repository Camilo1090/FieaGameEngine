#pragma once

#include "Attributed.h"
#include "Factory.h"
#include "WorldState.h"
#include "GameClock.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
	class Sector;
	class GameTime;

	/// <summary>
	/// container of sectors
	/// </summary>
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)

	public:
		/// <summary>
		/// special key for sectors datum
		/// </summary>
		static const std::string SectorsKey;

		/// <summary>
		/// sectors key index
		/// </summary>
		static const std::size_t SectorsIndex;

		/// <summary>
		/// attributes of this world
		/// </summary>
		/// <returns>attributes</returns>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// constructor with time
		/// </summary>
		/// <param name="gameTime">game time</param>
		explicit World(const std::shared_ptr<GameTime>& gameTime);

		/// <summary>
		/// constructor with time and name
		/// </summary>
		/// <param name="gameTime">game time</param>
		/// <param name="name">name</param>
		World(const std::shared_ptr<GameTime>& gameTime, const std::string& name);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">world</param>
		World(const World&) = default;

		/// <summary>
		/// move contructor
		/// </summary>
		/// <param name="">world</param>
		World(World&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">world</param>
		/// <returns>world</returns>
		World& operator=(const World&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">world</param>
		/// <returns>world</returns>
		World& operator=(World&&) = default;

		/// <summary>
		/// default destructor
		/// </summary>
		virtual ~World() = default;

		/// <summary>
		/// creates a copy of this world
		/// </summary>
		/// <returns>world</returns>
		gsl::owner<World*> Clone() const override;

		/// <summary>
		/// updates all sectors inside this world
		/// </summary>
		/// <param name="worldState">world state</param>
		void Update(WorldState& worldState);

		/// <summary>
		/// creates and adopts a new sector. REQUIRES AN EXISTING SECTOR FACTORY
		/// </summary>
		/// <param name="instanceName">sector name</param>
		/// <returns>sector</returns>
		Sector* CreateSector(const std::string& instanceName);

		/// <summary>
		/// sectors of this world
		/// </summary>
		/// <returns>datum</returns>
		Datum& Sectors();

		/// <summary>
		/// sectors of this world
		/// </summary>
		/// <returns>datum</returns>
		const Datum& Sectors() const;

		/// <summary>
		/// add a scope to the deletion queue
		/// </summary>
		/// <param name="scope">scope</param>
		void MarkForDeletion(Scope& scope);

		/// <summary>
		/// gets the name of this world
		/// </summary>
		/// <returns>name</returns>
		std::string& Name();

		/// <summary>
		/// gets the name of this world
		/// </summary>
		/// <returns>name</returns>
		const std::string& Name() const;

		/// <summary>
		/// sets the name of this sector
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

		/// <summary>
		/// gets the world state
		/// </summary>
		/// <returns>world state</returns>
		WorldState& GetWorldState();

		/// <summary>
		/// gets the world state
		/// </summary>
		/// <returns>world state</returns>
		const WorldState& GetWorldState() const;

		EventQueue* GetEventQueue();
		void SetEventQueue(EventQueue& queue);

	private:
		/// <summary>
		/// name of this sector
		/// </summary>
		std::string mName;

		/// <summary>
		/// world state
		/// </summary>
		WorldState mWorldState;

		/// <summary>
		/// 
		/// </summary>
		EventQueue* mEventQueue;

		Vector<Scope*> mDeleteList{ 8 };

		void DeleteMarkedScopes();
	};
}
