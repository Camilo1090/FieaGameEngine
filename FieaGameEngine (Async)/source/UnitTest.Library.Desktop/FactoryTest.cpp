#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Utility.h"
#include <string>
#include <tuple>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(TestFind)
		{
			Assert::IsNull(Factory<RTTI>::Find("Foo"s));
			Assert::AreEqual(Factory<RTTI>::Size(), 0_z);
			Assert::IsTrue(Factory<RTTI>::IsEmpty());
			FooFactory ff;
			Assert::IsNotNull(Factory<RTTI>::Find("Foo"s));
			Assert::AreEqual(Factory<RTTI>::Size(), 1_z);
			Assert::IsFalse(Factory<RTTI>::IsEmpty());
		}

		TEST_METHOD(TestCreate)
		{
			RTTI* ptr = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(ptr);
			FooFactory ff;
			ptr = Factory<RTTI>::Create("Foo"s);
			Assert::IsNotNull(ptr);
			Assert::IsTrue(ptr->Is("Foo"s));
			delete ptr;
		}

		TEST_METHOD(TestAdd)
		{
			Assert::IsNull(Factory<RTTI>::Find("Foo"s));
			FooFactory ff;
			Assert::IsNotNull(Factory<RTTI>::Find("Foo"s));

			Assert::ExpectException<std::exception>([] { FooFactory dup; });
		}

		TEST_METHOD(TestRemove)
		{
			Assert::IsNull(Factory<RTTI>::Find("Foo"s));
			FooFactory* ff = new FooFactory();
			Assert::IsNotNull(Factory<RTTI>::Find("Foo"s));
			delete ff;
			Assert::IsNull(Factory<RTTI>::Find("Foo"s));
		}

		TEST_METHOD(TestClassName)
		{
			FooFactory ff;
			Assert::AreEqual(ff.ClassName(), "Foo"s);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState FactoryTest::sStartMemState;
}
