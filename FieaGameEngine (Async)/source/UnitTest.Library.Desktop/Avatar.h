#pragma once

#include "Entity.h"
#include "Factory.h"

namespace UnitTestLibraryDesktop
{
	class Avatar : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(Avatar, FieaGameEngine::Entity)

	public:
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	public:
		Avatar();
		Avatar(const std::string& name, const glm::vec4& position);
		Avatar(const Avatar&) = default;
		Avatar(Avatar&&) = default;
		Avatar& operator=(const Avatar&) = default;
		Avatar& operator=(Avatar&&) = default;
		~Avatar() = default;

		virtual gsl::owner<Avatar*> Clone() const override;

		glm::vec4 Position;
	};

	CONCRETE_FACTORY(Avatar, FieaGameEngine::Scope)
}
