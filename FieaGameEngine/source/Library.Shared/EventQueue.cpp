#include "pch.h"
#include "EventQueue.h"
#include <algorithm>

namespace FieaGameEngine
{
	void EventQueue::EnqueueEvent(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, const DurationType& delay)
	{
		const auto& it = Find(mPendingQueue, event);

		if (it != mPendingQueue.end())
		{
			(*it).TimeEnqueued = gameTime.CurrentTime();
			(*it).Delay = delay;
		}
		else
		{
			mPendingQueue.EmplaceBack(std::move(event), gameTime.CurrentTime(), delay);
		}
	}

	void EventQueue::UpdateEvent(const std::shared_ptr<EventPublisher>& event, const DurationType& delay)
	{
		const auto& it = Find(mPendingQueue, event);

		if (it != mPendingQueue.end())
		{
			(*it).Delay = delay;
		}
	}

	bool EventQueue::RemoveEvent(const std::shared_ptr<EventPublisher>& event)
	{
		const auto& it = Find(mPendingQueue, event);

		if (it != mPendingQueue.end())
		{
			mPendingQueue.Remove(it);
			return true;
		}

		return false;
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		// move all expired events to the back of the list
		auto expiredBegin = std::partition(mPendingQueue.begin(), mPendingQueue.end(), 
			[&gameTime](const QueueEntry& queued) { return !queued.IsExpired(gameTime.CurrentTime()); });

		// move all expired events from the pending queue to the expired queue
		std::move(expiredBegin, mPendingQueue.end(), std::back_inserter(mExpiredQueue));
		mPendingQueue.Remove(expiredBegin, mPendingQueue.end());

		// deliver all expired events
		for (const auto& entry : mExpiredQueue)
		{
			entry.Event->Deliver();
		}

		// clear expired queue
		mExpiredQueue.Clear();
	}

	void EventQueue::Clear()
	{
		mPendingQueue.Clear();
	}

	void EventQueue::ShrinkToFit()
	{
		mPendingQueue.ShrinkToFit();
		mExpiredQueue.ShrinkToFit();
	}

	bool EventQueue::IsEmpty() const
	{
		return mPendingQueue.IsEmpty();
	}

	size_t EventQueue::Size() const
	{
		return mPendingQueue.Size();
	}

	Vector<EventQueue::QueueEntry>::Iterator EventQueue::Find(Vector<QueueEntry>& list, const std::shared_ptr<EventPublisher>& event)
	{
		return std::find_if(list.begin(), list.end(),
			[&event](const QueueEntry& entry) { return entry.Event == event; });
	}

	EventQueue::QueueEntry::QueueEntry(std::shared_ptr<EventPublisher>&& event, const TimePointType& timeEnqueued, const DurationType& delay) :
		Event(std::move(event)), TimeEnqueued(timeEnqueued), Delay(delay)
	{
	}

	bool EventQueue::QueueEntry::IsExpired(const TimePointType& currentTime) const
	{
		return TimeEnqueued + Delay < currentTime;
	}
}
