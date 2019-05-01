#include "pch.h"
#include "Signature.h"

namespace FieaGameEngine
{
	Signature::Signature(const std::string& name, Datum::DatumType type, size_t size, size_t offset) :
		Name(name), Type(type), Size(size), Offset(offset)
	{
	}
}
