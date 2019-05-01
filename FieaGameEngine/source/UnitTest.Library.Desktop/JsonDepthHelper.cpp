#include "pch.h"
#include <cstddef>
#include "JsonDepthHelper.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(JsonDepthHelper)

	gsl::owner<IJsonParseHelper*> JsonDepthHelper::Create() const
	{
		return new JsonDepthHelper();
	}

	void JsonDepthHelper::Initialize()
	{
		InitializeCalled = true;
		StartHandlerCount = 0;
		EndHandlerCount = 0;
	}

	bool JsonDepthHelper::StartHandler(JsonParseMaster::SharedData& data, const std::string& /*name*/, const Json::Value& /*value*/, size_t /*index*/)
	{
		DepthSharedData* depthData = data.As<DepthSharedData>();
		if (depthData == nullptr)
		{
			return false;
		}

		++StartHandlerCount;

		if (depthData->Depth() > depthData->MaxDepth)
		{
			depthData->MaxDepth = depthData->Depth();
		}

		return true;
	}

	bool JsonDepthHelper::EndHandler(JsonParseMaster::SharedData& data, const std::string& /*name*/)
	{
		if (data.TypeIdInstance() != DepthSharedData::TypeIdClass())
		{
			return false;
		}

		++EndHandlerCount;

		return true;
	}

	RTTI_DEFINITIONS(JsonDepthHelper::DepthSharedData)

	gsl::owner<JsonParseMaster::SharedData*> JsonDepthHelper::DepthSharedData::Create() const
	{
		return new DepthSharedData();
	}
}
