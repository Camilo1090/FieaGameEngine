#include "Scope.h"

namespace FieaGameEngine
{
	template <typename T>
	T* Scope::GetParent()
	{
		if (mParent != nullptr)
		{
			assert(mParent->Is(T::TypeIdClass()));
			return static_cast<T*>(mParent);
		}

		return nullptr;
	}

	template <typename T>
	const T* Scope::GetParent() const
	{
		if (mParent != nullptr)
		{
			assert(mParent->Is(T::TypeIdClass()));
			return static_cast<T*>(mParent);
		}

		return nullptr;
	}
}
