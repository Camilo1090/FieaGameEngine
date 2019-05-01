#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action)

	Vector<Signature> Action::Signatures()
	{
		return {
			{ "Name", Datum::DatumType::STRING, 1, offsetof(Action, mName) }
		};
	}

	Action::Action(RTTI::IdType type) : Attributed(type)
	{
	}

	Action::Action(RTTI::IdType type, const std::string& name) : Attributed(type), mName(name)
	{
	}

	std::string& Action::Name()
	{
		return mName;
	}

	const std::string& Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string& name)
	{
		mName = name;
	}
}
