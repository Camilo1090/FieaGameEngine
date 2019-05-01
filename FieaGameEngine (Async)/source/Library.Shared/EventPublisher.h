#pragma once

#include "RTTI.h"
#include "vector.h"
#include <mutex>
#include <set>
#include <future>

namespace FieaGameEngine
{
	class EventSubscriber;

	/// <summary>
	/// abstract base class for events to derive from
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	public:
		/// <summary>
		/// destructor
		/// </summary>
		virtual ~EventPublisher() = default;

		/// <summary>
		/// notify all subscriber when this event is fired
		/// </summary>
		void Deliver();

	protected:
		using SubscriberListType = std::set<EventSubscriber*>;

		/// <summary>
		/// constructor with list of subscriber to set this event's list of subscribers
		/// </summary>
		/// <param name="subscribers">subscribers</param>
		EventPublisher(const SubscriberListType& subscribers, std::mutex& mutex);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">event</param>
		EventPublisher(const EventPublisher&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">event</param>
		EventPublisher(EventPublisher&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">event</param>
		/// <returns>event</returns>
		EventPublisher& operator=(const EventPublisher&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">event</param>
		/// <returns>event</returns>
		EventPublisher& operator=(EventPublisher&&) = default;

	private:
		/// <summary>
		/// pointer to this event's list of subscribers
		/// </summary>
		const SubscriberListType* mSubscribers;

		/// <summary>
		/// mutex for subscriber list
		/// </summary>
		std::mutex* mSubscribersMutex;

		/// <summary>
		/// list of jobs of notifies
		/// </summary>
		Vector<std::future<void>> mJobs;
	};
}
