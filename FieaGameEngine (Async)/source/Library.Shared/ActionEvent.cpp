#include "pch.h"
#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionEvent)

	const std::string ActionEvent::SubtypeKey = "Subtype";

	const std::string ActionEvent::DelayKey = "Delay";

	Vector<Signature> ActionEvent::Signatures()
	{
		return {
			{ SubtypeKey, Datum::DatumType::STRING, 1, offsetof(ActionEvent, mSubtype) },
			{ DelayKey, Datum::DatumType::INTEGER, 1, offsetof(ActionEvent, mDelay) }
		};
	}

	ActionEvent::ActionEvent(const std::string& subtype, const std::string& name) : Action(TypeIdClass(), name), mSubtype(subtype)
	{
	}

	gsl::owner<ActionEvent*> ActionEvent::Clone() const
	{
		return new ActionEvent(*this);
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		assert(worldState.World != nullptr);
		EventMessageAttributed message(*worldState.World, mSubtype);
		for (auto pair : AuxiliaryAttributes())
		{
			assert(pair != nullptr);
			message.AppendAuxiliaryAttribute(*pair);
		}
		
		std::shared_ptr event = std::make_shared<Event<EventMessageAttributed>>(std::move(message));

		assert(worldState.World->GetEventQueue() != nullptr);
		worldState.World->GetEventQueue()->EnqueueEvent(event, *worldState.GameTime, std::chrono::milliseconds(mDelay));
		worldState.World->MarkForDeletion(*this);
	}

	std::string& ActionEvent::Subtype()
	{
		return mSubtype;
	}

	const std::string& ActionEvent::Subtype() const
	{
		return mSubtype;
	}

	void ActionEvent::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}

	std::int32_t ActionEvent::GetDelay() const
	{
		return mDelay;
	}

	void ActionEvent::SetDelay(const std::int32_t& delay)
	{
		mDelay = delay;
	}
}
