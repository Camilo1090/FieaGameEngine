#pragma once

#include "CppUnitTestAssert.h"
#include "Foo.h"
#include "SList.h"
#include "vector.h"
//#include "HashMap.h"
#include "Datum.h"
#include "Scope.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "Avatar.h"
#include "AttributedFoo.h"
#include "JsonParseMaster.h"

// glm
#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

namespace  Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<UnitTestLibraryDesktop::Foo>(const UnitTestLibraryDesktop::Foo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template<>
	inline std::wstring ToString<UnitTestLibraryDesktop::Foo>(UnitTestLibraryDesktop::Foo* t)
	{
		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTestLibraryDesktop::Foo>::Iterator>(const FieaGameEngine::SList<UnitTestLibraryDesktop::Foo>::Iterator& t)
	{
		try
		{
			FieaGameEngine::SList<UnitTestLibraryDesktop::Foo>::Iterator it = t;
			RETURN_WIDE_STRING((*it).Data());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("end");
		}
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTestLibraryDesktop::Foo>::ConstIterator>(const FieaGameEngine::SList<UnitTestLibraryDesktop::Foo>::ConstIterator& t)
	{
		try
		{
			RETURN_WIDE_STRING((*t).Data());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("end");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTestLibraryDesktop::Foo>::Iterator>(FieaGameEngine::SList<UnitTestLibraryDesktop::Foo>::Iterator* t)
	{
		try
		{
			RETURN_WIDE_STRING((*(*t)).Data());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("end");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTestLibraryDesktop::Foo>::Iterator>(const FieaGameEngine::Vector<UnitTestLibraryDesktop::Foo>::Iterator& t)
	{
		try
		{
			FieaGameEngine::Vector<UnitTestLibraryDesktop::Foo>::Iterator it = t;
			RETURN_WIDE_STRING((*it).Data());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("Invalid iterator");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTestLibraryDesktop::Foo>::ConstIterator>(const FieaGameEngine::Vector<UnitTestLibraryDesktop::Foo>::ConstIterator& t)
	{
		try
		{
			RETURN_WIDE_STRING((*t).Data());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("Invalid iterator");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Datum>(const FieaGameEngine::Datum& t)
	{
		try
		{
			RETURN_WIDE_STRING(t.ToString().c_str());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("empty");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Datum>(FieaGameEngine::Datum* t)
	{
		try
		{
			RETURN_WIDE_STRING(t->ToString().c_str());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("empty");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Datum::DatumType>(const FieaGameEngine::Datum::DatumType& t)
	{
		RETURN_WIDE_STRING(std::to_string(static_cast<size_t>(t)).c_str());
	}

	template <>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template <>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		RETURN_WIDE_STRING(glm::to_string(t).c_str());
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Datum::Iterator>(const FieaGameEngine::Datum::Iterator& t)
	{
		try
		{
			RETURN_WIDE_STRING(*t);
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("Invalid iterator");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Datum::ConstIterator>(const FieaGameEngine::Datum::ConstIterator& t)
	{
		try
		{
			RETURN_WIDE_STRING(*t);
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("Invalid iterator");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Scope>(const FieaGameEngine::Scope& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Scope>(FieaGameEngine::Scope* t)
	{
		if (t == nullptr)
		{
			RETURN_WIDE_STRING("nullptr");
		}

		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template <>
	inline std::wstring ToString<UnitTestLibraryDesktop::AttributedFoo>(const UnitTestLibraryDesktop::AttributedFoo& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template <>
	inline std::wstring ToString<UnitTestLibraryDesktop::AttributedFoo>(UnitTestLibraryDesktop::AttributedFoo* t)
	{
		if (t == nullptr)
		{
			RETURN_WIDE_STRING("nullptr");
		}

		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::JsonParseMaster>(FieaGameEngine::JsonParseMaster* t)
	{
		if (t == nullptr)
		{
			RETURN_WIDE_STRING("nullptr");
		}

		RETURN_WIDE_STRING("JsonParseMaster");
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Entity>(const FieaGameEngine::Entity& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Entity>(FieaGameEngine::Entity* t)
	{
		if (t == nullptr)
		{
			RETURN_WIDE_STRING("nullptr");
		}

		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Sector>(const FieaGameEngine::Sector& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::Sector>(FieaGameEngine::Sector* t)
	{
		if (t == nullptr)
		{
			RETURN_WIDE_STRING("nullptr");
		}

		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::World>(const FieaGameEngine::World& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::World>(FieaGameEngine::World* t)
	{
		if (t == nullptr)
		{
			RETURN_WIDE_STRING("nullptr");
		}

		RETURN_WIDE_STRING(t->ToString().c_str());
	}

	template <>
	inline std::wstring ToString<UnitTestLibraryDesktop::Avatar>(const UnitTestLibraryDesktop::Avatar& t)
	{
		RETURN_WIDE_STRING(t.ToString().c_str());
	}

	template <>
	inline std::wstring ToString<UnitTestLibraryDesktop::Avatar>(UnitTestLibraryDesktop::Avatar* t)
	{
		if (t == nullptr)
		{
			RETURN_WIDE_STRING("nullptr");
		}

		RETURN_WIDE_STRING(t->ToString().c_str());
	}
}
