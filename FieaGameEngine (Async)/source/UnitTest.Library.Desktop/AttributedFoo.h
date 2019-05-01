#pragma once

#include "Attributed.h"

namespace UnitTestLibraryDesktop
{
	class AttributedFoo : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed)

	private:
		static const size_t sArraySize = 5;

	public:
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	public:
		AttributedFoo();
		AttributedFoo(const AttributedFoo&) = default;
		AttributedFoo(AttributedFoo&&) = default;
		AttributedFoo& operator=(const AttributedFoo&) = default;
		AttributedFoo& operator=(AttributedFoo&&) = default;
		virtual ~AttributedFoo() = default;

		virtual gsl::owner<AttributedFoo*> Clone() const override;

		virtual bool Equals(const RTTI* other) const override;
		virtual std::string ToString() const override;

		std::int32_t ExternalInteger = 0;
		float ExternalFloat = 0.0f;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;
		std::string ExternalString;
		//Foo ExternalFoo;

		std::int32_t ExternalIntegerArray[sArraySize];
		float ExternalFloatArray[sArraySize];
		glm::vec4 ExternalVectorArray[sArraySize];
		glm::mat4 ExternalMatrixArray[sArraySize];
		std::string ExternalStringArray[sArraySize];
		//Foo ExternalFooArray[sArraySize];
	};
}
