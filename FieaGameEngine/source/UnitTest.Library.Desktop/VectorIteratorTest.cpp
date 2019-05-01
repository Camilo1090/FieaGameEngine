#include "pch.h"
#include "CppUnitTest.h"
#include "Vector.h"
#include "Foo.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorIteratorTest)
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
			Vector<Foo>::Iterator it1;
			Vector<Foo>::Iterator it2;
			Assert::AreEqual(it1, it2);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Vector<Foo> list = { Foo(1) };
			Vector<Foo>::ConstIterator it(list.begin());
			Assert::AreEqual(list.cbegin(), it);
		}

		TEST_METHOD(TestCopyAssignmentOperator)
		{
			Vector<Foo> list = { Foo(1) };
			Vector<Foo>::ConstIterator it;
			it = list.begin();
			Assert::AreEqual(list.cbegin(), it);
		}

		TEST_METHOD(TestPrivateConstructor)
		{
			Vector<Foo> list1 = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo> list2 = { Foo(1), Foo(2), Foo(3) };

			Vector<Foo>::Iterator it1 = list1.begin();
			Vector<Foo>::Iterator it2 = list1.begin();
			Assert::AreEqual(it1, it2);
			it2 = list2.begin();
			Assert::AreNotEqual(it1, it2);
		}

		TEST_METHOD(TestEqualOperator)
		{
			Vector<Foo> list1 = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo> list2 = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::Iterator it1;
			Vector<Foo>::Iterator it2;
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
			Vector<Foo> list1 = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo> list2 = { Foo(1), Foo(2), Foo(3) };

			Vector<Foo>::Iterator it1 = list1.begin();
			Vector<Foo>::Iterator it2 = list2.begin();
			Assert::IsTrue(it1 != it2);
			it2 = list1.end();
			Assert::IsTrue(it1 != it2);
			it2 = list1.Find(Foo(2));
			Assert::IsTrue(it1 != it2);
		}

		TEST_METHOD(TestInequality)
		{
			Vector<Foo> list = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::Iterator it1 = list.begin();
			Vector<Foo>::Iterator it2 = list.end();
			Assert::IsTrue(it1 < it2);
			Assert::IsFalse(it1 > it2);
			it2 = list.begin();
			Assert::IsTrue(it1 <= it2);
			Assert::IsTrue(it1 >= it2);

			Vector<Foo> list2;
			it2 = list2.begin();
			const auto expression1 = [&it1, &it2] { it1 < it2; };
			Assert::ExpectException<std::exception>(expression1);
			const auto expression2 = [&it1, &it2] { it1 > it2; };
			Assert::ExpectException<std::exception>(expression2);
			const auto expression3 = [&it1, &it2] { it1 <= it2; };
			Assert::ExpectException<std::exception>(expression3);
			const auto expression4 = [&it1, &it2] { it1 >= it2; };
			Assert::ExpectException<std::exception>(expression4);

			const Vector<Foo> constList = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::ConstIterator constIt1 = constList.begin();
			Vector<Foo>::ConstIterator constIt2 = constList.end();
			Assert::IsTrue(constIt1 < constIt2);
			Assert::IsFalse(constIt1 > constIt2);
			constIt2 = constList.begin();
			Assert::IsTrue(constIt1 <= constIt2);
			Assert::IsTrue(constIt1 >= constIt2);

			const Vector<Foo> constList2;
			constIt2 = constList2.begin();
			const auto expression5 = [&constIt1, &constIt2] { constIt1 < constIt2; };
			Assert::ExpectException<std::exception>(expression5);
			const auto expression6 = [&constIt1, &constIt2] { constIt1 > constIt2; };
			Assert::ExpectException<std::exception>(expression6);
			const auto expression7 = [&constIt1, &constIt2] { constIt1 <= constIt2; };
			Assert::ExpectException<std::exception>(expression7);
			const auto expression8 = [&constIt1, &constIt2] { constIt1 >= constIt2; };
			Assert::ExpectException<std::exception>(expression8);
		}

		TEST_METHOD(TestPreIncrementOperator)
		{
			Vector<Foo> list = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::Iterator it = list.begin();
			Assert::AreEqual(*(++it), Foo(2));
			Assert::AreEqual(*(++it), Foo(3));
			Assert::AreEqual(++it, list.end());
			it = Vector<Foo>::Iterator();
			const auto preInc = [&it] { ++it; };
			Assert::ExpectException<std::exception>(preInc);

			const Vector<Foo> constList = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::ConstIterator constIt = constList.begin();
			Assert::AreEqual(*(++constIt), Foo(2));
			Assert::AreEqual(*(++constIt), Foo(3));
			Assert::AreEqual(++constIt, constList.end());
			constIt = Vector<Foo>::ConstIterator();
			const auto preInc2 = [&constIt] { ++constIt; };
			Assert::ExpectException<std::exception>(preInc2);
		}

		TEST_METHOD(TestPostIncrementOperator)
		{
			Vector<Foo> list = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::Iterator it = list.begin();
			Assert::AreEqual(*(it++), Foo(1));
			Assert::AreEqual(*(it++), Foo(2));
			Assert::AreEqual(*(it++), Foo(3));
			Assert::AreEqual(it, list.end());
			it = Vector<Foo>::Iterator();
			const auto postInc = [&it] { it++; };
			Assert::ExpectException<std::exception>(postInc);

			const Vector<Foo> constList = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::ConstIterator constIt = constList.begin();
			Assert::AreEqual(*(constIt++), Foo(1));
			Assert::AreEqual(*(constIt++), Foo(2));
			Assert::AreEqual(*(constIt++), Foo(3));
			Assert::AreEqual(constIt, constList.end());
			constIt = Vector<Foo>::ConstIterator();
			const auto postInc2 = [&constIt] { constIt++; };
			Assert::ExpectException<std::exception>(postInc2);
		}

		TEST_METHOD(TestDereferenceOperator)
		{
			Vector<Foo> list = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::Iterator it = list.begin();
			Assert::AreEqual(*(++it), Foo(2));
			Assert::AreEqual(*(++it), Foo(3));
			const auto deref = [&it] { *(++it); };
			Assert::ExpectException<std::exception>(deref);
			Assert::AreEqual(it == list.end(), true);

			const Vector<Foo> constList = { Foo(1), Foo(2), Foo(3) };
			Vector<Foo>::ConstIterator constIt = constList.begin();
			Assert::AreEqual(*(++constIt), Foo(2));
			Assert::AreEqual(*(++constIt), Foo(3));
			const auto deref2 = [&constIt] { *(++constIt); };
			Assert::ExpectException<std::exception>(deref2);
			Assert::AreEqual(constIt == constList.end(), true);

			Vector<Foo>::ConstIterator it3;
			const auto deref3 = [&it3] { *it3; };
			Assert::ExpectException<std::exception>(deref3);
		}

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState VectorIteratorTest::sStartMemState;
}