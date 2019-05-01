#pragma once

#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine
{
	/// <summary>
	/// increases target by 1
	/// </summary>
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:
		/// <summary>
		/// target key
		/// </summary>
		static const std::string TargetKey;

		/// <summary>
		/// step key
		/// </summary>
		static const std::string StepKey;

		/// <summary>
		/// signatures
		/// </summary>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// constructor
		/// </summary>
		ActionIncrement();

		/// <summary>
		/// constructor with name
		/// </summary>
		/// <param name="name">name</param>
		explicit ActionIncrement(const std::string& name);

		/// <summary>
		/// destructor
		/// </summary>
		virtual ~ActionIncrement() = default;

		/// <summary>
		/// clones this action
		/// </summary>
		/// <returns>sction</returns>
		virtual gsl::owner<ActionIncrement*> Clone() const override;

		/// <summary>
		/// updates this action
		/// </summary>
		/// <param name="worldState">world state</param>
		virtual void Update(WorldState& worldState) override;

	private:
		/// <summary>
		/// target for increment
		/// </summary>
		std::string mTarget;

		/// <summary>
		/// step increment
		/// </summary>
		float mStep = 1.0f;

		/// <summary>
		/// cached reference to target
		/// </summary>
		Datum* mReference = nullptr;
	};

	CONCRETE_FACTORY(ActionIncrement, FieaGameEngine::Scope)
}
