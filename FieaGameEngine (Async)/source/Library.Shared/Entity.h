#pragma once

#include "Attributed.h"
#include "Factory.h"
#include <string>

namespace FieaGameEngine
{
	class Sector;
	class WorldState;
	class Action;

	/// <summary>
	/// base class for updatable game objects in the engine
	/// </summary>
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:
		/// <summary>
		/// key name for actions prescribed attribute
		/// </summary>
		static const std::string ActionsKey;

		/// <summary>
		/// index of special actions key
		/// </summary>
		static const std::size_t ActionsIndex;

		/// <summary>
		/// prescribed attributes for this class
		/// </summary>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// default constructor, creates a nameless empty entity
		/// </summary>
		Entity();

		/// <summary>
		/// constructor with name
		/// </summary>
		/// <param name="name">name</param>
		explicit Entity(const std::string& name);

		/// <summary>
		/// copy contructor
		/// </summary>
		/// <param name="">entity</param>
		Entity(const Entity&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">entity</param>
		Entity(Entity&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">entity</param>
		/// <returns>entity</returns>
		Entity& operator=(const Entity&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">entity</param>
		/// <returns>entity</returns>
		Entity& operator=(Entity&&) = default;

		/// <summary>
		/// default destructor
		/// </summary>
		virtual ~Entity() = default;

		/// <summary>
		/// gets a copy of this entity
		/// </summary>
		/// <returns>entity pointer</returns>
		virtual gsl::owner<Entity*> Clone() const override;

		/// <summary>
		/// updates this entity
		/// </summary>
		/// <param name="worldState">world state</param>
		virtual void Update(WorldState& worldState);

		/// <summary>
		/// sets the sector of this entity
		/// </summary>
		/// <param name="sector">sector</param>
		void SetSector(Sector& sector);

		/// <summary>
		/// creates a new action and adopts it
		/// </summary>
		/// <param name="className">class name</param>
		/// <param name="instanceName">instance name</param>
		/// <returns>action</returns>
		gsl::owner<Action*> CreateAction(const std::string& className, const std::string& instanceName);

		Action* FindAction(const std::string& actionName);

		const Action* FindAction(const std::string& actionName) const;

		/// <summary>
		/// gets the actions of this entity
		/// </summary>
		/// <returns>actions</returns>
		Datum& Actions();

		/// <summary>
		/// gets the actions of this entity
		/// </summary>
		/// <returns>actions</returns>
		const Datum& Actions() const;

		/// <summary>
		/// gets the instance name of this entity
		/// </summary>
		/// <returns>name</returns>
		std::string& Name();

		/// <summary>
		/// gets the instance name of this entity
		/// </summary>
		/// <returns>name</returns>
		const std::string& Name() const;

		/// <summary>
		/// sets the name of this entity
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

	protected:
		/// <summary>
		/// contructor for derived types to correctly use the rtti system
		/// </summary>
		/// <param name="type">type</param>
		explicit Entity(RTTI::IdType type);

		/// <summary>
		/// constructor with rtti type and name
		/// </summary>
		/// <param name="type">type</param>
		/// <param name="name">name</param>
		Entity(RTTI::IdType type, const std::string& name);

	private:
		/// <summary>
		/// name of this entity
		/// </summary>
		std::string mName;
	};

	/// <summary>
	/// entity factory
	/// </summary>
	CONCRETE_FACTORY(Entity, FieaGameEngine::Scope)
}
