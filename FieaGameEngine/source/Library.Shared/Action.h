#pragma once

#include "Attributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Represents a possible action for entities, sectors and worlds
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:
		/// <summary>
		/// signatures of action
		/// </summary>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">action</param>
		/// <returns>action</returns>
		Action& operator=(const Action&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">action</param>
		/// <returns>action</returns>
		Action& operator=(Action&&) = default;

		/// <summary>
		/// default destructor
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// pure virtual update for derived classes
		/// </summary>
		/// <param name="">world state</param>
		virtual void Update(WorldState&) = 0;

		/// <summary>
		/// clone
		/// </summary>
		/// <returns>action</returns>
		virtual gsl::owner<Action*> Clone() const override = 0;

		/// <summary>
		/// gets the name
		/// </summary>
		/// <returns>name</returns>
		std::string& Name();

		/// <summary>
		/// gets the name
		/// </summary>
		/// <returns>name</returns>
		const std::string& Name() const;

		/// <summary>
		/// sets the name
		/// </summary>
		/// <param name="name">name</param>
		void SetName(const std::string& name);

	protected:
		/// <summary>
		/// constructor for derived classes
		/// </summary>
		/// <param name="type">actual type</param>
		explicit Action(RTTI::IdType type);

		/// <summary>
		/// constructor for derived classes
		/// </summary>
		/// <param name="type">actual type</param>
		/// <param name="name">name</param>
		Action(RTTI::IdType type, const std::string& name);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">action</param>
		Action(const Action&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">action</param>
		Action(Action&&) = default;

		/// <summary>
		/// name of this action
		/// </summary>
		std::string mName;
	};
}
