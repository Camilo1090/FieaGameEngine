#pragma once

#include "ActionList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// updates actions based on a condition
	/// </summary>
	class ActionListIf final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)

	public:
		/// <summary>
		/// condition key name
		/// </summary>
		static const std::string ConditionKey;

		/// <summary>
		/// true actions key name
		/// </summary>
		static const std::string TrueActionsKey;

		/// <summary>
		/// false actions key name
		/// </summary>
		static const std::string FalseActionsKey;

		/// <summary>
		/// signatures
		/// </summary>
		/// <returns></returns>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// constructor
		/// </summary>
		ActionListIf();

		/// <summary>
		/// constructor with name
		/// </summary>
		/// <param name="name">name</param>
		explicit ActionListIf(const std::string& name);

		/// <summary>
		/// destructor
		/// </summary>
		virtual ~ActionListIf() = default;

		/// <summary>
		/// clones this action
		/// </summary>
		/// <returns>action</returns>
		virtual gsl::owner<ActionListIf*> Clone() const override;

		/// <summary>
		/// updates this action
		/// </summary>
		/// <param name="worldState">world state</param>
		virtual void Update(WorldState& worldState) override;

	private:
		/// <summary>
		/// condition to evaluate
		/// </summary>
		std::int32_t mCondition = 0;
	};

	CONCRETE_FACTORY(ActionListIf, FieaGameEngine::Scope)
}
