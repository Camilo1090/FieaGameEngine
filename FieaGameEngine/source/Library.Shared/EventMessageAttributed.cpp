#include "pch.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	const std::string EventMessageAttributed::SubtypeKey = "Subtype";

	Vector<Signature> EventMessageAttributed::Signatures()
	{
		return {
			{ SubtypeKey, Datum::DatumType::STRING, 1, offsetof(EventMessageAttributed, mSubtype) }
		};
	}

	EventMessageAttributed::EventMessageAttributed(World& world) : 
		EventMessageAttributed(world, std::string())
	{
	}

	EventMessageAttributed::EventMessageAttributed(World& world, const std::string& subtype) : 
		Attributed(TypeIdClass()), mWorld(&world), mSubtype(subtype)
	{
	}

	gsl::owner<EventMessageAttributed*> EventMessageAttributed::Clone() const
	{
		return new EventMessageAttributed(*this);
	}

	std::string & EventMessageAttributed::Subtype()
	{
		return mSubtype;
	}

	const std::string & EventMessageAttributed::Subtype() const
	{
		return mSubtype;
	}

	void EventMessageAttributed::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	World* EventMessageAttributed::GetWorld() const
	{
		return mWorld;
	}

	void EventMessageAttributed::SetWorld(World& world)
	{
		mWorld = &world;
	}
}
