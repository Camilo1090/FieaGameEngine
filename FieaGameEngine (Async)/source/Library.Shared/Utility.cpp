#include "pch.h"
#include <cstddef>
#include "Utility.h"

namespace FieaGameEngine
{
	size_t FieaGameEngine::DefaultIncrement::operator()(size_t /*size*/, size_t capacity) const
	{
		return static_cast<size_t>((capacity * DefaultIncrementMultiplier) + 1);
	}
}
