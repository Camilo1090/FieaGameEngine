#pragma once

#include "Factory.h"
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// list of actions
	/// </summary>
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		static const std::string ActionsKey;
		static const std::size_t ActionsIndex;
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// constructor
		/// </summary>
		ActionList();

		/// <summary>
		/// constructor with name
		/// </summary>
		/// <param name="name">name</param>
		explicit ActionList(const std::string& name);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">action</param>
		ActionList(const ActionList&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">action</param>
		ActionList(ActionList&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">action</param>
		/// <returns>action</returns>
		ActionList& operator=(const ActionList&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">action</param>
		/// <returns>action</returns>
		ActionList& operator=(ActionList&&) = default;

		/// <summary>
		/// destructor
		/// </summary>
		virtual ~ActionList() = default;

		/// <summary>
		/// clones this action
		/// </summary>
		/// <returns>action</returns>
		virtual gsl::owner<ActionList*> Clone() const override;

		/// <summary>
		/// updates all contained actions
		/// </summary>
		/// <param name="worldState">world state</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// gets contained actions
		/// </summary>
		/// <returns>actions</returns>
		Datum& Actions();

		/// <summary>
		/// gets contained actions
		/// </summary>
		/// <returns>actions</returns>
		const Datum& Actions() const;

	protected:
		/// <summary>
		/// rtti constructor
		/// </summary>
		/// <param name="type">actual type</param>
		explicit ActionList(RTTI::IdType type);

		/// <summary>
		/// rtti constructor with name
		/// </summary>
		/// <param name="type">actual type</param>
		/// <param name="name">name</param>
		ActionList(RTTI::IdType type, const std::string& name);
	};

	CONCRETE_FACTORY(ActionList, FieaGameEngine::Scope)
}
