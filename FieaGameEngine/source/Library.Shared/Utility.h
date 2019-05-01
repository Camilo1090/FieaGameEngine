#pragma once

namespace FieaGameEngine
{
	constexpr float DefaultIncrementMultiplier = 1.4f;

	class DefaultIncrement final
	{
	public:
		size_t operator()(size_t size, size_t capacity) const;
	};
}

#include "Utility.inl"
