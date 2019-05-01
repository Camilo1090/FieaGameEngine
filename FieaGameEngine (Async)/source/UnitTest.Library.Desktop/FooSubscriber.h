#pragma once

#include "EventSubscriber.h"

namespace UnitTestLibraryDesktop
{
	class FooSubscriber : public FieaGameEngine::EventSubscriber
	{
		RTTI_DECLARATIONS(FooSubscriber, EventSubscriber)

	public:
		FooSubscriber() = default;
		virtual ~FooSubscriber();

		virtual void Notify(const FieaGameEngine::EventPublisher& event) override;

		void Subscribe();

		int Data = 0;
	};
}
