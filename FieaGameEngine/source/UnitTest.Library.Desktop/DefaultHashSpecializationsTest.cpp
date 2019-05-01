#include "pch.h"
#include "CppUnitTest.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "DefaultHashFooSpecialization.h"
#include "ToStringSpecializations.h"
#include "Foo.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DefaultHashTest)
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

		TEST_METHOD(TestDefault) 
		{
			HashMap<float, Foo> map;
			float f1 = 1.0f;
			float f2 = 2.0f;
			float f3 = 1.0f;
			DefaultHash<float> hash;
			Assert::AreEqual(hash(f1), hash(f3));
			map[f1] = Foo(1);
			Assert::AreEqual(map[f1], Foo(1));
			map[f2] = Foo(2);
			Assert::AreEqual(map[f2], Foo(2));
			map[f3] = Foo(3);
			Assert::AreEqual(map[f1], Foo(3));
			Assert::AreEqual(map[f3], Foo(3));
		}

		TEST_METHOD(TestChar)
		{
			HashMap<const char*, Foo> map;
			char* name1 = "Camilo";
			char* name2 = "Erika";
			char name3[7];
			strncpy_s(name3, 7, name1, std::strlen(name1) + 1);
			DefaultHash<const char*> hash;
			Assert::AreEqual(hash(name1), hash(name3));
			map[name1] = Foo(1);
			Assert::AreEqual(map[name1], Foo(1));
			map[name2] = Foo(2);
			Assert::AreEqual(map[name2], Foo(2));
			map[name3] = Foo(3);
			Assert::AreEqual(map[name1], Foo(3));
			Assert::AreEqual(map[name3], Foo(3));
		}

		TEST_METHOD(TestString)
		{
			HashMap<const std::string, Foo> map;
			char* name1 = "Camilo";
			char* name2 = "Erika";
			char name3[7];
			strncpy_s(name3, 7, name1, std::strlen(name1) + 1);
			DefaultHash < const std::string > hash;
			Assert::AreEqual(hash(name1), hash(name3));
			map[name1] = Foo(1);
			Assert::AreEqual(map[name1], Foo(1));
			map[name2] = Foo(2);
			Assert::AreEqual(map[name2], Foo(2));
			map[name3] = Foo(3);
			Assert::AreEqual(map[name1], Foo(3));
			Assert::AreEqual(map[name3], Foo(3));
		}

		TEST_METHOD(TestInt)
		{
			HashMap<int, Foo> map;
			int one = 1;
			int two = 2;
			int otherOne = 1;
			DefaultHash<const int> hash;
			Assert::AreEqual(hash(one), hash(otherOne));
			map[one] = Foo(1);
			Assert::AreEqual(map[one], Foo(1));
			map[two] = Foo(2);
			Assert::AreEqual(map[two], Foo(2));
			map[otherOne] = Foo(3);
			Assert::AreEqual(map[one], Foo(3));
			Assert::AreEqual(map[otherOne], Foo(3));
		}

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState DefaultHashTest::sStartMemState;
}