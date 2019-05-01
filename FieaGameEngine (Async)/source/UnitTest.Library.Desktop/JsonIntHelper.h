#pragma once

#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"

namespace UnitTestLibraryDesktop
{
	class JsonIntHelper : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntHelper, IJsonParseHelper)

	public:
		class IntSharedData : public FieaGameEngine::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(IntSharedData, SharedData)

		public:
			virtual gsl::owner<SharedData*> Create() const override;

			int Data = 0;
		};

	public:
		JsonIntHelper() = default;
		~JsonIntHelper() = default;

		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual bool StartHandler(FieaGameEngine::JsonParseMaster::SharedData& data, const std::string& name, const Json::Value& value, size_t index = 0) override;
		virtual bool EndHandler(FieaGameEngine::JsonParseMaster::SharedData& data, const std::string& name) override;
	};
}

