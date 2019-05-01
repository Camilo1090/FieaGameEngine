#pragma once

#include "Attributed.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class Entity;
	class World;
	class WorldState;

	/// <summary>
	/// container of entities
	/// </summary>
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)

	public:
		/// <summary>
		/// special key for entities datum
		/// </summary>
		static const std::string EntitiesKey;

		/// <summary>
		/// index of sectors key
		/// </summary>
		static const std::size_t EntitiesIndex;

		/// <summary>
		/// attributes of this sector
		/// </summary>
		/// <returns>attributes of this sector</returns>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// default constructor
		/// </summary>
		Sector();

		/// <summary>
		/// constructor with name
		/// </summary>
		/// <param name="name">name</param>
		explicit Sector(const std::string& name);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">sector</param>
		Sector(const Sector&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">sector</param>
		Sector(Sector&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">sector</param>
		/// <returns>sector</returns>
		Sector& operator=(const Sector&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">sector</param>
		/// <returns>sector</returns>
		Sector& operator=(Sector&&) = default;

		/// <summary>
		/// default destructor
		/// </summary>
		virtual ~Sector() = default;

		/// <summary>
		/// gets a copy of this sector
		/// </summary>
		/// <returns>sector pointer</returns>
		virtual gsl::owner<Sector*> Clone() const override;

		/// <summary>
		/// updates all child entities
		/// </summary>
		/// <param name="worldState">world state</param>
		void Update(WorldState& worldState);

		/// <summary>
		/// creates and adopts a new entity of the given type. REQUIRES AN EXISTING ENTITY FACTORY
		/// </summary>
		/// <param name="className">entity type</param>
		/// <param name="instanceName">entity name</param>
		/// <returns>entity</returns>
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// gets the entities inside this sector
		/// </summary>
		/// <returns>datum</returns>
		Datum& Entities();

		/// <summary>
		/// gets the entities inside this sector
		/// </summary>
		/// <returns>datum</returns>
		const Datum& Entities() const;

		/// <summary>
		/// sets the world containing this sector
		/// </summary>
		/// <param name="world">world</param>
		void SetWorld(World& world);

		/// <summary>
		/// gets the name of this sector
		/// </summary>
		/// <returns>name</returns>
		std::string& Name();

		/// <summary>
		/// gets the name of this sector
		/// </summary>
		/// <returns>name</returns>
		const std::string& Name() const;

		/// <summary>
		/// sets the name of this sector
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

	private:
		/// <summary>
		/// name of this sector
		/// </summary>
		std::string mName;
	};

	CONCRETE_FACTORY(Sector, FieaGameEngine::Scope)
}
