#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	const std::string ActionList::ActionsKey = "Actions";

	const std::size_t ActionList::ActionsIndex = 2;

	Vector<Signature> ActionList::Signatures()
	{
		return {
			{ ActionsKey, Datum::DatumType::TABLE, 0, 0 }
		};
	}

	ActionList::ActionList() : ActionList(TypeIdClass())
	{
	}

	ActionList::ActionList(const std::string& name) : ActionList(TypeIdClass(), name)
	{
	}

	gsl::owner<ActionList*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	void ActionList::Update(WorldState& worldState)
	{
		Datum& actions = Actions();
		for (size_t i = 0; i < actions.Size(); ++i)
		{
			Scope& scope = actions[i];
			assert(scope.Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(&scope);
			worldState.Action = action;
			action->Update(worldState);
			worldState.Action = nullptr;
		}
	}

	Datum& ActionList::Actions()
	{
		return operator[](ActionsIndex);
	}

	const Datum& ActionList::Actions() const
	{
		return operator[](ActionsIndex);
	}

	ActionList::ActionList(RTTI::IdType type) : ActionList(type, std::string())
	{
	}

	ActionList::ActionList(RTTI::IdType type, const std::string& name) : Action(type, name)
	{
		assert(mList[ActionsIndex]->first == ActionsKey);
	}
}
