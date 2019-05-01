#pragma once

#include "JsonParseMaster.h"
#include "RTTI.h"
#include <json/value.h>
#include <gsl/gsl>

namespace FieaGameEngine
{
	/// <summary>
	/// abstract base class for creating helpers to parse different grammars
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)
	
	public:
		/// <summary>
		/// default constructor
		/// </summary>
		IJsonParseHelper() = default;

		/// <summary>
		/// default copy constructor
		/// </summary>
		/// <param name="">helper</param>
		IJsonParseHelper(const IJsonParseHelper&) = default;

		/// <summary>
		/// default move constructor
		/// </summary>
		/// <param name="">helper</param>
		IJsonParseHelper(IJsonParseHelper&&) = default;

		/// <summary>
		/// default copy assignment
		/// </summary>
		/// <param name="">helper</param>
		/// <returns>reference</returns>
		IJsonParseHelper& operator=(const IJsonParseHelper&) = default;

		/// <summary>
		/// default move assignment
		/// </summary>
		/// <param name="">helper</param>
		/// <returns>reference</returns>
		IJsonParseHelper& operator=(IJsonParseHelper&&) = default;

		/// <summary>
		/// default destructor
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// compares helpers for equality, same class
		/// </summary>
		/// <param name="other">helper</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const IJsonParseHelper& other) const;

		/// <summary>
		/// compares helpers for inequality, different class
		/// </summary>
		/// <param name="other">helper</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const IJsonParseHelper& other) const;

		/// <summary>
		/// creates a new helper of this type
		/// </summary>
		/// <returns>pointer to new helper</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;

		/// <summary>
		/// initializes this helper
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// attempts to start parsing the json value
		/// </summary>
		/// <param name="data">shared data</param>
		/// <param name="name">name</param>
		/// <param name="value">value</param>
		/// <param name="isInArray">is element of an array</param>
		/// <returns>true if handled value, false otherwise</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& data, const std::string& name, const Json::Value& value, size_t index = 0) = 0;

		/// <summary>
		/// attempts to finish parsing the json value
		/// </summary>
		/// <param name="data">shared data</param>
		/// <param name="name">name</param>
		/// <returns>true if handled, false otherwise</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& data, const std::string& name) = 0;

		/// <summary>
		/// compares helpers for equality, same class
		/// </summary>
		/// <param name="other">helper</param>
		/// <returns>true if equal, false otherwise</returns>
		virtual bool Equals(const RTTI* other) const override;

		/// <summary>
		/// string representation of this class
		/// </summary>
		/// <returns>string</returns>
		virtual std::string ToString() const override;
	};
}
