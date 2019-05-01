#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(const SubscriberListType& subscribers, std::mutex& mutex) : mSubscribers(&subscribers), mSubscribersMutex(&mutex)
	{
	}

	void EventPublisher::Deliver()
	{
		{
			std::lock_guard<std::mutex> lock(*mSubscribersMutex);
			mJobs.Reserve(mSubscribers->size());
			for (auto subscriber : *mSubscribers)
			{
				assert(subscriber != nullptr);
				mJobs.EmplaceBack(std::async(std::launch::async, &EventSubscriber::Notify, subscriber, std::ref(*this)));
			}
		}

		for (auto& job : mJobs)
		{
			try
			{
				job.get();
			}
			catch (...)
			{
				mJobs.Clear();
				throw;
			}
		}

		mJobs.Clear();
	}
}
