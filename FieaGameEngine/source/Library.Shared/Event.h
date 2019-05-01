#pragma once

#include "EventPublisher.h"
#include "vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// templated event class for creating different types of payloads
	/// </summary>
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<T>, EventPublisher)
		
	public:
		/// <summary>
		/// subscribes to his event
		/// </summary>
		/// <param name="subscriber">subscriber</param>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// unsubscribe from this event
		/// </summary>
		/// <param name="subscriber">subscriber</param>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// unsubscribes all subscribers
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// constructor with message
		/// </summary>
		/// <param name="message">message</param>
		explicit Event(const T& message = T());

		/// <summary>
		/// constructor with message
		/// </summary>
		/// <param name="message">message</param>
		explicit Event(T&& message);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">event</param>
		Event(const Event&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">event</param>
		Event(Event&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">event</param>
		/// <returns>event</returns>
		Event& operator=(const Event&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">event</param>
		/// <returns>event</returns>
		Event& operator=(Event&&) = default;

		/// <summary>
		/// destructor
		/// </summary>
		virtual ~Event() = default;

		/// <summary>
		/// gets the message
		/// </summary>
		/// <returns></returns>
		const T& Message() const;

	private:
		/// <summary>
		/// list of subscribers
		/// </summary>
		static SubscriberListType sSubscribers;

		/// <summary>
		/// message to deliver
		/// </summary>
		T mMessage;
	};
}

#include "Event.inl"
