#include "pch.h"
#include "IJsonParseHelper.h"
#include "JsonParseMaster.h"
#include <sstream>
#include <fstream>
#include <json/json.h>

namespace FieaGameEngine
{
	JsonParseMaster::JsonParseMaster(SharedData& data, size_t helperCount) :
		mHelpers(helperCount), mData(&data), mIsClone(false)
	{
		mData->SetJsonParseMaster(*this);
	}

	JsonParseMaster::JsonParseMaster(JsonParseMaster&& other) :
		mHelpers(std::move(other.mHelpers)), mData(other.mData), mIsClone(other.mIsClone), mFileName(std::move(other.mFileName))
	{
		mData->SetJsonParseMaster(*this);

		other.mData = nullptr;
		other.mIsClone = false;
	}

	JsonParseMaster& JsonParseMaster::operator=(JsonParseMaster&& other)
	{
		if (this != &other)
		{
			Clear();

			mHelpers = std::move(other.mHelpers);
			mData = other.mData;
			mIsClone = other.mIsClone;
			mFileName = std::move(other.mFileName);

			mData->SetJsonParseMaster(*this);

			other.mData = nullptr;
			other.mIsClone = false;
		}

		return *this;
	}

	JsonParseMaster::~JsonParseMaster()
	{
		Clear();
	}

	gsl::owner<JsonParseMaster*> JsonParseMaster::Clone() const
	{
		JsonParseMaster* clone = new JsonParseMaster(*mData->Create());
		clone->mHelpers.Reserve(mHelpers.Size());
		for (auto helper : mHelpers)
		{
			clone->mHelpers.PushBack(helper->Create());
		}
		clone->mIsClone = true;

		return clone;
	}

	void JsonParseMaster::Initialize()
	{
		mData->Initialize();

		for (auto helper : mHelpers)
		{
			helper->Initialize();
		}

		mFileName.clear();
	}

	bool JsonParseMaster::AddHelper(IJsonParseHelper& helper)
	{
		if (mIsClone)
		{
			throw std::exception("Cannot add helpers to a cloned master.");
		}

		auto[it, inserted] = mHelpers.PushBackUnique(&helper);

		return inserted;
	}

	bool JsonParseMaster::RemoveHelper(IJsonParseHelper& helper)
	{
		bool removed = false;
		if (mIsClone)
		{
			Vector<IJsonParseHelper*>::Iterator it = mHelpers.Find(&helper);
			if (it != mHelpers.end())
			{
				delete *it;
				removed = mHelpers.Remove(it);
			}
		}
		else
		{
			removed = mHelpers.Remove(&helper);
		}

		return removed;
	}

	void JsonParseMaster::Parse(const std::string& string)
	{
		std::stringstream sstream;
		sstream << string;

		Parse(sstream);
	}

	void JsonParseMaster::ParseFromFile(const std::string& filename)
	{
		std::ifstream fileStream(filename);
		if (!fileStream.good())
		{
			throw std::exception("Error opening file.");
		}

		mFileName = filename;
		Parse(fileStream);
		fileStream.close();
	}

	void JsonParseMaster::Parse(std::istream& stream)
	{
		Json::Value root;
		stream >> root;

		if (!root.isObject())
		{
			throw std::exception("JSON root must be an object value.");
		}

		ParseMembers(root);
	}

	void JsonParseMaster::ParseMembers(const Json::Value& value, size_t index)
	{
		if (value.size() > 0)
		{
			const auto& members = value.getMemberNames();
			for (const auto& name : members)
			{
				Parse(name, value[name], index);
			}
		}
	}

	void JsonParseMaster::Parse(const std::string& name, const Json::Value& value, size_t index)
	{
		if (!value.isArray())
		{
			IJsonParseHelper* chosenHelper = nullptr;

			for (auto helper : mHelpers)
			{
				if (helper->StartHandler(*mData, name, value, index))
				{
					chosenHelper = helper;
					break;
				}
			}

			if (chosenHelper == nullptr)
			{
				throw std::exception(std::string("Could not parse value of key: ").append(name).c_str());
			}

			if (value.isObject())
			{
				mData->IncrementDepth();
				ParseMembers(value);
				mData->DecrementDepth();
			}

			if (!chosenHelper->EndHandler(*mData, name))
			{
				throw std::exception("Helper start end mismatch.");
			}
		}
		else
		{
			for (Json::ArrayIndex i = 0; i < value.size(); ++i)
			{
				const Json::Value& arrayValue = value[Json::ArrayIndex(i)];
				if (arrayValue.isObject())
				{
					mData->IncrementDepth();
					ParseMembers(arrayValue, i);
					mData->DecrementDepth();
				}
				else
				{
					Parse(name, arrayValue, i);
				}
			}
		}
	}

	void JsonParseMaster::Clear()
	{
		if (mIsClone)
		{
			delete mData;
			for (auto helper : mHelpers)
			{
				delete helper;
			}
		}
	}

	size_t JsonParseMaster::GetHelperCount() const
	{
		return mHelpers.Size();
	}

	const std::string& JsonParseMaster::GetFileName() const
	{
		return mFileName;
	}

	JsonParseMaster::SharedData* JsonParseMaster::GetSharedData()
	{
		return mData;
	}

	const JsonParseMaster::SharedData* JsonParseMaster::GetSharedData() const
	{
		return mData;
	}

	void JsonParseMaster::SetSharedData(SharedData& data)
	{
		if (mIsClone)
		{
			throw std::exception("Cannot set SharedData of a cloned master.");
		}

		mData = &data;
	}

	bool JsonParseMaster::IsClone() const
	{
		return mIsClone;
	}

	// SharedData
	RTTI_DEFINITIONS(JsonParseMaster::SharedData)

	void JsonParseMaster::SharedData::SetJsonParseMaster(JsonParseMaster& parser)
	{
		mOwner = &parser;
	}

	void JsonParseMaster::SharedData::Initialize()
	{
	}

	JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster()
	{
		return mOwner;
	}

	const JsonParseMaster* JsonParseMaster::SharedData::GetJsonParseMaster() const
	{
		return mOwner;
	}

	void JsonParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}

	void JsonParseMaster::SharedData::DecrementDepth()
	{
		if (mDepth > 0)
		{
			--mDepth;
		}
	}

	size_t JsonParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}
}
