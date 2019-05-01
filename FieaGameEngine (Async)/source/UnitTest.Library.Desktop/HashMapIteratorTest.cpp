#include "pch.h"
#include "CppUnitTest.h"
#include "HashMap.h"
#include "Foo.h"
#include "DefaultHashFooSpecialization.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapIteratorTest)
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

		TEST_METHOD(Test)
		{
			const int count = 10;
			HashMap<Foo, Foo> map(count);
			for (int i = 0; i < count; ++i)
			{
				map[Foo(i)] = Foo(i);
			}

			const auto map2 = map;
			int i = 0;
			for (auto& pair : map2)
			{
				Assert::AreEqual(pair.first, Foo(i));
				++i;
			}
		}

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState HashMapIteratorTest::sStartMemState;
}