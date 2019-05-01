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
#include "World.h"
#include "EventMessageAttributed.h"
#include "EnqueueEventSubscriber.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace std::chrono_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventQueueTest)
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

		TEST_METHOD(TestQueue)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			Assert::IsTrue(queue.IsEmpty());
			Assert::AreEqual(queue.Size(), 0_z);

			{
				std::shared_ptr<Event<Foo>> e1 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time);

				Assert::IsFalse(queue.IsEmpty());
				Assert::AreEqual(queue.Size(), 1_z);

				FooSubscriber s1;
				s1.Subscribe();
				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 0);

				Assert::IsFalse(queue.IsEmpty());
				Assert::AreEqual(queue.Size(), 1_z);

				time.SetCurrentTime(time.CurrentTime() + 1s);
				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 5);

				Assert::IsTrue(queue.IsEmpty());
				Assert::AreEqual(queue.Size(), 0_z);
			}

			{
				std::shared_ptr<Event<Foo>> e1 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time, 5s);
				Assert::AreEqual(queue.Size(), 1_z);

				FooSubscriber s1;
				s1.Subscribe();
				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 0);

				time.SetCurrentTime(time.CurrentTime() + 5s);
				queue.EnqueueEvent(e1, time, 1s);
				Assert::AreEqual(queue.Size(), 1_z);

				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 0);

				time.SetCurrentTime(time.CurrentTime() + 1001ms);

				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 5);

				Assert::AreEqual(queue.Size(), 0_z);
			}

			{
				std::shared_ptr<Event<Foo>> e1 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time);

				std::shared_ptr<Event<Foo>> e2 = std::make_shared<Event<Foo>>(Foo(10));
				queue.EnqueueEvent(e2, time, 5s);

				Assert::IsFalse(queue.IsEmpty());
				Assert::AreEqual(queue.Size(), 2_z);

				FooSubscriber s1;
				s1.Subscribe();
				Assert::AreEqual(s1.Data, 0);
				time.SetCurrentTime(time.CurrentTime() + 1ms);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 5);

				Assert::IsFalse(queue.IsEmpty());
				Assert::AreEqual(queue.Size(), 1_z);

				time.SetCurrentTime(time.CurrentTime() + 6s);

				Assert::AreEqual(s1.Data, 5);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 10);

				Assert::IsTrue(queue.IsEmpty());
				Assert::AreEqual(queue.Size(), 0_z);
			}
		}

		TEST_METHOD(TestEnqueueWhileDelivering)
		{
			auto gt = std::make_shared<GameTime>();
			World w(gt);
			EventQueue queue;
			w.SetEventQueue(queue);
			GameClock gc;
			gc.UpdateGameTime(*gt);

			auto e1 = std::make_shared<Event<EventMessageAttributed>>(EventMessageAttributed(w));
			Assert::AreEqual(queue.Size(), 0_z);
			queue.EnqueueEvent(e1, *gt);
			Assert::AreEqual(queue.Size(), 1_z);

			EnqueueEventSubscriber s1;
			Assert::AreEqual(s1.Notifications, 0);
			gc.UpdateGameTime(*gt);
			w.Update(w.GetWorldState());
			Assert::AreEqual(s1.Notifications, 1);
			Assert::AreEqual(queue.Size(), 1_z);
			gc.UpdateGameTime(*gt);
			w.Update(w.GetWorldState());
			Assert::AreEqual(s1.Notifications, 2);
			Assert::AreEqual(queue.Size(), 1_z);

			queue.EnqueueEvent(e1, *gt);
			Assert::AreEqual(queue.Size(), 2_z);
			gc.UpdateGameTime(*gt);
			w.Update(w.GetWorldState());
			Assert::AreEqual(s1.Notifications, 4);
			Assert::AreEqual(queue.Size(), 2_z);

			queue.EnqueueEvent(e1, *gt, 5s);
			Assert::AreEqual(queue.Size(), 3_z);
			gc.UpdateGameTime(*gt);
			w.Update(w.GetWorldState());
			Assert::AreEqual(s1.Notifications, 6);
			Assert::AreEqual(queue.Size(), 3_z);

			gt->SetCurrentTime(gt->CurrentTime() + 5001ms);
			w.Update(w.GetWorldState());
			Assert::AreEqual(s1.Notifications, 9);
			Assert::AreEqual(queue.Size(), 3_z);
		}

		TEST_METHOD(TestUpdateEvent)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			{
				std::shared_ptr<Event<Foo>> e1 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time, 5s);

				FooSubscriber s1;
				s1.Subscribe();

				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 0);

				time.SetCurrentTime(time.CurrentTime() + 6s);

				queue.UpdateEvent(e1, 10s);

				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 0);

				queue.UpdateEvent(e1, 1s);

				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 5);
			}
		}

		TEST_METHOD(TestRemoveEvent)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			{
				std::shared_ptr<Event<Foo>> e1 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time, 5s);

				FooSubscriber s1;
				s1.Subscribe();

				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 0);

				time.SetCurrentTime(time.CurrentTime() + 5001ms);

				Assert::IsTrue(queue.RemoveEvent(e1));

				Assert::AreEqual(s1.Data, 0);
				queue.Update(time);
				Assert::AreEqual(s1.Data, 0);

				std::shared_ptr<Event<Foo>> e2 = std::make_shared<Event<Foo>>(Foo(10));
				Assert::IsFalse(queue.RemoveEvent(e2));
			}
		}

		TEST_METHOD(TestClear)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			{
				std::shared_ptr<Event<Foo>> e1 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time);

				FooSubscriber s1;
				s1.Subscribe();

				Assert::AreEqual(s1.Data, 0);
				time.SetCurrentTime(time.CurrentTime() + 1ms);
				queue.Clear();
				queue.Update(time);
				Assert::AreEqual(s1.Data, 0);
			}
		}

		TEST_METHOD(TestShrink)
		{
			GameTime time;
			time.SetCurrentTime(std::chrono::high_resolution_clock::now());
			EventQueue queue;

			{
				std::shared_ptr<Event<Foo>> e1 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time);
				std::shared_ptr<Event<Foo>> e2 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time);
				std::shared_ptr<Event<Foo>> e3 = std::make_shared<Event<Foo>>(Foo(5));
				queue.EnqueueEvent(e1, time, 5s);

				FooSubscriber s1;
				s1.Subscribe();
				queue.Update(time);

				time.SetCurrentTime(time.CurrentTime() + 6s);
				queue.ShrinkToFit();
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EventQueueTest::sStartMemState;
}
