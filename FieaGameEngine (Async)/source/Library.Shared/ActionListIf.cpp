#include "pch.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf)

	const std::string ActionListIf::ConditionKey = "Condition";
	const std::string ActionListIf::TrueActionsKey = "True";
	const std::string ActionListIf::FalseActionsKey = "False";

	Vector<Signature> ActionListIf::Signatures()
	{
		return {
			{ ConditionKey, Datum::DatumType::INTEGER, 1, offsetof(ActionListIf, mCondition) }
		};
	}

	ActionListIf::ActionListIf() : ActionList(TypeIdClass())
	{
	}

	ActionListIf::ActionListIf(const std::string& name) : ActionList(TypeIdClass(), name)
	{
	}

	gsl::owner<ActionListIf*> ActionListIf::Clone() const
	{
		return new ActionListIf(*this);
	}

	void ActionListIf::Update(WorldState& worldState)
	{
		const Datum* datum = &At(TrueActionsKey);
		
		if (!mCondition)
		{
			datum = &At(FalseActionsKey);
		}

		for (size_t i = 0; i < datum->Size(); ++i)
		{
			auto& actions = At(ActionList::ActionsKey);
			auto actionIndex = static_cast<size_t>(datum->Get<std::int32_t>(i));
			assert(actionIndex >= 0 && actionIndex < actions.Size());
			Scope& scope = actions[actionIndex];
			assert(scope.Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(&scope);
			worldState.Action = action;
			action->Update(worldState);
			worldState.Action = nullptr;
		}
	}
}
