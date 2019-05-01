#pragma once

#include "RTTI.h"
#include <string>

namespace FieaGameEngine
{
	class IJsonParseHelper;

	template <typename T>
	class DefaultComparator final
	{
	public:
		bool operator()(const T& left, const T& right) const;
	};

	template <>
	class DefaultComparator<char*> final
	{
	public:
		bool operator()(const char* left, const char* right) const;
	};

	template <>
	class DefaultComparator<const char*> final
	{
	public:
		bool operator()(const char* left, const char* right) const;
	};

	template <>
	class DefaultComparator<IJsonParseHelper*> final
	{
	public:
		bool operator()(const RTTI* left, const RTTI* right) const;
	};

	template <>
	class DefaultComparator<const IJsonParseHelper*> final
	{
	public:
		bool operator()(const RTTI* left, const RTTI* right) const;
	};
}

#include "DefaultComparator.inl"
