#include "DefaultComparator.h"

namespace FieaGameEngine
{
	template <typename T>
	inline bool DefaultComparator<T>::operator()(const T& left, const T& right) const
	{
		return left == right;
	}

	inline bool DefaultComparator<char*>::operator()(const char* left, const char* right) const
	{
		return !std::strcmp(left, right);
	}

	inline bool DefaultComparator<const char*>::operator()(const char* left, const char* right) const
	{
		return DefaultComparator<char*>()(left, right);
	}

	inline bool DefaultComparator<IJsonParseHelper*>::operator()(const RTTI* left, const RTTI* right) const
	{
		if (left == nullptr)
		{
			return right == nullptr;
		}
		else if (right == nullptr)
		{
			return false;
		}

		return left->Equals(right);
	}

	inline bool DefaultComparator<const IJsonParseHelper*>::operator()(const RTTI* left, const RTTI* right) const
	{
		return DefaultComparator<IJsonParseHelper*>()(left, right);
	}
}
