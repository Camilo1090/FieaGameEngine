#include "pch.h"
#include "CppUnitTest.h"
#include "DefaultHashFooSpecialization.h"
#include "HashMap.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashMapTest)
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

		TEST_METHOD(TestConstructor)
		{
			HashMap<std::string, Foo> map;
			Assert::AreEqual(map.Size(), 0_z);
			Assert::IsTrue(map.BucketCount() >= map.Size());

			HashMap<std::string, Foo> map2(100_z);
			Assert::AreEqual(map2.Size(), 0_z);
			Assert::AreEqual(map2.BucketCount(), 100_z);
		}

		TEST_METHOD(TestMoveConstructor)
		{
			HashMap<std::string, Foo> map = { { "1", Foo(1) }, { "2", Foo(2) }, { "3", Foo(3) } };
			Assert::AreEqual(map.Size(), 3_z);
			HashMap<std::string, Foo> map2(std::move(map));
			Assert::AreEqual(map.Size(), 0_z);
			Assert::AreEqual(map2.Size(), 3_z);
		}

		TEST_METHOD(TestInitializerListConstructor)
		{
			HashMap<std::string, Foo> map = { { "1", Foo(1) }, { "2", Foo(2) }, { "3", Foo(3) } };
			Assert::IsTrue(map.ContainsKey("1"));
			Assert::IsTrue(map.ContainsKey("2"));
			Assert::IsTrue(map.ContainsKey("3"));
			Assert::IsFalse(map.ContainsKey("4"));
		}

		TEST_METHOD(TestMoveAssignment)
		{
			HashMap<std::string, Foo> map1 = { { "1", Foo(1) }, { "2", Foo(2) }, { "3", Foo(3) } };
			Assert::AreEqual(map1.Size(), 3_z);
			HashMap<std::string, Foo> map2 = { { "55", Foo(33) } };
			Assert::AreEqual(map2.Size(), 1_z);
			map2 = std::move(map1);
			Assert::AreEqual(map1.Size(), 0_z);
			Assert::AreEqual(map2.Size(), 3_z);
		}

		TEST_METHOD(TestInitializerListAssignmentOperator)
		{
			HashMap<std::string, Foo> map1 = { { "1", Foo(1) }, { "2", Foo(2) }, { "3", Foo(3) } };
			HashMap<std::string, Foo> map2 = { { "55", Foo(55) } };
			Assert::AreEqual(map2.Size(), 1_z);
			map2 = { { "1", Foo(1) }, { "2", Foo(2) }, { "3", Foo(3) } };
			Assert::AreEqual(map2.Size(), 3_z);
			Assert::AreEqual(map1["1"], map2["1"]);
			map2["1"] = Foo(100);
			Assert::AreNotEqual(map1["1"], map2["1"]);
		}

		TEST_METHOD(TestIndexOperator)
		{
			Foo foo1(1);
			HashMap<Foo, Foo> map = { { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) } };
			Assert::AreEqual(map[foo1], Foo(1));
			map[Foo(1)] = Foo(55);
			Assert::AreEqual(map[Foo(1)], Foo(55));
			map[Foo(4)] = Foo(4);
			Assert::AreEqual(map[Foo(4)], Foo(4));
			map[Foo(5)];
			Assert::AreEqual(map[Foo(5)], Foo());
		}

		TEST_METHOD(TestAt)
		{
			HashMap<Foo, Foo> map = { { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) } };
			Assert::AreEqual(map.At(Foo(1)), Foo(1));
			map.At(Foo(1)) = Foo(55);
			Assert::AreEqual(map.At(Foo(1)), Foo(55));
			const auto expression = [&map] { map.At(Foo(4)); };
			Assert::ExpectException<std::exception>(expression);

			const auto constMap = map;
			Assert::AreEqual(constMap.At(Foo(1)), Foo(55));
			const auto exp = [&constMap] { constMap.At(Foo(4)); };
			Assert::ExpectException<std::exception>(exp);
		}

		TEST_METHOD(TestInsert)
		{
			HashMap<Foo, Foo> map;
			const int foos = 10;
			for (int i = 0; i < foos; ++i)
			{
				map.Insert(std::pair(Foo(i), Foo(i)));
				Assert::AreEqual(map[Foo(i)], Foo(i));
			}
			Assert::AreEqual(map.Size(), static_cast<size_t>(foos));

			for (int i = foos; i < foos * 2; ++i)
			{
				std::pair<Foo, Foo> pair = { Foo(i), Foo(i) };
				map.Insert(pair);
				Assert::AreEqual(map[Foo(i)], Foo(i));
			}
			Assert::AreEqual(map.Size(), static_cast<size_t>(foos) * 2);
		}

		TEST_METHOD(TestFind)
		{
			HashMap<Foo, Foo> map = { { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) } };
			Assert::AreEqual(map.Find(Foo(1))->second, Foo(1));
			Assert::AreEqual(map.Find(Foo(2))->second, Foo(2));
			Assert::AreEqual(map.Find(Foo(3))->second, Foo(3));
			Assert::AreEqual(map.Find(Foo(4)), map.end());

			const HashMap<Foo, Foo> constMap = map;
			Assert::AreEqual(constMap.Find(Foo(1))->second, Foo(1));
			Assert::AreEqual(constMap.Find(Foo(2))->second, Foo(2));
			Assert::AreEqual(constMap.Find(Foo(3))->second, Foo(3));
			Assert::AreEqual(constMap.Find(Foo(4)), constMap.end());
		}

		TEST_METHOD(TestRemove)
		{
			HashMap<Foo, Foo> map = { { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) } };
			Assert::AreEqual(map.Size(), 3_z);
			Assert::IsTrue(map.Remove(Foo(2)));
			Assert::IsFalse(map.Remove(Foo(2)));
			Assert::AreEqual(map.Size(), 2_z);
			Assert::IsTrue(map.Remove(Foo(3)));
			Assert::IsFalse(map.Remove(Foo(3)));
			Assert::AreEqual(map.Size(), 1_z);
			Assert::IsTrue(map.Remove(Foo(1)));
			Assert::IsFalse(map.Remove(Foo(1)));
			Assert::AreEqual(map.Size(), 0_z);

			map = { { Foo(0), Foo(0) }, { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) }, { Foo(4), Foo(4) }, { Foo(5), Foo(5) } };
			Assert::AreEqual(map.Size(), 6_z);
			map.Remove(map.Find(Foo(3)), map.end());
			Assert::AreEqual(map.Size(), 3_z);
			map.Remove(map.begin(), map.Find(Foo(2)));
			Assert::AreEqual(map.Size(), 1_z);
			map.Remove(map.begin(), map.end());
			Assert::AreEqual(map.Size(), 0_z);

			map = { { Foo(0), Foo(0) }, { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) }, { Foo(4), Foo(4) }, { Foo(5), Foo(5) } };
			const auto exp = [&map] { map.Remove(HashMap<Foo, Foo>::Iterator(), map.begin()); };
			Assert::ExpectException<std::exception>(exp);
			const auto exp2 = [&map] { map.Remove(map.end(), map.begin()); };
			Assert::ExpectException<std::exception>(exp2);
		}

		TEST_METHOD(TestReserve)
		{
			HashMap<Foo, Foo> map = { { Foo(0), Foo(0) }, { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) }, { Foo(4), Foo(4) }, { Foo(5), Foo(5) } };
			Assert::AreEqual(map.Size(), 6_z);
			Assert::IsTrue(map.ContainsKey(Foo(2)));
			map.Reserve(100);
			Assert::AreEqual(map.BucketCount(), 100_z);
			Assert::AreEqual(map.Size(), 6_z);
			Assert::IsTrue(map.ContainsKey(Foo(2)));
		}

		TEST_METHOD(TestRehash)
		{
			HashMap<Foo, Foo> map = { { Foo(0), Foo(0) }, { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) }, { Foo(4), Foo(4) }, { Foo(5), Foo(5) } };
			Assert::AreEqual(map.Size(), 6_z);
			Assert::IsTrue(map.ContainsKey(Foo(2)));
			map.Rehash(100);
			Assert::AreEqual(map.BucketCount(), 100_z);
			Assert::AreEqual(map.Size(), 6_z);
			Assert::IsTrue(map.ContainsKey(Foo(2)));
			map.Rehash(3);
			Assert::AreEqual(map.BucketCount(), 3_z);
			Assert::AreEqual(map.Size(), 6_z);
			Assert::IsTrue(map.ContainsKey(Foo(2)));
		}

		TEST_METHOD(TestContainsKey)
		{
			HashMap<Foo, Foo> map = { { Foo(0), Foo(0) }, { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) }, { Foo(4), Foo(4) }, { Foo(5), Foo(5) } };
			Assert::IsTrue(map.ContainsKey(Foo(2)));
			Assert::IsFalse(map.ContainsKey(Foo(10)));

			HashMap<Foo, Foo>::Iterator it;
			Assert::IsTrue(map.ContainsKey(Foo(2), it));
			Assert::AreEqual(it->first, Foo(2));

			const auto constMap = map;
			HashMap<Foo, Foo>::ConstIterator constIt;
			Assert::IsTrue(constMap.ContainsKey(Foo(2), constIt));
			Assert::AreEqual(constIt->first, Foo(2));
		}

		TEST_METHOD(TestClear)
		{
			HashMap<Foo, Foo> map = { { Foo(0), Foo(0) }, { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) }, { Foo(4), Foo(4) }, { Foo(5), Foo(5) } };
			Assert::AreEqual(map.Size(), 6_z);
			Assert::IsTrue(map.ContainsKey(Foo(0)));
			map.Clear();
			Assert::AreEqual(map.Size(), 0_z);
			Assert::IsFalse(map.ContainsKey(Foo(0)));
		}

		TEST_METHOD(TestIterators)
		{
			HashMap<Foo, Foo> map = { { Foo(0), Foo(0) }, { Foo(1), Foo(1) }, { Foo(2), Foo(2) }, { Foo(3), Foo(3) }, { Foo(4), Foo(4) }, { Foo(5), Foo(5) } };
			Assert::AreEqual(map.begin()->first, Foo(0));
			const auto expression = [&map] { map.end()->first; };
			Assert::ExpectException<std::exception>(expression);
			Assert::AreEqual(map.cbegin()->first, Foo(0));
			const auto expression2 = [&map] { map.cend()->first; };
			Assert::ExpectException<std::exception>(expression2);

			const auto map2 = map;
			Assert::AreEqual(map2.begin()->first, Foo(0));
			const auto expression3 = [&map2] { map2.end()->first; };
			Assert::ExpectException<std::exception>(expression3);

			map.Clear();
			const auto exp = [&map] { map.begin()->first; };
			Assert::ExpectException<std::exception>(exp);
			const auto exp2 = [&map] { map.cbegin()->first; };
			Assert::ExpectException<std::exception>(exp2);

			const auto map3 = map;
			const auto exp3 = [&map3] { map3.begin()->first; };
			Assert::ExpectException<std::exception>(exp3);
		}

		TEST_METHOD(TestLoadFactor)
		{
			HashMap<Foo, Foo> map;
			Assert::AreEqual(map.MaxLoadFactor(), 1.0f);
			Assert::AreEqual(map.LoadFactor(), 0.0f);
			map[Foo(1)] = Foo(1);
			Assert::AreEqual(map.LoadFactor(), 1.0f / 16);
			map.MaxLoadFactor(0.5f);
			Assert::AreEqual(map.MaxLoadFactor(), 0.5f);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState HashMapTest::sStartMemState;
}

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template <>
	inline std::wstring ToString<FieaGameEngine::HashMap<UnitTestLibraryDesktop::Foo, UnitTestLibraryDesktop::Foo>::Iterator>(const FieaGameEngine::HashMap<UnitTestLibraryDesktop::Foo, UnitTestLibraryDesktop::Foo>::Iterator& t)
	{
		try
		{
			FieaGameEngine::HashMap<UnitTestLibraryDesktop::Foo, UnitTestLibraryDesktop::Foo>::Iterator it = t;
			RETURN_WIDE_STRING(it->first.Data());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("Invalid iterator");
		}
	}

	template <>
	inline std::wstring ToString<FieaGameEngine::HashMap<UnitTestLibraryDesktop::Foo, UnitTestLibraryDesktop::Foo>::ConstIterator>(const FieaGameEngine::HashMap<UnitTestLibraryDesktop::Foo, UnitTestLibraryDesktop::Foo>::ConstIterator& t)
	{
		try
		{
			RETURN_WIDE_STRING(t->first.Data());
		}
		catch (std::exception)
		{
			RETURN_WIDE_STRING("Invalid iterator");
		}
	}
}
