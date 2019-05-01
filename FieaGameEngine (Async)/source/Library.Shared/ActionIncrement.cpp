#include "pch.h"
#include "ActionIncrement.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement)

	const std::string ActionIncrement::TargetKey = "Target";

	const std::string ActionIncrement::StepKey = "Step";

	Vector<Signature> ActionIncrement::Signatures()
	{
		return {
			{ TargetKey, Datum::DatumType::STRING, 1, offsetof(ActionIncrement, mTarget) },
			{ StepKey, Datum::DatumType::FLOAT, 1, offsetof(ActionIncrement, mStep) }
		};
	}

	ActionIncrement::ActionIncrement() : Action(TypeIdClass())
	{
	}

	ActionIncrement::ActionIncrement(const std::string& name) : Action(TypeIdClass(), name)
	{
	}

	gsl::owner<ActionIncrement*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	void ActionIncrement::Update(WorldState&)
	{
		mReference = Search(mTarget);
		if (mReference == nullptr)
		{
			throw std::exception("Unable to dereference target.");
		}

		if (mReference->Type() == Datum::DatumType::INTEGER)
		{
			mReference->Get<std::int32_t>() += static_cast<std::int32_t>(mStep);
		}
		else if (mReference->Type() == Datum::DatumType::FLOAT)
		{
			mReference->Get<float>() += mStep;
		}
		else
		{
			throw std::exception("Unsupported type.");
		}
	}
}
