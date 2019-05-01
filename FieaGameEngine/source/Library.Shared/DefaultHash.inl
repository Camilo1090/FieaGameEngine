#include "DefaultHash.h"

namespace FieaGameEngine
{
	template <typename TKey>
	size_t DefaultHash<TKey>::operator()(const TKey& key) const
	{
		/*
		* Hash algorithm taken from: http://www.partow.net
		* Author: Arash Partow
		*/

		std::uint32_t hash = 0xAAAAAAAA;
		std::uint32_t i = 0;
		size_t length = sizeof(TKey);
		const std::uint8_t* bytes = reinterpret_cast<const std::uint8_t*>(&key);

		for (; i < length; ++bytes, ++i)
		{
			hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*bytes) * (hash >> 3)) :
				(~((hash << 11) + ((*bytes) ^ (hash >> 5))));
		}

		return static_cast<size_t>(hash);
	}

	inline size_t DefaultHash<char*>::operator()(const char* key) const
	{
		std::uint32_t hash = 5381;
		std::uint32_t i = 0;
		size_t length = std::strlen(key);

		for (; i < length; ++key, ++i)
		{
			hash = ((hash << 5) + hash) + (*key);
		}

		return static_cast<size_t>(hash);
	}

	inline size_t DefaultHash<const char*>::operator()(const char* key) const
	{
		return DefaultHash<char*>()(key);
	}

	inline size_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		return DefaultHash<char*>()(key.c_str());
	}

	inline size_t DefaultHash<const std::string>::operator()(const std::string& key) const
	{
		return DefaultHash<char*>()(key.c_str());
	}

	inline size_t DefaultHash<int>::operator()(const int& key) const
	{
		size_t hash = key;
		return hash;
	}

	inline size_t DefaultHash<const int>::operator()(const int& key) const
	{
		return DefaultHash<int>()(key);
	}
}
