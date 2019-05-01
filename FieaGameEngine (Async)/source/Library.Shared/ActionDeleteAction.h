#pragma once

#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class ActionDeleteAction : public Action
	{
		RTTI_DECLARATIONS(ActionDeleteAction, Action)

	public:
		static const std::string ActionNameKey;
		static const std::size_t ActionNameIndex;
		static Vector<Signature> Signatures();

	public:
		explicit ActionDeleteAction(const std::string& action = std::string(), const std::string& name = std::string());
		virtual ~ActionDeleteAction() = default;

		virtual gsl::owner<ActionDeleteAction*> Clone() const override;
		virtual void Update(WorldState& worldState) override;

		std::string& ActionName();
		const std::string& ActionName() const;

	private:
		std::string mActionName;
	};

	CONCRETE_FACTORY(ActionDeleteAction, Scope)
}
