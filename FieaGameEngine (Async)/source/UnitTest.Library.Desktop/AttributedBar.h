#pragma once

#include "Attributed.h"

namespace UnitTestLibraryDesktop
{
	class AttributedBar : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedBar, Attributed)

	public:
		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	public:
		AttributedBar() : Attributed(TypeIdClass()) {};
		virtual ~AttributedBar() = default;
		virtual gsl::owner<AttributedBar*> Clone() const override
		{
			return new AttributedBar(*this);
		}


		std::int32_t Integer = 0;
	};
}
