#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <sstream>
#include <json/json.h>
#include "Foo.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "EventQueue.h"
#include "Event.h"
#include "FooSubscriber.h"
#include "GameTime.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTest)
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

		TEST_METHOD(TestRTTI)
		{
			FooSubscriber s;
			Assert::IsTrue(s.Is(EventSubscriber::TypeIdClass()));

			Event<Foo> e;
			Assert::IsTrue(e.Is(EventPublisher::TypeIdClass()));
		}

		TEST_METHOD(TestConstructor)
		{
			Event<Foo> e1(Foo(5));
			Assert::AreEqual(e1.Message(), Foo(5));

			Foo f;
			Event<Foo> e2(f);
			Assert::AreEqual(e2.Message(), f);
		}

		TEST_METHOD(TestSubscription)
		{
			FooSubscriber s1, s2;
			s1.Subscribe();
			Event<Foo> e1(Foo(5));
			Assert::AreEqual(s1.Data, 0);
			Assert::AreEqual(s2.Data, 0);
			e1.Deliver();
			Assert::AreEqual(s1.Data, 5);
			Assert::AreEqual(s2.Data, 0);

			Event<Foo> e2(Foo(10));
			Event<Foo>::UnsubscribeAll();
			e2.Deliver();
			Assert::AreEqual(s1.Data, 5);
			Assert::AreEqual(s2.Data, 0);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventTest::sStartMemState;
}
