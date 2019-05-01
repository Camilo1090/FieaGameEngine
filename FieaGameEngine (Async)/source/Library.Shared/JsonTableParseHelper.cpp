#include "pch.h"
#include <string>
#include "JsonTableParseHelper.h"
#include "Scope.h"
#include "Factory.h"
#include <cstddef>
#include <functional>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper)

	const HashMap<std::string, Datum::DatumType> JsonTableParseHelper::sDataTypes = 
	{
		{ "integer", Datum::DatumType::INTEGER },
		{ "float", Datum::DatumType::FLOAT },
		{ "vector", Datum::DatumType::VECTOR },
		{ "matrix", Datum::DatumType::MATRIX },
		{ "string", Datum::DatumType::STRING },
		{ "table", Datum::DatumType::TABLE }
	};

	const JsonTableParseHelper::ValueSetter JsonTableParseHelper::sValueSetters[] =
	{
		nullptr,
		&JsonTableParseHelper::SetInteger,
		&JsonTableParseHelper::SetFloat,
		&JsonTableParseHelper::SetFromString,
		&JsonTableParseHelper::SetFromString,
		&JsonTableParseHelper::SetFromString,
		nullptr,
		nullptr
	};

	gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
	{
		return new JsonTableParseHelper();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseMaster::SharedData& data, const std::string& name, const Json::Value& value, size_t index)
	{
		TableSharedData* tableData = data.As<TableSharedData>();
		if (tableData != nullptr)
		{
			if (name == "class")
			{
				StackFrame& frame = mStack.Top();
				frame.ClassName = value.asString();
			}
			// set the type of the datum
			else if (name == "type")
			{
				Datum::DatumType type = sDataTypes.At(value.asString());
				StackFrame& frame = mStack.Top();
				frame.Table[frame.Name].SetType(type);
			}
			// add the value and its index to the stack frame
			else if (name == "value")
			{
				StackFrame& frame = mStack.Top();
				frame.Value = &value;
				frame.Index = index;
				Datum& datum = frame.Table[frame.Name];

				// if the value is an object we need a scope to populate
				if (value.isObject())
				{
					if (!frame.ClassName.empty())
					{
						if (datum.Size() <= index)
						{
							Scope* nestedScope = Factory<Scope>::Create(frame.ClassName);
							assert(nestedScope != nullptr);
							frame.Table.Adopt(frame.Name, *nestedScope);
						}
						else if (!datum[index].Is(frame.ClassName))
						{
							throw std::exception("Scope type mismatch.");
						}
					}
					else if (datum.Size() <= index)
					{
						frame.Table.AppendScope(frame.Name);
					}
				}

				// add/set value
				auto func = sValueSetters[static_cast<size_t>(datum.Type())];
				if (func != nullptr)
				{
					std::invoke(func, this, frame.Table, frame.Name, value, index);
				}
			}
			// if not type or value then it's a key
			else
			{
				// root scope is the context of the key if stack is empty
				if (mStack.IsEmpty())
				{
					tableData->GetTable().Append(name);
					mStack.Emplace(name, tableData->GetTable());
				}
				// the context is the top of the stack if available
				else
				{
					StackFrame& frame = mStack.Top();
					Scope& context = frame.Table[frame.Name][frame.Index];
					context.Append(name);
					mStack.Emplace(name, context);
				}
			}

			return true;
		}

		return false;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseMaster::SharedData& data, const std::string& name)
	{
		TableSharedData* tableData = data.As<TableSharedData>();
		if (tableData != nullptr)
		{
			StackFrame& frame = mStack.Top();
			
			// if we end type or value but are not done with the whole key/value pair
			if (name == "type" || name == "value" || name == "class")
			{
				return true;
			}
			// if we are done parsing a key/value pair
			else if (&frame.Name == &name)
			{
				mStack.Pop();
				return true;
			}
			
		}

		return false;
	}

	void JsonTableParseHelper::SetInteger(Scope& scope, const std::string& name, const Json::Value& value, size_t index)
	{
		Datum& datum = scope[name];
		if (datum.OwnsStorage() && datum.Size() <= index)
		{
			datum.PushBack(value.asInt());
		}
		else
		{
			datum.Set(value.asInt(), index);
		}
	}

	void JsonTableParseHelper::SetFloat(Scope& scope, const std::string& name, const Json::Value& value, size_t index)
	{
		Datum& datum = scope[name];
		if (datum.OwnsStorage() && datum.Size() <= index)
		{
			datum.PushBack(value.asFloat());
		}
		else
		{
			datum.Set(value.asFloat(), index);
		}
	}

	void JsonTableParseHelper::SetFromString(Scope& scope, const std::string& name, const Json::Value& value, size_t index)
	{
		Datum& datum = scope[name];
		if (datum.OwnsStorage() && datum.Size() <= index)
		{
			datum.Resize(datum.Size() + 1);
		}

		datum.SetFromString(value.asString(), index);
	}


	// Table Shared Data
	RTTI_DEFINITIONS(TableSharedData)

	TableSharedData::TableSharedData(Scope& table) :
		mTable(&table), mOwnsTable(false)
	{
	}

	TableSharedData::~TableSharedData()
	{
		if (mOwnsTable)
		{
			delete mTable;
		}
	}

	gsl::owner<JsonParseMaster::SharedData*> TableSharedData::Create() const
	{
		TableSharedData* data = new TableSharedData(*(new Scope));
		data->mOwnsTable = true;

		return data;
	}

	Scope& TableSharedData::GetTable()
	{
		assert(mTable != nullptr);
		return *mTable;
	}


	// Stack Frame
	StackFrame::StackFrame(const std::string& name, Scope& table) :
		Name(name), Table(table)
	{
	}
}
