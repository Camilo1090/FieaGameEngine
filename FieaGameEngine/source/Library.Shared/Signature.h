#pragma once

#include "Datum.h"
#include <string>

namespace FieaGameEngine
{
	struct Signature final
	{
	public:
		const std::string Name;
		const Datum::DatumType Type;
		const size_t Size;
		const size_t Offset;

		Signature(const std::string& name, Datum::DatumType type, size_t size, size_t offset);
		Signature(const Signature&) = default;
		Signature(Signature&&) = default;
		Signature& operator=(const Signature&) = default;
		Signature& operator=(Signature&&) = default;
		~Signature() = default;
	};
}
