#pragma once

#include "RTTI.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	/// <summary>
	/// abstract base class for event subscribers to derive from
	/// </summary>
	class EventSubscriber : public RTTI
	{
		RTTI_DECLARATIONS(EventSubscriber, RTTI)

	public:
		/// <summary>
		/// destructor
		/// </summary>
		virtual ~EventSubscriber() = default;

		/// <summary>
		/// all events this subscriber is subscribed to call this function when fired
		/// </summary>
		/// <param name="">event</param>
		virtual void Notify(const EventPublisher&) = 0;

	protected:
		/// <summary>
		/// constructor
		/// </summary>
		EventSubscriber() = default;

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">subscriber</param>
		EventSubscriber(const EventSubscriber&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">subscriber</param>
		EventSubscriber(EventSubscriber&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">subscriber</param>
		/// <returns>subscriber</returns>
		EventSubscriber& operator=(const EventSubscriber&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">subscriber</param>
		/// <returns>subscriber</returns>
		EventSubscriber& operator=(EventSubscriber&&) = default;
	};
}
