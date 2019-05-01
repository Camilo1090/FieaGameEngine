#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed)

	const std::string ReactionAttributed::SubtypeKey = "Subtype";

	const std::size_t ReactionAttributed::SubtypeKeyIndex = 3;

	Vector<Signature> ReactionAttributed::Signatures()
	{
		return {
			{ SubtypeKey, Datum::DatumType::STRING, 1, offsetof(ReactionAttributed, mSubtype) }
		};
	}

	ReactionAttributed::ReactionAttributed(const std::string& subtype, const std::string& name) : Reaction(TypeIdClass(), name), mSubtype(subtype)
	{
		assert(mList[SubtypeKeyIndex]->first == SubtypeKey);
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& other) : Reaction(other), mSubtype(other.mSubtype)
	{
		assert(mList[SubtypeKeyIndex]->first == SubtypeKey);
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& other) : Reaction(std::move(other)), mSubtype(std::move(mSubtype))
	{
		assert(mList[SubtypeKeyIndex]->first == SubtypeKey);
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	gsl::owner<ReactionAttributed*> ReactionAttributed::Clone() const
	{
		return new ReactionAttributed(*this);
	}

	void ReactionAttributed::Notify(const EventPublisher& event)
	{
		assert(event.Is(Event<EventMessageAttributed>::TypeIdClass()));
		auto actualEvent = static_cast<const Event<EventMessageAttributed>*>(&event);
		const auto& message = actualEvent->Message();

		std::string& subtype = At(SubtypeKeyIndex).Get<std::string>();
		if (subtype == message.Subtype())
		{
			// go through all actions
			auto& actions = Actions();
			const auto& parameters = message.AuxiliaryAttributes();
			for (size_t i = 0; i < actions.Size(); ++i)
			{
				// pass parameters to action
				assert(actions[i].Is(Action::TypeIdClass()));
				auto& action = static_cast<Action&>(actions[i]);
				for (auto pair : parameters)
				{
					// only existing entries are copied
					// therefore if you're expecting a parameter it had better be a prescribed or auxiliary attribute in the action
					auto datum = action.Find(pair->first);
					if (datum != nullptr)
					{
						Datum::CopyValues(pair->second, *datum);
					}
				}

				// execute action
				action.Update(message.GetWorld()->GetWorldState());
			}
		}
	}

	std::string& ReactionAttributed::Subtype()
	{
		return mSubtype;
	}

	const std::string& ReactionAttributed::Subtype() const
	{
		return mSubtype;
	}

	void ReactionAttributed::SetSubtype(const std::string& subtype)
	{
		mSubtype = subtype;
	}
}
