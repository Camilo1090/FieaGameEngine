#include "pch.h"
#include <cstddef>
#include "vector.h"
#include "Signature.h"
#include "AttributedBar.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(AttributedBar)

	const Vector<Signature> AttributedBar::Signatures()
	{
		return Vector<Signature>(
			{
				{ "Integer", Datum::DatumType::INTEGER, 1, offsetof(AttributedBar, Integer) },
			});
	}
}
