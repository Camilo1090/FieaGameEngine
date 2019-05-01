#pragma once

#include "RTTI.h"
#include "Factory.h"

namespace UnitTestLibraryDesktop
{
	/// <summary>
	/// Dummy class used for testing porposes only
	/// </summary>
	class Foo : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, FieaGameEngine::RTTI)

	public:
		explicit Foo(int data = 0);
		Foo(const Foo& other);
		Foo(Foo&& other);
		~Foo();
		Foo& operator=(const Foo& other);
		Foo& operator=(Foo&& other);
		bool operator==(const Foo& other) const;

		int& Data() const;
		void Data(const int data);

		std::string ToString() const override;
		bool Equals(const RTTI* other) const override;

	private:
		int* mData;
	};

	CONCRETE_FACTORY(Foo, FieaGameEngine::RTTI)
}

