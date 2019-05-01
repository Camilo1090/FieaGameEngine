#pragma once

#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"

namespace UnitTestLibraryDesktop
{
	class JsonDepthHelper : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonDepthHelper, IJsonParseHelper)

	public:
		class DepthSharedData : public FieaGameEngine::JsonParseMaster::SharedData
		{
			RTTI_DECLARATIONS(DepthSharedData, SharedData)

		public:
			virtual gsl::owner<SharedData*> Create() const override;

			size_t MaxDepth = 0;
		};

	public:
		JsonDepthHelper() = default;
		~JsonDepthHelper() = default;

		virtual gsl::owner<IJsonParseHelper*> Create() const override;
		virtual void Initialize() override;
		virtual bool StartHandler(FieaGameEngine::JsonParseMaster::SharedData& data, const std::string& name, const Json::Value& value, size_t index = 0) override;
		virtual bool EndHandler(FieaGameEngine::JsonParseMaster::SharedData& data, const std::string& name) override;

		bool InitializeCalled = false;
		size_t StartHandlerCount = 0;
		size_t EndHandlerCount = 0;
	};
}
