#pragma once

#include "ActionList.h"
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	/// <summary>
	/// abstract base class for reactions
	/// executes the list of actions when notified
	/// </summary>
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)

	public:
		/// <summary>
		/// signatures
		/// </summary>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// destructor
		/// </summary>
		virtual ~Reaction() = default;

		/// <summary>
		/// clones this reaction
		/// </summary>
		/// <returns>reaction</returns>
		virtual gsl::owner<Reaction*> Clone() const override = 0;

		/// <summary>
		/// does nothing
		/// </summary>
		/// <param name="worldState">world state</param>
		virtual void Update(WorldState& worldState) override;

		/// <summary>
		/// all derived reactions must implement this
		/// </summary>
		/// <param name="">event</param>
		virtual void Notify(const EventPublisher&) override = 0;

	protected:
		/// <summary>
		/// constructor wiht type and name
		/// </summary>
		/// <param name="type">type</param>
		/// <param name="name">name</param>
		Reaction(RTTI::IdType type, const std::string& name);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">reaction</param>
		Reaction(const Reaction&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">reaction</param>
		Reaction(Reaction&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">reaction</param>
		/// <returns>reaction</returns>
		Reaction& operator=(const Reaction&) = default;

		/// <summary>
		/// move asignment
		/// </summary>
		/// <param name="">reaction</param>
		/// <returns>reaction</returns>
		Reaction& operator=(Reaction&&) = default;
	};
}
