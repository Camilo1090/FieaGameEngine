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
#include "ActionIncrement.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace std::chrono_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ReactionTest)
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

		TEST_METHOD(TestReaction)
		{
			GameClock gc;
			std::shared_ptr gt = std::make_shared<GameTime>();
			World w(gt);
			EventQueue eq;
			w.SetEventQueue(eq);

			TableSharedData data(w);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			SectorFactory sf;
			EntityFactory ef;
			ActionEventFactory aef;
			ReactionAttributedFactory raf;
			ActionIncrementFactory aif;

			master.Initialize();
			master.ParseFromFile("content/reaction.json");

			std::int32_t& score = w.Sectors()[0]["Score"s].Get<std::int32_t>();
			Assert::AreEqual(score, 0);

			Assert::IsTrue(eq.IsEmpty());
			gc.UpdateGameTime(*gt);
			w.Update(w.GetWorldState());
			Assert::AreEqual(eq.Size(), 2_z);
			Assert::AreEqual(score, 0);

			gc.UpdateGameTime(*gt);
			w.Update(w.GetWorldState());
			Assert::AreEqual(eq.Size(), 1_z);
			Assert::AreEqual(score, 5);

			gt->SetCurrentTime(gt->CurrentTime() + 5s);
			w.Update(w.GetWorldState());
			Assert::AreEqual(eq.Size(), 0_z);
			Assert::AreEqual(score, 15);
		}

		TEST_METHOD(TestCopyMoveSemantics)
		{
			GameClock gc;
			std::shared_ptr gt = std::make_shared<GameTime>();
			World w(gt);
			EventQueue eq;
			w.SetEventQueue(eq);

			TableSharedData data(w);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			SectorFactory sf;
			EntityFactory ef;
			ActionEventFactory aef;
			ReactionAttributedFactory raf;
			ActionIncrementFactory aif;

			master.Initialize();
			master.ParseFromFile("content/reaction.json");

			World w2 = w;
			Assert::AreEqual(w2, w);
			w2.Append("dummy"s);
			Assert::AreNotEqual(w2, w);

			World w3 = std::move(w2);
			Assert::AreNotEqual(w3, w2);
		}

		TEST_METHOD(TestAccessors)
		{
			GameClock gc;
			std::shared_ptr gt = std::make_shared<GameTime>();
			World w(gt);
			EventQueue eq;
			w.SetEventQueue(eq);

			TableSharedData data(w);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			SectorFactory sf;
			EntityFactory ef;
			ActionEventFactory aef;
			ReactionAttributedFactory raf;
			ActionIncrementFactory aif;

			master.Initialize();
			master.ParseFromFile("content/reaction.json");

			ReactionAttributed* ptr1 = w["Sectors"s][0]["Entities"s][0]["Actions"s][0].As<ReactionAttributed>();
			Assert::IsNotNull(ptr1);

			ActionEvent* ptr2 = w["Sectors"s][0]["Entities"s][0]["Actions"s][1].As<ActionEvent>();
			Assert::IsNotNull(ptr2);

			ActionEvent* ptr3 = w["Sectors"s][0]["Entities"s][0]["Actions"s][2].As<ActionEvent>();
			Assert::IsNotNull(ptr3);

			Assert::AreEqual(ptr1->Subtype(), "test"s);
			Assert::AreEqual(ptr2->Subtype(), "test"s);
			Assert::AreEqual(ptr3->Subtype(), "test"s);

			const ReactionAttributed* p1 = ptr1;
			const ActionEvent* p2 = ptr2;
			const ActionEvent* p3 = ptr3;

			Assert::AreEqual(p1->Subtype(), "test"s);
			Assert::AreEqual(p2->Subtype(), "test"s);
			Assert::AreEqual(p3->Subtype(), "test"s);

			ptr1->SetSubtype("1"s);
			ptr2->SetSubtype("2"s);
			ptr3->SetSubtype("3"s);

			Assert::AreEqual(ptr1->Subtype(), "1"s);
			Assert::AreEqual(ptr2->Subtype(), "2"s);
			Assert::AreEqual(ptr3->Subtype(), "3"s);

			Assert::AreEqual(ptr2->GetDelay(), 0);
			Assert::AreEqual(ptr3->GetDelay(), 5000);

			ptr2->SetDelay(1000);
			ptr3->SetDelay(1500);

			Assert::AreEqual(ptr2->GetDelay(), 1000);
			Assert::AreEqual(ptr3->GetDelay(), 1500);

			EventMessageAttributed ema(w);
			ema.SetWorld(w);
		}

		TEST_METHOD(TestMessage)
		{
			GameClock gc;
			auto gt = std::make_shared<GameTime>();
			World w(gt);
			EventMessageAttributed m(w);

			m.SetSubtype("test"s);
			Assert::AreEqual(m.Subtype(), "test"s);

			const auto p = m.Clone();
			Assert::AreEqual(m.Subtype(), p->Subtype());

			delete p;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ReactionTest::sStartMemState;
}
