#pragma once

#include "Reaction.h"
#include <mutex>

namespace FieaGameEngine
{
	/// <summary>
	/// concrete reaction that subscribes to events of message attributed
	/// </summary>
	class ReactionAttributed final : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction)

	public:
		/// <summary>
		/// key for subtype
		/// </summary>
		static const std::string SubtypeKey;

		/// <summary>
		/// index for subtype
		/// </summary>
		static const std::size_t SubtypeKeyIndex;

		/// <summary>
		/// signatures
		/// </summary>
		/// <returns>signatures</returns>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// constructor with subtype and action name, subscribes to publisher
		/// </summary>
		/// <param name="subtype">subtype</param>
		/// <param name="name">name</param>
		explicit ReactionAttributed(const std::string& subtype = std::string(), const std::string& name = std::string());
		
		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">reaction</param>
		ReactionAttributed(const ReactionAttributed& other);

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">reaction</param>
		ReactionAttributed(ReactionAttributed&& other);

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">reaction</param>
		/// <returns>reaction</returns>
		ReactionAttributed& operator=(const ReactionAttributed&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">reaction</param>
		/// <returns>reaction</returns>
		ReactionAttributed& operator=(ReactionAttributed&&) = default;

		/// <summary>
		/// destructor, unsubscribe from publisher
		/// </summary>
		virtual ~ReactionAttributed();

		/// <summary>
		/// clones this reaction
		/// </summary>
		/// <returns>reaction</returns>
		virtual gsl::owner<ReactionAttributed*> Clone() const override;

		/// <summary>
		/// gets invoked when events fire
		/// </summary>
		/// <param name="event">fired event</param>
		virtual void Notify(const EventPublisher& event) override;

		/// <summary>
		/// gets the subtype
		/// </summary>
		/// <returns>subtype</returns>
		std::string& Subtype();

		/// <summary>
		/// gets the subtype
		/// </summary>
		/// <returns>subtype</returns>
		const std::string& Subtype() const;

		/// <summary>
		/// sets the subtype
		/// </summary>
		/// <param name="subtype">subtype</param>
		void SetSubtype(const std::string& subtype);

	private:
		/// <summary>
		/// subtype of events this reaction reacts to
		/// </summary>
		std::string mSubtype;

		std::mutex mMutex;
	};

	CONCRETE_FACTORY(ReactionAttributed, Scope)
}
