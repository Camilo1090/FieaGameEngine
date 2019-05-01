#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListIteratorTest)
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

		TEST_METHOD(TestDefaultConstructor)
		{
			SList<Foo>::Iterator it1;
			SList<Foo>::Iterator it2;
			Assert::AreEqual(it1, it2);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			SList<Foo> list = { Foo(1) };
			SList<Foo>::ConstIterator it(list.begin());
			Assert::AreEqual(list.cbegin(), it);
		}

		TEST_METHOD(TestCopyAssignmentOperator)
		{
			SList<Foo> list = { Foo(1) };
			SList<Foo>::ConstIterator it;
			it = list.begin();
			Assert::AreEqual(list.cbegin(), it);
		}

		TEST_METHOD(TestPrivateConstructor)
		{
			SList<Foo> list1 = { Foo(1), Foo(2), Foo(3) };
			SList<Foo> list2 = { Foo(1), Foo(2), Foo(3) };

			SList<Foo>::Iterator it1 = list1.begin();
			SList<Foo>::Iterator it2 = list1.begin();
			Assert::AreEqual(it1, it2);
			it2 = list2.begin();
			Assert::AreNotEqual(it1, it2);
		}

		TEST_METHOD(TestEqualOperator)
		{
			SList<Foo> list1 = { Foo(1), Foo(2), Foo(3) };
			SList<Foo> list2 = { Foo(1), Foo(2), Foo(3) };

			SList<Foo>::Iterator it1;
			SList<Foo>::Iterator it2;
			Assert::AreEqual(it1, it2);
			it1 = list1.begin();
			it2 = list1.begin();
			Assert::AreEqual(it1, it2);
			it2 = list1.end();
			Assert::AreNotEqual(it1, it2);
			it2 = list2.begin();
			Assert::AreNotEqual(it1, it2);
			it1 = list2.end();
			it2 = list2.end();
			Assert::AreEqual(it1, it2);
		}

		TEST_METHOD(TestNotEqualOperator)
		{
			SList<Foo> list1 = { Foo(1), Foo(2), Foo(3) };
			SList<Foo> list2 = { Foo(1), Foo(2), Foo(3) };

			SList<Foo>::Iterator it1 = list1.begin();
			SList<Foo>::Iterator it2 = list2.begin();
			Assert::IsTrue(it1 != it2);
			it2 = list1.end();
			Assert::IsTrue(it1 != it2);
			list1.Find(Foo(2), it2);
			Assert::IsTrue(it1 != it2);
		}

		TEST_METHOD(TestComparison)
		{
			SList<Foo> list = { Foo(1), Foo(2), Foo(3) };
			const auto exp = [&list] { list.begin() < SList<Foo>::Iterator(); };
			Assert::ExpectException<std::exception>(exp);
		}

		TEST_METHOD(TestPreIncrementOperator)
		{
			SList<Foo> list = { Foo(1), Foo(2), Foo(3) };
			SList<Foo>::Iterator it = list.begin();
			Assert::AreEqual(*(++it), Foo(2));
			Assert::AreEqual(*(++it), Foo(3));
			Assert::AreEqual(++it, list.end());
			it = SList<Foo>::Iterator();
			const auto preInc = [&it] { ++it; };
			Assert::ExpectException<std::exception>(preInc);

			const SList<Foo> constList = { Foo(1), Foo(2), Foo(3) };
			SList<Foo>::ConstIterator constIt = constList.begin();
			Assert::AreEqual(*(++constIt), Foo(2));
			Assert::AreEqual(*(++constIt), Foo(3));
			Assert::AreEqual(++constIt, constList.end());
			constIt = SList<Foo>::ConstIterator();
			const auto preInc2 = [&constIt] { ++constIt; };
			Assert::ExpectException<std::exception>(preInc2);
		}

		TEST_METHOD(TestPostIncrementOperator)
		{
			SList<Foo> list = { Foo(1), Foo(2), Foo(3) };
			SList<Foo>::Iterator it = list.begin();
			Assert::AreEqual(*(it++), Foo(1));
			Assert::AreEqual(*(it++), Foo(2));
			Assert::AreEqual(*(it++), Foo(3));
			Assert::AreEqual(it, list.end());
			it = SList<Foo>::Iterator();
			const auto postInc = [&it] { it++; };
			Assert::ExpectException<std::exception>(postInc);

			const SList<Foo> constList = { Foo(1), Foo(2), Foo(3) };
			SList<Foo>::ConstIterator constIt = constList.begin();
			Assert::AreEqual(*(constIt++), Foo(1));
			Assert::AreEqual(*(constIt++), Foo(2));
			Assert::AreEqual(*(constIt++), Foo(3));
			Assert::AreEqual(constIt, constList.end());
			constIt = SList<Foo>::ConstIterator();
			const auto postInc2 = [&constIt] { constIt++; };
			Assert::ExpectException<std::exception>(postInc2);
		}

		TEST_METHOD(TestDereferenceOperator)
		{
			SList<Foo> list = { Foo(1), Foo(2), Foo(3) };
			SList<Foo>::Iterator it = list.begin();
			Assert::AreEqual(*(++it), Foo(2));
			Assert::AreEqual(*(++it), Foo(3));
			const auto deref = [&it] { *(++it); };
			Assert::ExpectException<std::exception>(deref);
			Assert::AreEqual(it == list.end(), true);

			const SList<Foo> constList = { Foo(1), Foo(2), Foo(3) };
			SList<Foo>::ConstIterator constIt = constList.begin();
			Assert::AreEqual(*(++constIt), Foo(2));
			Assert::AreEqual(*(++constIt), Foo(3));
			const auto deref2 = [&constIt] { *(++constIt); };
			Assert::ExpectException<std::exception>(deref2);
			Assert::AreEqual(constIt == constList.end(), true);

			SList<Foo>::ConstIterator it3;
			const auto deref3 = [&it3] { *it3; };
			Assert::ExpectException<std::exception>(deref3);

			const auto exp = [&it3] { it3->Data(); };
			Assert::ExpectException<std::exception>(exp);

			const auto exp2 = [&list] { list.end()->Data(); };
			Assert::ExpectException<std::exception>(exp2);

			const auto exp3 = [] { SList<Foo>::Iterator()->Data(); };
			Assert::ExpectException<std::exception>(exp3);

			const auto exp4 = [&list] { list.cend()->Data(); };
			Assert::ExpectException<std::exception>(exp4);
		}

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState SListIteratorTest::sStartMemState;
}
