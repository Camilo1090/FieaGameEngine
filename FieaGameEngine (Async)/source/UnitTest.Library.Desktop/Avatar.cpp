#include "pch.h"
#include "Avatar.h"

using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(Avatar)

	Vector<Signature> Avatar::Signatures()
	{
		return {
			{ "Position", Datum::DatumType::VECTOR, 1, offsetof(Avatar, Position) }
		};
	}

	Avatar::Avatar() : Entity(TypeIdClass())
	{
	}

	Avatar::Avatar(const std::string& name, const glm::vec4& position) : Entity(TypeIdClass(), name), Position(position)
	{
	}

	gsl::owner<Avatar*> Avatar::Clone() const
	{
		return new Avatar(*this);
	}
}
