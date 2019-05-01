#include "pch.h"
#include "ActionDeleteAction.h"
#include "Entity.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDeleteAction)

	const std::string ActionDeleteAction::ActionNameKey = "Action";
	const std::size_t ActionDeleteAction::ActionNameIndex = 2;

	Vector<Signature> ActionDeleteAction::Signatures()
	{
		return {
			{ ActionNameKey, Datum::DatumType::STRING, 1, offsetof(ActionDeleteAction, mActionName) }
		};
	}

	ActionDeleteAction::ActionDeleteAction(const std::string& action, const std::string& name) :
		Action(TypeIdClass(), name), mActionName(action)
	{
		assert(mList[ActionNameIndex]->first == ActionNameKey);
	}

	gsl::owner<ActionDeleteAction*> ActionDeleteAction::Clone() const
	{
		return new ActionDeleteAction(*this);
	}

	void ActionDeleteAction::Update(WorldState& worldState)
	{
		assert(worldState.World != nullptr);
		assert(worldState.Entity != nullptr);

		auto action = worldState.Entity->FindAction(mActionName);

		if (action != nullptr)
		{
			worldState.World->MarkForDeletion(*action);
		}

		worldState.World->MarkForDeletion(*this);
	}

	std::string& ActionDeleteAction::ActionName()
	{
		return mActionName;
	}

	const std::string& ActionDeleteAction::ActionName() const
	{
		return mActionName;
	}
}
