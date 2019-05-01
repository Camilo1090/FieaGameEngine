#include "pch.h"
#include <cstddef>
#include "JsonIntHelper.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(JsonIntHelper)

	gsl::owner<IJsonParseHelper*> JsonIntHelper::Create() const
	{
		return new JsonIntHelper();
	}

	bool JsonIntHelper::StartHandler(JsonParseMaster::SharedData& data, const std::string& /*name*/, const Json::Value& value, size_t /*index*/)
	{
		IntSharedData* intData = data.As<IntSharedData>();
		if (intData == nullptr || !value.isIntegral())
		{
			return false;
		}

		intData->Data = value.asInt();

		return true;
	}

	bool JsonIntHelper::EndHandler(FieaGameEngine::JsonParseMaster::SharedData& data, const std::string& /*name*/)
	{
		if (data.TypeIdInstance() != IntSharedData::TypeIdClass())
		{
			return false;
		}

		return true;
	}

	RTTI_DEFINITIONS(JsonIntHelper::IntSharedData)

	gsl::owner<JsonParseMaster::SharedData*> JsonIntHelper::IntSharedData::Create() const
	{
		return new IntSharedData();
	}
}
