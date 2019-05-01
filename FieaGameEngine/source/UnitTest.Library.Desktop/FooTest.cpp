#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(FooTest)
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
			Foo foo1;
			Assert::AreEqual(foo1.Data(), 0);

			const int data = 20;
			Foo foo2(data);
			Assert::AreEqual(data, foo2.Data());
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Foo foo1(1);
			Foo foo2(foo1);
			Assert::AreEqual(foo1.Data(), foo2.Data());
			foo2.Data(22);
			Assert::AreNotEqual(foo1.Data(), foo2.Data());
		}

		TEST_METHOD(TestDestructor)
		{
			Foo foo1;
			Foo foo2(100);
		}

		TEST_METHOD(TestAssignmentOperator)
		{
			Foo foo1(1);
			Assert::AreEqual(foo1.Data(), 1);
			Foo foo2(2);
			Assert::AreEqual(foo2.Data(), 2);
			foo2 = foo1;
			Assert::AreEqual(foo1.Data(), foo2.Data());
			foo2.Data(333);
			Assert::AreNotEqual(foo1.Data(), foo2.Data());
		}

		TEST_METHOD(TestEqualsOperator)
		{
			Foo foo1(1);
			Foo foo2(2);
			Foo foo3(1);
			Assert::IsFalse(foo1 == foo2);
			Assert::IsTrue(foo1 == foo3);
			Assert::IsFalse(foo3 == foo2);
		}

		TEST_METHOD(TestData)
		{
			Foo foo1(100);
			Assert::AreEqual(foo1.Data(), 100);

			int& data = foo1.Data();
			data = 5;
			Assert::AreEqual(foo1.Data(), 5);

			foo1.Data(100);
			Assert::AreEqual(foo1.Data(), 100);
		}

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState FooTest::sStartMemState;
}
