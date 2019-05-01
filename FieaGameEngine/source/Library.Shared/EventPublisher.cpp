#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const SubscriberListType& subscribers) : mSubscribers(&subscribers)
	{
	}

	void EventPublisher::Deliver() const
	{
		for (auto subscriber : *mSubscribers)
		{
			assert(subscriber != nullptr);
			subscriber->Notify(*this);
		}
	}
}
