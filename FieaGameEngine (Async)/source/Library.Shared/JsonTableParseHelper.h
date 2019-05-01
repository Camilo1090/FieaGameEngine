#pragma once

#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"
#include "Stack.h"
#include "HashMap.h"
#include "Datum.h"

namespace FieaGameEngine
{
	class Scope;

	/// <summary>
	/// structure to hold information need to set a value
	/// name of the datum, value to set, position to set and table the datum belongs to
	/// </summary>
	struct StackFrame final
	{
		StackFrame(const std::string& name, Scope& table);

		const std::string& Name;
		Scope& Table;
		const Json::Value* Value = nullptr;
		size_t Index = 0;
		std::string ClassName;
	};

	/// <summary>
	/// main class for parsing tables with the following grammar:
	/// { "Health": { "type": "integer", "value": 100 } }
	/// special keys: 'type' and 'value'
	/// </summary>
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper)

	public:
		/// <summary>
		/// creates a new helper of this type
		/// </summary>
		virtual gsl::owner<IJsonParseHelper*> Create() const override;

		/// <summary>
		/// starts parsing the passed key value pair
		/// </summary>
		/// <param name="data">shared data</param>
		/// <param name="name">name</param>
		/// <param name="value">value</param>
		/// <param name="isInArray">is an array element</param>
		/// <param name="index">index in array</param>
		/// <returns>true if handled, false otherwise</returns>
		virtual bool StartHandler(JsonParseMaster::SharedData& data, const std::string& name, const Json::Value& value, size_t index = 0) override;

		/// <summary>
		/// finishes parsing the value associated with name
		/// </summary>
		/// <param name="data">shared data</param>
		/// <param name="name">name</param>
		/// <returns>true if handled, false otherwise</returns>
		virtual bool EndHandler(JsonParseMaster::SharedData& data, const std::string& name) override;

	private:
		/// <summary>
		/// stack of frames to correctly parse nested values
		/// </summary>
		Stack<StackFrame> mStack;

		/// <summary>
		/// sets the parsed value as integer at the given index
		/// </summary>
		/// <param name="datum">datum</param>
		/// <param name="value">value</param>
		/// <param name="index">index</param>
		void SetInteger(Scope& scope, const std::string& name, const Json::Value& value, size_t index);

		/// <summary>
		/// sets the parsed value as float at the given index
		/// </summary>
		/// <param name="datum">datum</param>
		/// <param name="value">value</param>
		/// <param name="index">index</param>
		void SetFloat(Scope& scope, const std::string& name, const Json::Value& value, size_t index);

		/// <summary>
		/// sets the parsed value as string/vector/matrix at the given index
		/// </summary>
		/// <param name="datum">datum</param>
		/// <param name="value">value</param>
		/// <param name="index">index</param>
		void SetFromString(Scope& scope, const std::string& name, const Json::Value& value, size_t index);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="scope"></param>
		/// <param name="name"></param>
		/// <param name="value"></param>
		/// <param name="index"></param>
		void SetReference(Scope& scope, const std::string& name, const Json::Value& value, size_t index);

	private:
		/// <summary>
		/// map of grammar data types to datum types
		/// </summary>
		static const HashMap<std::string, Datum::DatumType> sDataTypes;

		using ValueSetter = void (JsonTableParseHelper::*)(Scope&, const std::string&, const Json::Value&, size_t);
		static const ValueSetter sValueSetters[static_cast<size_t>(Datum::DatumType::LAST) + 1];
	};

	/// <summary>
	/// table shared data for parsing tables as scopes
	/// </summary>
	class TableSharedData final : public JsonParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TableSharedData, SharedData)

	public:
		/// <summary>
		/// constructor with a valid scope
		/// </summary>
		TableSharedData(Scope& table);

		/// <summary>
		/// destructor, deletes memory if owned
		/// </summary>
		virtual ~TableSharedData();

		/// <summary>
		/// creates a new shared data of this type
		/// </summary>
		/// <returns>pointer to the new shared data</returns>
		virtual gsl::owner<SharedData*> Create() const override;

		/// <summary>
		/// gets the scope of this shared data
		/// </summary>
		/// <returns>scope reference</returns>
		Scope& GetTable();

	private:
		/// <summary>
		/// root scope containing all parsed data
		/// </summary>
		Scope* mTable = nullptr;

		/// <summary>
		/// if this shared data was created from another object, it owns its scope
		/// </summary>
		bool mOwnsTable = false;
	};
}
