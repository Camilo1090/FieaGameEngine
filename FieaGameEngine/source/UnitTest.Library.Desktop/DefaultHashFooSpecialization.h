#pragma once

#include "Foo.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	template <>
	class DefaultHash<UnitTestLibraryDesktop::Foo> final
	{
	public:
		inline size_t DefaultHash<UnitTestLibraryDesktop::Foo>::operator()(const UnitTestLibraryDesktop::Foo& key) const
		{
			return static_cast<size_t>(key.Data());
		}
	};
}
