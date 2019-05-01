#pragma once

#include "vector.h"
#include "EventPublisher.h"
#include "GameTime.h"
#include <chrono>
#include <mutex>
#include <future>

namespace FieaGameEngine
{
	/// <summary>
	/// queue of events that fires them based on expiration
	/// </summary>
	class EventQueue final
	{
	private:
		using TimePointType = std::chrono::high_resolution_clock::time_point;
		using DurationType = std::chrono::milliseconds;

		/// <summary>
		/// internal type to represent an entry of the queue
		/// </summary>
		struct QueueEntry final
		{
			/// <summary>
			/// constructor
			/// </summary>
			/// <param name="event">event pointer</param>
			/// <param name="timeEnqueued">time enqueued</param>
			/// <param name="delay">delay</param>
			QueueEntry(std::shared_ptr<EventPublisher>&& event, const TimePointType& timeEnqueued, const DurationType& delay);

			/// <summary>
			/// determines if this event has expired based on the current time
			/// </summary>
			/// <param name="currentTime">current time</param>
			/// <returns>true if expired, false otherwise</returns>
			bool IsExpired(const TimePointType& currentTime) const;

			/// <summary>
			/// event
			/// </summary>
			std::shared_ptr<EventPublisher> Event;

			/// <summary>
			/// time enqueued
			/// </summary>
			TimePointType TimeEnqueued;

			/// <summary>
			/// delay
			/// </summary>
			DurationType Delay;
		};

	public:
		/// <summary>
		/// constructor
		/// </summary>
		EventQueue() = default;

		EventQueue(const EventQueue&) = delete;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">queue</param>
		EventQueue(EventQueue&&) = default;

		EventQueue& operator=(const EventQueue&) = delete;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">queue</param>
		/// <returns>queue</returns>
		EventQueue& operator=(EventQueue&&) = default;

		/// <summary>
		/// destructor
		/// </summary>
		~EventQueue() = default;

		/// <summary>
		/// enqueues an event
		/// </summary>
		/// <param name="event">event</param>
		/// <param name="gameTime">current game time</param>
		/// <param name="delay">delay</param>
		void EnqueueEvent(std::shared_ptr<EventPublisher> event, const GameTime& gameTime, const DurationType& delay = DurationType());

		/// <summary>
		/// updates the delay of a pending quueue
		/// </summary>
		/// <param name="event">event</param>
		/// <param name="delay">delay</param>
		void UpdateEvent(const std::shared_ptr<EventPublisher>& event, const DurationType& delay);

		/// <summary>
		/// removes an enqueued event without firing it
		/// </summary>
		/// <param name="event">event</param>
		bool RemoveEvent(const std::shared_ptr<EventPublisher>& event);

		/// <summary>
		/// iterates through all events and fires those that have expired
		/// </summary>
		/// <param name="gameTime">game time</param>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// clears pending events
		/// </summary>
		void Clear();

		/// <summary>
		/// reclaims unused queue memory
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// whether or not the queue is empty
		/// </summary>
		/// <returns>true if empty, false otherwise</returns>
		bool IsEmpty() const;

		/// <summary>
		/// gets the number of enqueued events
		/// </summary>
		/// <returns>number of enqueued events</returns>
		size_t Size() const;

	private:
		/// <summary>
		/// pending events
		/// </summary>
		Vector<QueueEntry> mPendingQueue;

		/// <summary>
		/// mutex for pending events queue
		/// </summary>
		mutable std::mutex mPendingQueueMutex;

		/// <summary>
		/// internal list for storing expired events
		/// </summary>
		Vector<QueueEntry> mExpiredQueue;

		/// <summary>
		/// vector of jobs of event delivery
		/// </summary>
		Vector<std::future<void>> mJobs;

		/// <summary>
		/// finds an entry in the given list based on the given event
		/// </summary>
		/// <param name="list">list of entries</param>
		/// <param name="event">event</param>
		/// <returns>iterator</returns>
		Vector<QueueEntry>::Iterator Find(Vector<QueueEntry>& list, const std::shared_ptr<EventPublisher>& event);
	};
}
