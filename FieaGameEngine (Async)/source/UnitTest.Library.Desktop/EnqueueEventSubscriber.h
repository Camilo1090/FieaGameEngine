#pragma once

#include "EventSubscriber.h"
#include <mutex>

namespace UnitTestLibraryDesktop
{
	class EnqueueEventSubscriber : public FieaGameEngine::EventSubscriber
	{
		RTTI_DECLARATIONS(EnqueueEventSubscriber, EventSubscriber)

	public:
		EnqueueEventSubscriber();
		virtual ~EnqueueEventSubscriber();

		virtual void Notify(const FieaGameEngine::EventPublisher& event) override;

		int Notifications = 0;
		bool Enqueue = true;
		bool Unsubscribe = false;
		bool SubscribeAgain = false;
		bool Throw = false;

	private:
		std::mutex mMutex;
	};
}
