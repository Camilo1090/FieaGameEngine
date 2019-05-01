#include "pch.h"
#include "ActionCreateAction.h"
#include "Entity.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	const std::string ActionCreateAction::PrototypeKey = "Prototype";
	const std::size_t ActionCreateAction::PrototypeIndex = 2;
	const std::string ActionCreateAction::ActionNameKey = "ActionName";
	const std::size_t ActionCreateAction::ActionNameIndex = 3;

	Vector<Signature> ActionCreateAction::Signatures()
	{
		return {
			{ PrototypeKey, Datum::DatumType::STRING, 1, offsetof(ActionCreateAction, mPrototype) },
			{ ActionNameKey, Datum::DatumType::STRING, 1, offsetof(ActionCreateAction, mActionName) }
		};
	}

	ActionCreateAction::ActionCreateAction(const std::string& className, const std::string& actionName, const std::string& name) :
		Action(TypeIdClass(), name), mPrototype(className), mActionName(actionName)
	{
		assert(mList[PrototypeIndex]->first == PrototypeKey);
		assert(mList[ActionNameIndex]->first == ActionNameKey);
	}

	gsl::owner<ActionCreateAction*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		assert(worldState.World != nullptr);
		assert(worldState.Entity != nullptr);

		auto action = worldState.Entity->CreateAction(mPrototype, mActionName);
		assert(action != nullptr);
		// pass parameters to created action
		for (auto pair : AuxiliaryAttributes())
		{
			auto datum = action->Find(pair->first);
			if (datum != nullptr)
			{
				Datum::MoveValues(pair->second, *datum);
			}
			else
			{
				action->Append(std::move(*pair));
			}
		}

		worldState.World->MarkForDeletion(*this);
	}

	std::string& ActionCreateAction::Prototype()
	{
		return mPrototype;
	}

	const std::string& ActionCreateAction::Prototype() const
	{
		return mPrototype;
	}

	std::string& ActionCreateAction::ActionName()
	{
		return mActionName;
	}

	const std::string& ActionCreateAction::ActionName() const
	{
		return mActionName;
	}
}
