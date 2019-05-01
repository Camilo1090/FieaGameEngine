#include "pch.h"
#include "EnqueueEventSubscriber.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "World.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(EnqueueEventSubscriber)

	EnqueueEventSubscriber::EnqueueEventSubscriber()
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	EnqueueEventSubscriber::~EnqueueEventSubscriber()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	void EnqueueEventSubscriber::Notify(const EventPublisher& event)
	{
		auto fooEvent = event.As<Event<EventMessageAttributed>>();

		if (fooEvent != nullptr)
		{
			std::lock_guard<std::mutex> lock(mMutex);
			++Notifications;

			if (Enqueue)
			{
				fooEvent->Message().GetWorld()->GetEventQueue()->EnqueueEvent(
					std::make_shared<Event<EventMessageAttributed>>(EventMessageAttributed(*fooEvent->Message().GetWorld())),
					*fooEvent->Message().GetWorld()->GetWorldState().GameTime);
			}

			if (Unsubscribe)
			{
				Event<EventMessageAttributed>::Unsubscribe(*this);
			}

			if (SubscribeAgain)
			{
				Event<EventMessageAttributed>::Subscribe(*this);
			}

			if (Throw)
			{
				throw std::exception("test exception.");
			}
		}
	}
}
