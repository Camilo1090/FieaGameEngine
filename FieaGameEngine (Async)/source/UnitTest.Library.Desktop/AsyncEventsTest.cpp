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
	TEST_CLASS(AsyncEventsTest)
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

		TEST_METHOD(TestAsync)
		{
			auto gt = std::make_shared<GameTime>();
			World w(gt);
			EventQueue queue;
			w.SetEventQueue(queue);
			GameClock gc;
			gc.UpdateGameTime(*gt);

			{
				const size_t count = 20;
				for (size_t i = 0; i < count; ++i)
				{
					auto e1 = std::make_shared<Event<EventMessageAttributed>>(EventMessageAttributed(w));
					queue.EnqueueEvent(e1, *gt);
				}

				EnqueueEventSubscriber s[count];
				//gc.UpdateGameTime(*gt);
				gt->SetCurrentTime(gt->CurrentTime() + 1ms);
				w.Update(w.GetWorldState());
				Assert::AreEqual(queue.Size(), count * count);

				for (size_t i = 0; i < count; ++i)
				{
					Assert::AreEqual(static_cast<size_t>(s[i].Notifications), count);
				}

				queue.Clear();
			}

			{
				const size_t count = 10;
				for (size_t i = 0; i < count; ++i)
				{
					auto e1 = std::make_shared<Event<EventMessageAttributed>>(EventMessageAttributed(w));
					queue.EnqueueEvent(e1, *gt);
				}
				Assert::AreEqual(queue.Size(), count);

				const size_t subs = 40;
				EnqueueEventSubscriber s[subs];
				for (size_t i = 0; i < subs; ++i)
				{
					s[i].Unsubscribe = true;
				}
				gt->SetCurrentTime(gt->CurrentTime() + 1ms);
				w.Update(w.GetWorldState());
				Assert::IsTrue(queue.Size() <= count * subs);

				for (const auto& sub : s)
				{
					Assert::IsTrue(static_cast<size_t>(sub.Notifications) <= count);
				}

				gt->SetCurrentTime(gt->CurrentTime() + 1ms);
				w.Update(w.GetWorldState());
				Assert::AreEqual(queue.Size(), 0_z);

				for (const auto& sub : s)
				{
					Assert::IsTrue(static_cast<size_t>(sub.Notifications) <= count);
				}
			}

			{
				const size_t count = 10;
				for (size_t i = 0; i < count; ++i)
				{
					auto e1 = std::make_shared<Event<EventMessageAttributed>>(EventMessageAttributed(w));
					queue.EnqueueEvent(e1, *gt);
				}
				Assert::AreEqual(queue.Size(), count);

				const size_t subs = 40;
				EnqueueEventSubscriber s[subs];
				for (size_t i = 0; i < subs; ++i)
				{
					s[i].Enqueue = false;
					s[i].Unsubscribe = true;
					s[i].SubscribeAgain = true;
				}
				gt->SetCurrentTime(gt->CurrentTime() + 1ms);
				w.Update(w.GetWorldState());
				Assert::IsTrue(queue.Size() <= count * subs);

				for (const auto& sub : s)
				{
					Assert::IsTrue(static_cast<size_t>(sub.Notifications) <= count);
				}

				gt->SetCurrentTime(gt->CurrentTime() + 1ms);
				w.Update(w.GetWorldState());
				Assert::AreEqual(queue.Size(), 0_z);

				for (const auto& sub : s)
				{
					Assert::IsTrue(static_cast<size_t>(sub.Notifications) <= count);
				}
			}

			{
				const size_t count = 10;
				for (size_t i = 0; i < count; ++i)
				{
					auto e1 = std::make_shared<Event<EventMessageAttributed>>(EventMessageAttributed(w));
					queue.EnqueueEvent(e1, *gt);
				}
				Assert::AreEqual(queue.Size(), count);

				const size_t subs = 40;
				EnqueueEventSubscriber s[subs];
				for (size_t i = 0; i < subs; ++i)
				{
					s[i].Enqueue = false;
					s[i].Unsubscribe = false;
					s[i].SubscribeAgain = false;
					s[i].Throw = true;
				}
				gt->SetCurrentTime(gt->CurrentTime() + 1ms);
				Assert::ExpectException<std::exception>([&w]() { w.Update(w.GetWorldState()); });
			}
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState AsyncEventsTest::sStartMemState;
}
