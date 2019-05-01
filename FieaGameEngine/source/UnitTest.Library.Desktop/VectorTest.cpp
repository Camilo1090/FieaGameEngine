#include "pch.h"
#include "CppUnitTest.h"
#include "vector.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTest)
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
			Vector<Foo> list;
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), 0_z);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);
			Vector<Foo> list1 = { foo1, foo2, foo3 };
			Vector<Foo> list2(list1);
			Assert::AreEqual(list2.Size(), 3_z);
			Assert::IsTrue(list2.Capacity() >= list2.Size());
			Assert::AreEqual(list1[0], list2[0]);
			(list2[0]).Data(50);
			Assert::AreNotEqual(list1[0], list2[0]);
		}

		TEST_METHOD(TestMoveConstructor)
		{
			Vector<Foo> list1(Vector<Foo>({ Foo(1), Foo(2), Foo(3) }));
			Assert::AreEqual(list1.Size(), 3_z);

			Vector<Foo> list2(std::move(list1));
			Assert::AreEqual(list1.Size(), 0_z);
			Assert::AreEqual(list2.Size(), 3_z);
			Assert::IsTrue(list2.Capacity() >= list2.Size());
			Assert::AreEqual(list2.Front(), Foo(1));
			Assert::AreEqual(list2.Back(), Foo(3));
		}

		TEST_METHOD(TestInitializerListConstructor)
		{
			Vector<Foo> list = { Foo(1), Foo(2), Foo(3) };
			Assert::AreEqual(list.Size(), 3_z);
			Assert::IsTrue(list.Capacity() >= list.Size());
			Assert::AreEqual(list[0], Foo(1));
		}

		TEST_METHOD(TestIncrementCallbackConstructor)
		{
			const auto increment = [](size_t, size_t capacity) { return capacity + 1; };
			Vector<Foo> list(0, increment);
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), 0_z);
			list.PushBack(Foo(1));
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Capacity(), 1_z);
			list.PushBack(Foo(2));
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list.Capacity(), 2_z);
		}

		TEST_METHOD(TestDestructor)
		{
			Vector<Foo> list({ Foo(1), Foo(2), Foo(3) });
		}

		TEST_METHOD(TestCopyAssignmentOperator)
		{
			Foo foo1(1), foo2(2), foo3(3);
			Vector<Foo> list1 = { foo1, foo2, foo3 };
			Vector<Foo> list2 = { foo3, foo1 };
			Assert::AreEqual(list2.Size(), 2_z);
			Assert::AreEqual(list2[0], foo3);
			list2 = list1;
			Assert::AreEqual(list2.Size(), 3_z);
			Assert::IsTrue(list2.Capacity() >= list2.Size());
			Assert::AreEqual(list1[0], list2[0]);
			(list2[0]).Data(50);
			Assert::AreNotEqual(list1[0], list2[0]);
		}

		TEST_METHOD(TestMoveAssignmentOperator)
		{
			Vector<Foo> list1 = { Foo(1) };
			Vector<Foo> list2;
			Assert::AreEqual(list1.Size(), 1_z);
			Assert::AreEqual(list2.Size(), 0_z);
			list2 = std::move(list1);
			Assert::AreEqual(list1.Size(), 0_z);
			Assert::AreEqual(list2.Size(), 1_z);
		}

		TEST_METHOD(TestInitializerListAssignmentOperator)
		{
			Vector<Foo> list = { Foo(50) };
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list[0], Foo(50));
			list = { Foo(1), Foo(2), Foo(3) };
			Assert::AreEqual(list.Size(), 3_z);
			Assert::IsTrue(list.Capacity() >= list.Size());
			Assert::AreEqual(list[0], Foo(1));
		}

		TEST_METHOD(TestOperatorBracketBracket)
		{
			Vector<Foo> list;
			const auto expression = [&list] { list[0]; };
			Assert::ExpectException<std::exception>(expression);
			const int foos = 10;
			for (int i = 0; i < foos; ++i)
			{
				Foo foo(i);
				list.PushBack(foo);
				Assert::AreEqual(list[i], foo);
			}

			const Vector<Foo> constList = list;
			for (int i = 0; i < foos; ++i)
			{
				Foo foo(i);
				Assert::AreEqual(constList[i], foo);
			}
			const auto expression2 = [&constList] { constList[1000]; };
			Assert::ExpectException<std::exception>(expression2);
		}

		TEST_METHOD(TestAt)
		{
			Vector<Foo> list;
			const auto expression = [&list] { list.At(0); };
			Assert::ExpectException<std::exception>(expression);
			const int foos = 10;
			for (int i = 0; i < foos; ++i)
			{
				Foo foo(i);
				list.PushBack(foo);
				Assert::AreEqual(list.At(i), foo);
			}

			const Vector<Foo> constList = list;
			for (int i = 0; i < foos; ++i)
			{
				Foo foo(i);
				Assert::AreEqual(constList.At(i), foo);
			}
			const auto expression2 = [&constList] { constList.At(1000); };
			Assert::ExpectException<std::exception>(expression2);
		}

		TEST_METHOD(TestPushBack)
		{
			Vector<Foo> list;
			const int foos = 10;
			for (int i = 0; i < foos; ++i)
			{
				Foo foo(i);
				list.PushBack(foo);
				Assert::AreEqual(list.At(i), foo);
				Assert::AreEqual(list.Size(), i + 1_z);
				Assert::IsTrue(list.Capacity() >= list.Size());
			}

			list.ShrinkToFit();
			const auto increment = [](size_t, size_t) { return 0_z; };
			list.SetIncrementCallback(increment);
			const auto expression = [&list] { list.PushBack(Foo()); };
			Assert::ExpectException<std::exception>(expression);

			Vector<int> a;
			a.push_back(1);
			int b = 5;
			a.push_back(b);
			Assert::AreEqual(a.Size(), 2_z);
		}

		TEST_METHOD(TestPopBack)
		{
			Vector<Foo> list = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) };
			size_t size = list.Size();
			size_t capacity = list.Capacity();
			while (list.Size() > 0)
			{
				list.PopBack();
				--size;
				Assert::AreEqual(list.Size(), size);
				Assert::AreEqual(list.Capacity(), capacity);
			}

			const auto expression = [&list] { list.PopBack(); };
			Assert::ExpectException<std::exception>(expression);
		}

		TEST_METHOD(TestReserve)
		{
			Vector<Foo> list;
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), 0_z);
			const size_t capacity = 10;
			list.Reserve(capacity);
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), capacity);
			for (size_t i = 0; i < capacity; ++i)
			{
				list.PushBack(Foo(static_cast<int>(i)));
				Assert::AreEqual(list.Size(), i + 1_z);
				Assert::AreEqual(list.Capacity(), capacity);
			}
		}

		TEST_METHOD(TestShrinkToFit)
		{
			Vector<Foo> list;
			const size_t capacity = 10;
			list.Reserve(capacity);
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), capacity);
			list.PushBack(Foo(1));
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Capacity(), capacity);
			list.ShrinkToFit();
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Capacity(), 1_z);
		}

		TEST_METHOD(TestClear)
		{
			Vector<Foo> list;
			const size_t capacity = 10;
			list.Reserve(capacity);
			for (size_t i = 0; i < capacity; ++i)
			{
				list.PushBack(Foo(static_cast<int>(i)));
			}
			list.Clear();
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), capacity);
		}

		TEST_METHOD(TestFind)
		{
			Foo foo1(1), foo2(2), foo3(3);
			Vector<Foo> list = { foo1, foo2, foo3 };
			Vector<Foo>::Iterator it = list.Find(foo1);
			Assert::AreEqual(it, list.begin());
			it = list.Find(foo2);
			Assert::AreEqual(*it, foo2);
			it = list.Find(Foo(1000));
			Assert::AreEqual(it, list.end());

			const Vector<Foo> constList = { foo1, foo2, foo3 };
			Vector<Foo>::ConstIterator constIt = constList.Find(foo1);
			Assert::AreEqual(constIt, constList.begin());
			constIt = constList.Find(foo2);
			Assert::AreEqual(*constIt, foo2);
			constIt = constList.Find(Foo(1000));
			Assert::AreEqual(constIt, constList.end());
		}

		TEST_METHOD(TestResize)
		{
			Vector<Foo> list = { Foo(1), Foo(2), Foo(3) };
			Assert::AreEqual(list.Size(), 3_z);
			list.Resize(6);
			Assert::AreEqual(list.Size(), 6_z);
			list.Resize(1);
			Assert::AreEqual(list.Size(), 1_z);
		}

		TEST_METHOD(TestRemove)
		{
			Vector<Foo> list;
			const size_t capacity = 10;
			list.Reserve(capacity);
			for (size_t i = 0; i < capacity; ++i)
			{
				list.PushBack(Foo(static_cast<int>(i)));
			}

			list.Remove(list.begin());
			Assert::AreEqual(list.Size(), capacity - 1);
			Assert::AreEqual(list.Capacity(), capacity);

			list.Remove(list.Find(Foo(2)), list.Find(Foo(7)));
			Assert::AreEqual(list.Size(), capacity - 6);
			Assert::AreEqual(list.Capacity(), capacity);

			const auto expression = [&list] { list.Remove(list.end(), list.begin()); };
			Assert::ExpectException<std::exception>(expression);

			const auto expression2 = [&list] { list.Remove(list.begin(), Vector<Foo>::Iterator()); };
			Assert::ExpectException<std::exception>(expression2);

			list.Remove(list.begin(), list.end());
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), capacity);
		}

		TEST_METHOD(TestBeginAndEnd)
		{
			Vector<Foo> list;
			Assert::AreEqual(list.begin(), list.end());

			const size_t capacity = 10;
			list.Reserve(capacity);
			for (size_t i = 0; i < capacity; ++i)
			{
				list.PushBack(Foo(static_cast<int>(i)));
			}

			size_t i = 0;
			for (const auto& value : list)
			{
				Assert::AreEqual(value, Foo(static_cast<int>(i)));
				++i;
			}

			const Vector<Foo> constList(list);
			i = 0;
			for (const auto& value : constList)
			{
				Assert::AreEqual(value, Foo(static_cast<int>(i)));
				++i;
			}

			Assert::AreEqual(constList.begin(), constList.cbegin());
			Assert::AreEqual(constList.end(), constList.cend());
		}

		TEST_METHOD(TestIsEmpty)
		{
			Vector<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			const size_t capacity = 3;
			list.Reserve(capacity);
			for (size_t i = 0; i < capacity; ++i)
			{
				list.PushBack(Foo(static_cast<int>(i)));
			}
			Assert::IsFalse(list.IsEmpty());

			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(TestFront)
		{
			Vector<Foo> list;
			const size_t capacity = 5;
			list.Reserve(capacity);
			for (size_t i = 0; i < capacity; ++i)
			{
				list.PushBack(Foo(static_cast<int>(i)));
			}
			Assert::AreEqual(list.Front(), Foo(0));
			Foo& ref = list.Front();
			ref = Foo(55);
			Assert::AreEqual(list.Front(), Foo(55));
			list.Remove(list.begin());
			Assert::AreEqual(list.Front(), Foo(1));

			list.Clear();
			const auto expression = [&list] { list.Front(); };
			Assert::ExpectException<std::exception>(expression);
			const Vector<Foo> constList1 = list;
			const auto expression2 = [&constList1] { constList1.Front(); };
			Assert::ExpectException<std::exception>(expression2);
			const Vector<Foo> constList2 = { Foo(1), Foo(2) };
			Assert::AreEqual(constList2.Front(), Foo(1));
		}

		TEST_METHOD(TestBack)
		{
			Vector<Foo> list;
			const size_t capacity = 5;
			list.Reserve(capacity);
			for (size_t i = 0; i < capacity; ++i)
			{
				list.PushBack(Foo(static_cast<int>(i)));
			}
			Assert::AreEqual(list.Back(), Foo(4));
			Foo& ref = list.Back();
			ref.Data(55);
			Assert::AreEqual(list.Back(), Foo(55));
			list.PopBack();
			Assert::AreEqual(list.Back(), Foo(3));

			list.Clear();
			const auto expression = [&list] { list.Back(); };
			Assert::ExpectException<std::exception>(expression);
			const Vector<Foo> constList1 = list;
			const auto expression2 = [&constList1] { constList1.Back(); };
			Assert::ExpectException<std::exception>(expression2);
			const Vector<Foo> constList2 = { Foo(1), Foo(2) };
			Assert::AreEqual(constList2.Back(), Foo(2));
		}

		TEST_METHOD(TestSize)
		{
			Vector<Foo> list;
			Assert::AreEqual(list.Size(), 0_z);
			list.PushBack(Foo(1));
			Assert::AreEqual(list.Size(), 1_z);
			list.PopBack();
			Assert::AreEqual(list.Size(), 0_z);
		}

		TEST_METHOD(TestCapacity)
		{
			Vector<Foo> list;
			Assert::AreEqual(list.Capacity(), 0_z);
			list.PushBack(Foo(1));
			Assert::IsTrue(list.Capacity() >= 1_z);
			list.PopBack();
			Assert::IsTrue(list.Capacity() >= 1_z);
			const size_t capacity = 10_z;
			list.Reserve(capacity);
			Assert::AreEqual(list.Capacity(), capacity);
			for (size_t i = 0; i < capacity; ++i)
			{
				list.PushBack(Foo(static_cast<int>(i)));
			}
			Assert::AreEqual(list.Capacity(), capacity);
			list.Clear();
			Assert::AreEqual(list.Capacity(), capacity);
			list.ShrinkToFit();
			Assert::AreEqual(list.Capacity(), 0_z);
		}

		TEST_METHOD(TestSetIncrementCallback)
		{
			Vector<Foo> list;
			list.PushBack(Foo(1));
			Assert::IsTrue(list.Capacity() >= list.Size());
			list.ShrinkToFit();
			list.SetIncrementCallback([](size_t, size_t) { return 0_z; });
			const auto expression = [&list] { list.PushBack(Foo(1)); };
			Assert::ExpectException<std::exception>(expression);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTest::sStartMemState;
}