#include "pch.h"
#include <cstddef>
#include "vector.h"
#include "Signature.h"
#include "AttributedFoo.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(AttributedFoo)

	AttributedFoo::AttributedFoo() : 
		Attributed(TypeIdClass())
	{
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	bool AttributedFoo::Equals(const RTTI* other) const
	{
		if (other == nullptr)
		{
			return false;
		}

		AttributedFoo* foo = other->As<AttributedFoo>();
		return foo == nullptr ? false : operator==(*foo);
	}

	std::string AttributedFoo::ToString() const
	{
		return std::string("AttributedFoo");
	}

	Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>(
			{
				{ "ExternalInteger", Datum::DatumType::INTEGER, 1, offsetof(AttributedFoo, ExternalInteger)},
				{ "ExternalFloat", Datum::DatumType::FLOAT, 1, offsetof(AttributedFoo, ExternalFloat)},
				{ "ExternalVector", Datum::DatumType::VECTOR, 1, offsetof(AttributedFoo, ExternalVector)},
				{ "ExternalMatrix", Datum::DatumType::MATRIX, 1, offsetof(AttributedFoo, ExternalMatrix)},
				{ "ExternalString", Datum::DatumType::STRING, 1, offsetof(AttributedFoo, ExternalString)},
				{ "ExternalIntegerArray", Datum::DatumType::INTEGER, sArraySize, offsetof(AttributedFoo, ExternalIntegerArray)},
				{ "ExternalFloatArray", Datum::DatumType::FLOAT, sArraySize, offsetof(AttributedFoo, ExternalFloatArray)},
				{ "ExternalVectorArray", Datum::DatumType::VECTOR, sArraySize, offsetof(AttributedFoo, ExternalVectorArray)},
				{ "ExternalMatrixArray", Datum::DatumType::MATRIX, sArraySize, offsetof(AttributedFoo, ExternalMatrixArray)},
				{ "ExternalStringArray", Datum::DatumType::STRING, sArraySize, offsetof(AttributedFoo, ExternalStringArray)},
				{ "EmptyTableDatum", Datum::DatumType::TABLE, 0, 0},
				{ "NestedScope", Datum::DatumType::TABLE, 1, 0},
				{ "NestedScopes", Datum::DatumType::TABLE, sArraySize, 0},
			});
	}
}
