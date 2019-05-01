#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Stack.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
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
			SList<int> list;
			Assert::AreEqual(list.Size(), 0_z);

			auto front = [&list] { list.Front(); };
			Assert::ExpectException<std::exception>(front);

			auto back = [&list] { list.Back(); };
			Assert::ExpectException<std::exception>(back);

			SList<Foo> fooList;
			Assert::AreEqual(fooList.Size(), 0_z);

			auto fooFront = [&fooList] { fooList.Front(); };
			Assert::ExpectException<std::exception>(fooFront);

			auto fooBack = [&fooList] { fooList.Back();	};
			Assert::ExpectException<std::exception>(fooBack);

			SList<Foo*> fooPtrList;
			Assert::AreEqual(fooPtrList.Size(), 0_z);

			auto fooPtrFront = [&fooPtrList] { fooPtrList.Front(); };
			Assert::ExpectException<std::exception>(fooPtrFront);

			auto fooPtrBack = [&fooPtrList]	{ fooPtrList.Back(); };
			Assert::ExpectException<std::exception>(fooPtrBack);

			//SList<const int> test = { 1, 2, 3 };
			//test.Remove(2);
		}

		TEST_METHOD(TestInitializerListConstructor)
		{
			SList<int> list = { 1, 2, 3 };
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(list.Front(), 1);
			Assert::AreEqual(list.Back(), 3);

			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);
			SList<Foo> fooList = { foo1, foo2, foo3 };
			Assert::AreEqual(fooList.Size(), 3_z);
			Assert::AreEqual(fooList.Front(), foo1);
			Assert::AreEqual(fooList.Back(), foo3);

			SList<Foo*> fooPtrList = { &foo1, &foo2, &foo3 };
			Assert::AreEqual(fooPtrList.Size(), 3_z);
			Assert::AreEqual(fooPtrList.Front(), &foo1);
			Assert::AreEqual(fooPtrList.Back(), &foo3);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			SList<int> list1;
			list1.PushFront(1);
			list1.PushFront(2);
			list1.PushFront(3);

			SList<int> list2(list1);
			Assert::AreEqual(list2.Size(), 3_z);
			Assert::AreEqual(list2.Front(), 3);
			Assert::AreEqual(list2.Back(), 1);

			int& item = list1.Front();
			item = 55;
			Assert::AreEqual(list1.Front(), 55);
			Assert::AreEqual(list2.Front(), 3);

			SList<Foo> fooList1;
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);
			fooList1.PushFront(foo1);
			fooList1.PushFront(foo2);
			fooList1.PushFront(foo3);

			SList<Foo> fooList2(fooList1);
			Assert::AreEqual(fooList2.Size(), 3_z);
			Assert::AreEqual(fooList2.Front(), foo3);
			Assert::AreEqual(fooList2.Back(), foo1);

			Foo& fooItem = fooList1.Front();
			fooItem.Data(55);
			Assert::AreEqual(fooList1.Front(), fooItem);
			Assert::AreEqual(fooList2.Front(), foo3);

			SList<Foo*> fooPtrList1;
			fooPtrList1.PushFront(&foo1);
			fooPtrList1.PushFront(&foo2);
			fooPtrList1.PushFront(&foo3);

			SList<Foo*> fooPtrList2(fooPtrList1);
			Assert::AreEqual(fooPtrList2.Size(), 3_z);
			Assert::AreEqual(fooPtrList2.Front(), &foo3);
			Assert::AreEqual(fooPtrList2.Back(), &foo1);

			Foo*& fooPtrItem = fooPtrList1.Front();
			Foo foo4(4);
			fooPtrItem = &foo4;
			Assert::AreEqual(fooPtrList1.Front(), &foo4);
			Assert::AreEqual(fooPtrList2.Front(), &foo3);
		}

		TEST_METHOD(TestDestructor)
		{
			SList<int> list1;
			list1.PushFront(1);
			list1.PushBack(2);

			SList<Foo> fooList1;
			Foo foo1(1);
			Foo foo2(2);
			fooList1.PushFront(foo1);
			fooList1.PushBack(foo2);

			SList<Foo*> fooPtrList1;
			fooPtrList1.PushFront(&foo1);
			fooPtrList1.PushBack(&foo2);
		}

		TEST_METHOD(TestAssignmentOperator)
		{
			SList<int> list1;
			list1.PushBack(1);
			list1.PushBack(2);
			list1.PushBack(3);

			SList<int> list2;
			list2.PushBack(44);

			list2 = list1;
			Assert::AreEqual(list2.Size(), 3_z);
			Assert::AreEqual(list2.Front(), 1);
			Assert::AreEqual(list2.Back(), 3);

			int& item = list2.Front();
			item = 44;
			Assert::AreEqual(list2.Front(), 44);
			Assert::AreEqual(list1.Front(), 1);

			SList<Foo> fooList1;
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);
			fooList1.PushBack(foo1);
			fooList1.PushBack(foo2);
			fooList1.PushBack(foo3);

			SList<Foo> fooList2;
			fooList2.PushBack(Foo(44));

			fooList2 = fooList1;
			Assert::AreEqual(fooList2.Size(), 3_z);
			Assert::AreEqual(fooList2.Front(), foo1);
			Assert::AreEqual(fooList2.Back(), foo3);

			Foo& fooItem = fooList2.Front();
			fooItem.Data(44);
			Assert::AreEqual(fooList2.Front(), Foo(44));
			Assert::AreEqual(fooList1.Front(), foo1);

			SList<Foo*> fooPtrList1;
			fooPtrList1.PushBack(&foo1);
			fooPtrList1.PushBack(&foo2);
			fooPtrList1.PushBack(&foo3);

			SList<Foo*> fooPtrList2;
			Foo foo4(4);
			fooPtrList2.PushBack(&foo4);

			fooPtrList2 = fooPtrList1;
			Assert::AreEqual(fooPtrList2.Size(), 3_z);
			Assert::AreEqual(fooPtrList2.Front(), &foo1);
			Assert::AreEqual(fooPtrList2.Back(), &foo3);

			Foo*& fooPtrItem = fooPtrList2.Front();
			fooPtrItem = &foo4;
			Assert::AreEqual(fooPtrList2.Front(), &foo4);
			Assert::AreEqual(fooPtrList1.Front(), &foo1);

			fooList1 = { foo1, foo2, foo3 };
			Assert::AreEqual(fooList1.Size(), 3_z);
			Assert::AreEqual(fooList1.Front(), foo1);
			Assert::AreEqual(fooList1.Back(), foo3);
		}

		TEST_METHOD(TestPushFront)
		{
			SList<int> list;
			list.PushFront(1);
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Front(), 1);
			Assert::AreEqual(list.Back(), 1);

			list.PushFront(2);
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list.Front(), 2);
			Assert::AreEqual(list.Back(), 1);

			list.PushFront(3);
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(list.Front(), 3);
			Assert::AreEqual(list.Back(), 1);

			SList<Foo> fooList;
			Foo foo1(1);
			fooList.PushFront(foo1);
			Assert::AreEqual(fooList.Size(), 1_z);
			Assert::AreEqual(fooList.Front(), foo1);
			Assert::AreEqual(fooList.Back(), foo1);

			Foo foo2(2);
			fooList.PushFront(foo2);
			Assert::AreEqual(fooList.Size(), 2_z);
			Assert::AreEqual(fooList.Front(), foo2);
			Assert::AreEqual(fooList.Back(), foo1);

			Foo foo3(3);
			fooList.PushFront(foo3);
			Assert::AreEqual(fooList.Size(), 3_z);
			Assert::AreEqual(fooList.Front(), foo3);
			Assert::AreEqual(fooList.Back(), foo1);

			SList<Foo*> fooPtrList;			
			fooPtrList.PushFront(&foo1);
			Assert::AreEqual(fooPtrList.Size(), 1_z);
			Assert::AreEqual(fooPtrList.Front(), &foo1);
			Assert::AreEqual(fooPtrList.Back(), &foo1);
			foo1.Data(100);
			Assert::AreEqual(fooPtrList.Front()->Data(), foo1.Data());
			Assert::AreEqual(fooPtrList.Back()->Data(), foo1.Data());

			fooPtrList.PushFront(&foo2);
			Assert::AreEqual(fooPtrList.Size(), 2_z);
			Assert::AreEqual(fooPtrList.Front(), &foo2);
			Assert::AreEqual(fooPtrList.Back(), &foo1);

			fooPtrList.PushFront(&foo3);
			Assert::AreEqual(fooPtrList.Size(), 3_z);
			Assert::AreEqual(fooPtrList.Front(), &foo3);
			Assert::AreEqual(fooPtrList.Back(), &foo1);
		}

		TEST_METHOD(TestPopFront)
		{
			SList<int> list;

			auto popFront = [&list] { list.PopFront(); };
			Assert::AreEqual(list.Size(), 0_z);
			Assert::ExpectException<std::exception>(popFront);

			list.PushFront(1);
			list.PopFront();
			Assert::AreEqual(list.Size(), 0_z);

			list.PushFront(1);
			list.PushFront(2);
			list.PopFront();
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Front(), 1);
			Assert::AreEqual(list.Back(), 1);

			list.PopFront();
			list.PushFront(1);
			list.PushFront(2);
			list.PushFront(3);
			list.PopFront();
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list.Front(), 2);
			Assert::AreEqual(list.Back(), 1);

			SList<Foo> fooList;

			auto fooPopFront = [&fooList] {	fooList.PopFront();	};
			Assert::AreEqual(fooList.Size(), 0_z);
			Assert::ExpectException<std::exception>(fooPopFront);

			Foo foo1(1);
			fooList.PushFront(foo1);
			fooList.PopFront();
			Assert::AreEqual(fooList.Size(), 0_z);

			Foo foo2(2);
			fooList.PushFront(foo1);
			fooList.PushFront(foo2);
			fooList.PopFront();
			Assert::AreEqual(fooList.Size(), 1_z);
			Assert::AreEqual(fooList.Front(), foo1);
			Assert::AreEqual(fooList.Back(), foo1);

			Foo foo3(3);
			fooList.PopFront();
			fooList.PushFront(foo1);
			fooList.PushFront(foo2);
			fooList.PushFront(foo3);
			fooList.PopFront();
			Assert::AreEqual(fooList.Size(), 2_z);
			Assert::AreEqual(fooList.Front(), foo2);
			Assert::AreEqual(fooList.Back(), foo1);

			SList<Foo*> fooPtrList;

			auto fooPtrPopFront = [&fooPtrList] { fooPtrList.PopFront(); };
			Assert::AreEqual(fooPtrList.Size(), 0_z);
			Assert::ExpectException<std::exception>(fooPtrPopFront);

			fooPtrList.PushFront(&foo1);
			fooPtrList.PopFront();
			Assert::AreEqual(fooPtrList.Size(), 0_z);

			fooPtrList.PushFront(&foo1);
			fooPtrList.PushFront(&foo2);
			fooPtrList.PopFront();
			Assert::AreEqual(fooPtrList.Size(), 1_z);
			Assert::AreEqual(fooPtrList.Front(), &foo1);
			Assert::AreEqual(fooPtrList.Back(), &foo1);

			fooPtrList.PopFront();
			fooPtrList.PushFront(&foo1);
			fooPtrList.PushFront(&foo2);
			fooPtrList.PushFront(&foo3);
			fooPtrList.PopFront();
			Assert::AreEqual(fooPtrList.Size(), 2_z);
			Assert::AreEqual(fooPtrList.Front(), &foo2);
			Assert::AreEqual(fooPtrList.Back(), &foo1);
		}

		TEST_METHOD(TestPushBack)
		{
			SList<int> list;
			list.PushBack(1);
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Front(), 1);
			Assert::AreEqual(list.Back(), 1);

			list.PushBack(2);
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list.Front(), 1);
			Assert::AreEqual(list.Back(), 2);

			list.PushBack(3);
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(list.Front(), 1);
			Assert::AreEqual(list.Back(), 3);

			SList<Foo> fooList;
			Foo foo1(1);
			fooList.PushBack(foo1);
			Assert::AreEqual(fooList.Size(), 1_z);
			Assert::AreEqual(fooList.Back(), foo1);
			Assert::AreEqual(fooList.Front(), foo1);

			Foo foo2(2);
			fooList.PushBack(foo2);
			Assert::AreEqual(fooList.Size(), 2_z);
			Assert::AreEqual(fooList.Back(), foo2);
			Assert::AreEqual(fooList.Front(), foo1);

			Foo foo3(3);
			fooList.PushBack(foo3);
			Assert::AreEqual(fooList.Size(), 3_z);
			Assert::AreEqual(fooList.Back(), foo3);
			Assert::AreEqual(fooList.Front(), foo1);

			SList<Foo*> fooPtrList;
			fooPtrList.PushBack(&foo1);
			Assert::AreEqual(fooPtrList.Size(), 1_z);
			Assert::AreEqual(fooPtrList.Front(), &foo1);
			Assert::AreEqual(fooPtrList.Back(), &foo1);

			fooPtrList.PushBack(&foo2);
			Assert::AreEqual(fooPtrList.Size(), 2_z);
			Assert::AreEqual(fooPtrList.Back(), &foo2);
			Assert::AreEqual(fooPtrList.Front(), &foo1);

			fooPtrList.PushBack(&foo3);
			Assert::AreEqual(fooPtrList.Size(), 3_z);
			Assert::AreEqual(fooPtrList.Back(), &foo3);
			Assert::AreEqual(fooPtrList.Front(), &foo1);
		}

		TEST_METHOD(TestPopBack)
		{
			SList<int> list;

			auto popBack = [&list] { list.PopBack(); };
			Assert::AreEqual(list.Size(), 0_z);
			Assert::ExpectException<std::exception>(popBack);

			list.PushFront(1);
			list.PopBack();
			Assert::AreEqual(list.Size(), 0_z);

			list.PushFront(1);
			list.PushFront(2);
			list.PopBack();
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Front(), 2);
			Assert::AreEqual(list.Back(), 2);

			list.PopBack();
			list.PushFront(1);
			list.PushFront(2);
			list.PushFront(3);
			list.PopBack();
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list.Front(), 3);
			Assert::AreEqual(list.Back(), 2);

			SList<Foo> fooList;

			auto fooPopBack = [&fooList] { fooList.PopBack(); };
			Assert::AreEqual(fooList.Size(), 0_z);
			Assert::ExpectException<std::exception>(fooPopBack);

			Foo foo1(1);
			fooList.PushBack(foo1);
			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), 0_z);

			Foo foo2(2);
			fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), 1_z);
			Assert::AreEqual(fooList.Front(), foo1);
			Assert::AreEqual(fooList.Back(), foo1);

			fooList.PopBack();
			Foo foo3(3);
			fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			fooList.PushBack(foo3);
			fooList.PopBack();
			Assert::AreEqual(fooList.Size(), 2_z);
			Assert::AreEqual(fooList.Front(), foo1);
			Assert::AreEqual(fooList.Back(), foo2);

			SList<Foo*> fooPtrList;

			auto fooPtrPopBack = [&fooPtrList] { fooPtrList.PopBack(); };
			Assert::AreEqual(fooPtrList.Size(), 0_z);
			Assert::ExpectException<std::exception>(fooPtrPopBack);

			fooPtrList.PushBack(&foo1);
			fooPtrList.PopBack();
			Assert::AreEqual(fooPtrList.Size(), 0_z);

			fooPtrList.PushBack(&foo1);
			fooPtrList.PushBack(&foo2);
			fooPtrList.PopBack();
			Assert::AreEqual(fooPtrList.Size(), 1_z);
			Assert::AreEqual(fooPtrList.Front(), &foo1);
			Assert::AreEqual(fooPtrList.Back(), &foo1);

			fooPtrList.PopBack();
			fooPtrList.PushBack(&foo1);
			fooPtrList.PushBack(&foo2);
			fooPtrList.PushBack(&foo3);
			fooPtrList.PopBack();
			Assert::AreEqual(fooPtrList.Size(), 2_z);
			Assert::AreEqual(fooPtrList.Front(), &foo1);
			Assert::AreEqual(fooPtrList.Back(), &foo2);
		}

		TEST_METHOD(TestInsertAfter)
		{
			SList<Foo> list = { Foo(1), Foo(3), Foo(6) };
			list.InsertAfter(list.begin(), Foo(2));
			Assert::AreEqual(list.Size(), 4_z);
			SList<Foo>::Iterator it;			
			Assert::IsTrue(list.Find(Foo(2), it));
			Assert::AreEqual(*it, Foo(2));
			Assert::IsTrue(list.Find(Foo(3), it));
			it = list.InsertAfter(it, Foo(4));
			it = list.InsertAfter(it, Foo(5));
			Assert::AreEqual(list.Size(), std::size_t(6));
			it = list.end();
			auto insert = [&list, &it] { list.InsertAfter(it, Foo(100)); };
			Assert::ExpectException<std::exception>(insert);
			it = SList<Foo>::Iterator();
			Assert::ExpectException<std::exception>(insert);

			list = { Foo(1), Foo(2) };
			list.Find(Foo(2), it);
			it = list.InsertAfter(it, Foo(3));
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(list.Back(), *it);
		}

		TEST_METHOD(TestFind)
		{
			SList<Foo> list = { Foo(1), Foo(3), Foo(6) };
			SList<Foo>::Iterator it;
			Assert::IsTrue(list.Find(Foo(1), it));
			Assert::AreEqual(it, list.begin());
			Assert::IsFalse(list.Find(Foo(100), it));
			Assert::AreEqual(it, list.end());
			Assert::IsTrue(list.Find(Foo(6), it));
			Assert::AreEqual(*it, Foo(6));
		}

		TEST_METHOD(TestRemove)
		{
			SList<Foo> list = { Foo(1), Foo(2), Foo(3) };
			Assert::AreEqual(list.Size(), 3_z);

			SList<Foo>::Iterator it;
			bool result = list.Remove(Foo(1));
			Assert::IsTrue(result);
			Assert::AreEqual(list.Size(), 2_z);
			list.Find(Foo(2), it);
			Assert::AreEqual(it, list.begin());
			Assert::AreEqual(list.Front(), Foo(2));
			Assert::AreEqual(list.Back(), Foo(3));

			result = list.Remove(Foo(1000));
			Assert::IsFalse(result);
			Assert::AreEqual(list.Size(), 2_z);

			result = list.Remove(list.begin());
			Assert::IsTrue(result);
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Front(), Foo(3));
			Assert::AreEqual(list.Back(), Foo(3));

			result = list.Remove(Foo(3));
			Assert::IsTrue(result);
			Assert::AreEqual(list.Size(), 0_z);
			const auto front = [&list] { list.Front(); };
			const auto back = [&list] { list.Back(); };
			Assert::ExpectException<std::exception>(front);
			Assert::ExpectException<std::exception>(back);

			list = { Foo(1), Foo(2) };
			const auto invalidRemove = [&list] { list.Remove(SList<Foo>::Iterator()); };
			Assert::ExpectException<std::exception>(invalidRemove);
			Assert::AreEqual(list.Size(), 2_z);

			list = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6) };
			list.Remove(list.Find(Foo(5)), list.end());
			Assert::AreEqual(list.Size(), 4_z);
			Assert::AreEqual(list.Front(), Foo(1));
			Assert::AreEqual(list.Back(), Foo(4));
			list.Remove(list.begin(), list.Find(Foo(3)));
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list.Front(), Foo(3));
			Assert::AreEqual(list.Back(), Foo(4));
			list.Remove(list.begin(), list.Find(Foo(4)));
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Front(), Foo(4));
			Assert::AreEqual(list.Back(), Foo(4));
			list.Remove(list.begin(), list.end());
			Assert::AreEqual(list.Size(), 0_z);

			list = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5), Foo(6) };
			const auto exp = [&list] { list.Remove(list.end(), list.begin()); };
			Assert::ExpectException<std::exception>(exp);
			const auto exp2 = [&list] { list.Remove(SList<Foo>::Iterator(), list.begin()); };
			Assert::ExpectException<std::exception>(exp2);
		}

		TEST_METHOD(TestClear)
		{
			SList<int> list;
			list.PushFront(1);
			list.PushFront(2);
			list.PushFront(3);
			list.PushFront(4);
			Assert::AreEqual(list.Size(), 4_z);
			list.Clear();
			Assert::AreEqual(list.Size(), 0_z);

			SList<Foo> fooList;
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(3);
			Foo foo4(4);
			fooList.PushFront(foo1);
			fooList.PushFront(foo2);
			fooList.PushFront(foo3);
			fooList.PushFront(foo4);
			Assert::AreEqual(fooList.Size(), 4_z);
			fooList.Clear();
			Assert::AreEqual(fooList.Size(), 0_z);

			SList<Foo*> fooPtrList;
			fooPtrList.PushFront(&foo1);
			fooPtrList.PushFront(&foo2);
			fooPtrList.PushFront(&foo3);
			fooPtrList.PushFront(&foo4);
			Assert::AreEqual(fooPtrList.Size(), 4_z);
			fooPtrList.Clear();
			Assert::AreEqual(fooPtrList.Size(), 0_z);
		}

		TEST_METHOD(TestIsEmpty)
		{
			SList<int> list;
			Assert::IsTrue(list.IsEmpty());
			list.PushFront(1);
			list.PushBack(2);
			Assert::IsFalse(list.IsEmpty());
			list.Clear();
			Assert::IsTrue(list.IsEmpty());

			SList<Foo> fooList;
			Assert::IsTrue(fooList.IsEmpty());
			Foo foo1(1);
			Foo foo2(2);
			fooList.PushFront(foo1);
			fooList.PushBack(foo2);
			Assert::IsFalse(fooList.IsEmpty());
			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());

			SList<Foo*> fooPtrList;
			Assert::IsTrue(fooPtrList.IsEmpty());
			fooPtrList.PushFront(&foo1);
			fooPtrList.PushBack(&foo2);
			Assert::IsFalse(fooPtrList.IsEmpty());
			fooPtrList.Clear();
			Assert::IsTrue(fooPtrList.IsEmpty());
		}

		TEST_METHOD(TestSize)
		{
			SList<int> list;
			Assert::AreEqual(list.Size(), 0_z);
			list.PushFront(1);
			list.PushBack(2);
			Assert::AreEqual(list.Size(), 2_z);

			SList<Foo> fooList;
			Assert::AreEqual(fooList.Size(), 0_z);
			Foo foo1(1);
			Foo foo2(2);
			fooList.PushFront(foo1);
			fooList.PushBack(foo2);
			Assert::AreEqual(fooList.Size(), 2_z);

			SList<Foo*> fooPtrList;
			Assert::AreEqual(fooPtrList.Size(), 0_z);
			fooPtrList.PushFront(&foo1);
			fooPtrList.PushBack(&foo2);
			Assert::AreEqual(fooPtrList.Size(), 2_z);
		}

		TEST_METHOD(TestFront)
		{
			SList<int> list;

			auto front = [&list] { list.Front(); };
			Assert::ExpectException<std::exception>(front);

			list.PushFront(1);
			int& item = list.Front();
			item = 55;
			Assert::AreEqual(list.Front(), 55);
			list.PushFront(2);
			Assert::AreEqual(list.Front(), 2);
			list.PopFront();
			Assert::AreEqual(list.Front(), 55);

			SList<Foo> fooList;

			auto fooFront = [&fooList] { fooList.Front(); };
			Assert::ExpectException<std::exception>(fooFront);

			Foo foo1(1);
			fooList.PushFront(foo1);
			Assert::AreEqual(fooList.Front(), foo1);
			Foo& fooItem = fooList.Front();
			fooItem.Data(55);
			Assert::AreEqual(fooList.Front().Data(), 55);
			Foo foo2(2);
			fooList.PushFront(foo2);
			Assert::AreEqual(fooList.Front(), foo2);
			fooList.PopFront();
			Assert::AreEqual(fooList.Front(), Foo(55));

			SList<Foo*> fooPtrList;

			auto fooPtrFront = [&fooPtrList] { fooPtrList.Front(); };
			Assert::ExpectException<std::exception>(fooPtrFront);

			fooPtrList.PushFront(&foo1);
			Foo*& fooPtrItem = fooPtrList.Front();
			Assert::AreEqual(fooPtrItem, &foo1);
			Foo foo3(3);
			fooPtrItem = &foo3;
			Assert::AreEqual(fooPtrList.Front(), &foo3);
			fooPtrList.PushFront(&foo2);
			Assert::AreEqual(fooPtrList.Front(), &foo2);
			fooPtrList.PopFront();
			Assert::AreEqual(fooPtrList.Front(), &foo3);

			fooList = { foo1, foo2 };
			const SList<Foo>& alias = fooList;
			Assert::AreEqual(alias.Front(), foo1);
		}

		TEST_METHOD(TestBack)
		{
			SList<int> list;

			auto back = [&list] { list.Back(); };
			Assert::ExpectException<std::exception>(back);

			list.PushFront(1);
			list.PushFront(2);
			int& item = list.Back();
			item = 55;
			Assert::AreEqual(list.Back(), 55);
			Assert::AreEqual(list.Front(), 2);
			list.PushBack(3);
			Assert::AreEqual(list.Back(), 3);
			list.PopBack();
			Assert::AreEqual(list.Back(), 55);

			SList<Foo> fooList;

			auto fooBack = [&fooList] { fooList.Back(); };
			Assert::ExpectException<std::exception>(fooBack);

			Foo foo1(1);
			fooList.PushBack(foo1);
			Assert::AreEqual(fooList.Back(), foo1);
			Foo& fooItem = fooList.Back();
			fooItem = Foo(55);
			Assert::AreEqual(fooList.Back(), Foo(55));
			Foo foo2(2);
			fooList.PushBack(foo2);
			Assert::AreEqual(fooList.Back(), foo2);
			fooList.PopBack();
			Assert::AreEqual(fooList.Back(), Foo(55));

			SList<Foo*> fooPtrList;

			auto fooPtrBack = [&fooPtrList] { fooPtrList.Back(); };
			Assert::ExpectException<std::exception>(fooPtrBack);

			fooPtrList.PushBack(&foo1);
			Foo*& fooPtrItem = fooPtrList.Back();
			Assert::AreEqual(fooPtrList.Back(), &foo1);
			fooPtrItem = &foo2;
			Assert::AreEqual(fooPtrList.Back(), &foo2);
			fooPtrList.PushBack(&foo1);
			Assert::AreEqual(fooPtrList.Back(), &foo1);
			fooPtrList.PopBack();
			Assert::AreEqual(fooPtrList.Back(), &foo2);

			fooList = { foo1, foo2 };
			const SList<Foo>& alias = fooList;
			Assert::AreEqual(alias.Back(), foo2);
		}

		TEST_METHOD(TestStack)
		{
			Stack<size_t> s1;
			s1.Push(1_z);
			s1.Push(2_z);
			s1.Push(3_z);
			Stack<size_t> s2 = s1;
			s2;
			s1.Pop();
			s1;
		}

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState SListTest::sStartMemState;
}