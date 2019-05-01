#include "pch.h"
#include "IJsonParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(IJsonParseHelper)
	
	bool IJsonParseHelper::operator==(const IJsonParseHelper& other) const
	{
		return TypeIdInstance() == other.TypeIdInstance();
	}

	bool IJsonParseHelper::operator!=(const IJsonParseHelper& other) const
	{
		return !operator==(other);
	}

	void IJsonParseHelper::Initialize()
	{
	}

	bool IJsonParseHelper::Equals(const RTTI* other) const
	{
		if (other == nullptr)
		{
			return false;
		}

		IJsonParseHelper* helper = other->As<IJsonParseHelper>();
		return helper == nullptr ? false : operator==(*helper);
	}

	std::string IJsonParseHelper::ToString() const
	{
		return TypeName();
	}
}
