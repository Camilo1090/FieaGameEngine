#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace FieaGameEngine
{
	template <typename TKey>
	class DefaultHash final
	{
	public:
		size_t operator()(const TKey& key) const;
	};

	template <>
	class DefaultHash<char*> final
	{
	public:
		size_t operator()(const char* key) const;
	};

	template <>
	class DefaultHash<const char*> final
	{
	public:
		size_t operator()(const char* key) const;
	};

	template <>
	class DefaultHash<std::string> final
	{
	public:
		size_t operator()(const std::string& key) const;
	};

	template <>
	class DefaultHash<const std::string> final
	{
	public:
		size_t operator()(const std::string& key) const;
	};

	template <>
	class DefaultHash<int> final
	{
	public:
		size_t operator()(const int& key) const;
	};

	template <>
	class DefaultHash<const int> final
	{
	public:
		size_t operator()(const int& key) const;
	};
}

#include "DefaultHash.inl"
