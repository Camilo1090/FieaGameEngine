#include "pch.h"
#include "FooSubscriber.h"
#include "Event.h"
#include "Foo.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(FooSubscriber)

	FooSubscriber::~FooSubscriber()
	{
		Event<Foo>::Unsubscribe(*this);
	}

	void FooSubscriber::Notify(const EventPublisher& event)
	{
		auto actualEvent = event.As<Event<Foo>>();

		if (actualEvent == nullptr)
		{
			throw std::exception("Unexpected event type.");
		}

		const Foo& message = actualEvent->Message();
		Data = message.Data();
	}

	void FooSubscriber::Subscribe()
	{
		Event<Foo>::Subscribe(*this);
	}
}
