#pragma once

#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine
{
	/// <summary>
	/// action that queues an event with a subtype and a delay
	/// </summary>
	class ActionEvent final : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)

	public:
		/// <summary>
		/// key for subtype attribute
		/// </summary>
		static const std::string SubtypeKey;

		/// <summary>
		/// key for delay attribute
		/// </summary>
		static const std::string DelayKey;

		/// <summary>
		/// signatures
		/// </summary>
		/// <returns>signatures</returns>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// constructor with optional subtype and action name
		/// </summary>
		/// <param name="subtype">subtype</param>
		/// <param name="name">name</param>
		explicit ActionEvent(const std::string& subtype = std::string(), const std::string& name = std::string());

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">action</param>
		ActionEvent(const ActionEvent&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">action</param>
		ActionEvent(ActionEvent&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">action</param>
		/// <returns>action</returns>
		ActionEvent& operator=(const ActionEvent&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">action</param>
		/// <returns>action</returns>
		ActionEvent& operator=(ActionEvent&&) = default;

		/// <summary>
		/// destructor
		/// </summary>
		virtual ~ActionEvent() = default;

		/// <summary>
		/// clones this action
		/// </summary>
		/// <returns>action</returns>
		virtual gsl::owner<ActionEvent*> Clone() const override;

		/// <summary>
		/// updates this action
		/// </summary>
		/// <param name="worldState">world state</param>
		virtual void Update(WorldState& worldState) override;

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

		/// <summary>
		/// gets the delay
		/// </summary>
		/// <returns>delay</returns>
		std::int32_t GetDelay() const;

		/// <summary>
		/// sets the delay
		/// </summary>
		/// <param name="delay">delay</param>
		void SetDelay(const std::int32_t& delay);

	private:
		/// <summary>
		/// subtype of the event to be queued
		/// </summary>
		std::string mSubtype;

		/// <summary>
		/// delay for the event to be queued
		/// </summary>
		std::int32_t mDelay;
	};

	CONCRETE_FACTORY(ActionEvent, Scope)
}
