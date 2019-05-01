#include "pch.h"
#include <string>
#include "Foo.h"

using namespace UnitTestLibraryDesktop;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

RTTI_DEFINITIONS(Foo)

Foo::Foo(int data) :
	mData(new int(data))
{
}

Foo::Foo(const Foo& other) :
	mData(new int(*(other.mData)))
{
}

Foo::Foo(Foo&& other) :
	mData(other.mData)
{
	other.mData = nullptr;
}

Foo::~Foo()
{
	delete mData;
}

Foo& Foo::operator=(const Foo& other)
{
	if (this != &other)
	{
		*mData = *(other.mData);
	}

	return *this;
}

Foo& Foo::operator=(Foo&& other)
{
	*mData = *(other.mData);

	return *this;
}

bool Foo::operator==(const Foo& other) const
{
	return *mData == other.Data();
}

int& Foo::Data() const
{
	return *mData;
}

void Foo::Data(const int data)
{
	*mData = data;
}

std::string UnitTestLibraryDesktop::Foo::ToString() const
{
	return "Foo("s + std::to_string(*mData) + ")"s;
}

bool UnitTestLibraryDesktop::Foo::Equals(const RTTI* other) const
{
	if (other == nullptr)
	{
		return false;
	}

	Foo* foo = other->As<Foo>();

	return foo == nullptr ? false : operator==(*foo);
}
