#include "Event.h"

namespace FieaGameEngine
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>)

	template <typename T>
	typename Event<T>::SubscriberListType Event<T>::sSubscribers;

	template <typename T>
	std::mutex Event<T>::sSubscribersMutex;

	template <typename T>
	void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lock(sSubscribersMutex);
		sSubscribers.emplace(&subscriber);
	}

	template <typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lock(sSubscribersMutex);
		sSubscribers.erase(&subscriber);
	}

	template <typename T>
	void Event<T>::UnsubscribeAll()
	{
		std::lock_guard<std::mutex> lock(sSubscribersMutex);
		sSubscribers.clear();
	}

	template <typename T>
	inline Event<T>::Event(const T& message) : EventPublisher(sSubscribers, sSubscribersMutex), mMessage(message)
	{
	}

	template <typename T>
	inline Event<T>::Event(T&& message) : EventPublisher(sSubscribers, sSubscribersMutex), mMessage(std::move(message))
	{
	}

	template <typename T>
	inline const T& Event<T>::Message() const
	{
		return mMessage;
	}
}
