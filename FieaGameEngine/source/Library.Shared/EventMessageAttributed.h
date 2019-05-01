#pragma once

#include "Attributed.h"

namespace FieaGameEngine
{
	class World;

	/// <summary>
	/// represents a payload to be delivered to subscribers when the event fires
	/// </summary>
	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed)

	public:
		/// <summary>
		/// key for subtype attribute
		/// </summary>
		static const std::string SubtypeKey;

		/// <summary>
		/// signatures
		/// </summary>
		static Vector<Signature> Signatures();

	public:
		/// <summary>
		/// constructor with world
		/// </summary>
		/// <param name="world">world</param>
		explicit EventMessageAttributed(World& world);

		/// <summary>
		/// constructor with world and subtype
		/// </summary>
		/// <param name="world">world</param>
		/// <param name="subtype">subtype</param>
		EventMessageAttributed(World& world, const std::string& subtype);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="">message</param>
		EventMessageAttributed(const EventMessageAttributed&) = default;

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="">message</param>
		EventMessageAttributed(EventMessageAttributed&&) = default;

		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="">message</param>
		/// <returns>message</returns>
		EventMessageAttributed& operator=(const EventMessageAttributed&) = default;

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="">message</param>
		/// <returns>message</returns>
		EventMessageAttributed& operator=(EventMessageAttributed&&) = default;

		/// <summary>
		/// destructor
		/// </summary>
		virtual ~EventMessageAttributed() = default;

		/// <summary>
		/// clones this message
		/// </summary>
		/// <returns>message</returns>
		virtual gsl::owner<EventMessageAttributed*> Clone() const override;

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
		/// <param name="subtype">new subtype</param>
		void SetSubtype(const std::string& subtype);

		/// <summary>
		/// gets the world
		/// </summary>
		/// <returns>world</returns>
		World* GetWorld() const;

		/// <summary>
		/// sets the world
		/// </summary>
		/// <param name="world">new world</param>
		void SetWorld(World& world);

	private:
		/// <summary>
		/// subtype, used to further disambiguate the event that holds this message
		/// </summary>
		std::string mSubtype;

		/// <summary>
		/// world, used to pass the current context to the subscribers
		/// </summary>
		World* mWorld = nullptr;
	};
}
