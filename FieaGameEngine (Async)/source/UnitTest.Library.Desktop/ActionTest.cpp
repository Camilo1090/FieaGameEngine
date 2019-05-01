#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <sstream>
#include <json/json.h>
#include "Foo.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "TypeManager.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Avatar.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "ActionListIf.h"
#include "GameTime.h"
#include "ActionCreateAction.h"
#include "ActionDeleteAction.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionTest)
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
			{
				ActionList a;
				Assert::IsTrue(a.Is(Action::TypeIdClass()));
				Assert::AreSame(a.Name(), a["Name"s].Get<std::string>());
				Assert::AreEqual(a.Name(), ""s);

				ActionList aa("test"s);
				Assert::AreEqual(aa.Name(), "test"s);
			}

			{
				ActionListIf a;
				Assert::IsTrue(a.Is(Action::TypeIdClass()));
				Assert::AreSame(a.Name(), a["Name"s].Get<std::string>());
				Assert::AreEqual(a.Name(), ""s);

				ActionListIf aa("test"s);
				Assert::AreEqual(aa.Name(), "test"s);
			}

			{
				ActionIncrement a;
				Assert::IsTrue(a.Is(Action::TypeIdClass()));
				Assert::AreSame(a.Name(), a["Name"s].Get<std::string>());
				Assert::AreEqual(a.Name(), ""s);

				ActionIncrement aa("test"s);
				Assert::AreEqual(aa.Name(), "test"s);
			}
		}

		TEST_METHOD(TestCopyConstructor)
		{
			{
				ActionList a1;
				a1.Adopt(ActionList::ActionsKey, *(new ActionIncrement()));
				ActionList a2 = a1;
				Assert::IsTrue(a1 == a2);
				a2.Adopt(ActionList::ActionsKey, *(new ActionList()));
				Assert::IsTrue(a1 != a2);

				const auto a3 = a2;
				Assert::AreEqual(a3.Actions(), a2.Actions());
			}

			{
				ActionListIf a1;
				a1.Adopt(ActionList::ActionsKey, *(new ActionIncrement()));
				ActionListIf a2 = a1;
				Assert::IsTrue(a1 == a2);
				a2.Adopt(ActionList::ActionsKey, *(new ActionList()));
				Assert::IsTrue(a1 != a2);
			}

			{
				ActionIncrement a1;
				a1.Name() = "a1";
				ActionIncrement a2 = a1;
				Assert::IsTrue(a1 == a2);
				a2.Name() = "a2";
				Assert::IsTrue(a1 != a2);
			}
		}

		TEST_METHOD(TestClone)
		{
			{
				ActionList a;
				a.SetName("a"s);
				auto p = a.Clone();
				Assert::IsTrue(a == *p);
				(*p)["Name"s] = "a2"s;
				Assert::IsTrue(a != *p);

				delete p;
			}

			{
				ActionListIf a;
				a.SetName("a"s);
				auto p = a.Clone();
				Assert::IsTrue(a == *p);
				(*p)["Name"s] = "a2"s;
				Assert::IsTrue(a != *p);

				delete p;
			}

			{
				ActionIncrement a;
				a.SetName("a"s);
				auto p = a.Clone();
				Assert::IsTrue(a == *p);
				(*p)["Name"s] = "a2"s;
				Assert::IsTrue(a != *p);

				delete p;
			}
		}

		TEST_METHOD(TestName)
		{
			{
				ActionList a;
				Assert::IsTrue(a.Name().empty());
				a.SetName("Test"s);
				Assert::AreEqual(a.Name(), "Test"s);

				const ActionList a2 = a;
				Assert::AreEqual(a2.Name(), "Test"s);
			}

			{
				ActionListIf a;
				Assert::IsTrue(a.Name().empty());
				a.SetName("Test"s);
				Assert::AreEqual(a.Name(), "Test"s);

				const ActionListIf a2 = a;
				Assert::AreEqual(a2.Name(), "Test"s);
			}

			{
				ActionIncrement a;
				Assert::IsTrue(a.Name().empty());
				a.SetName("Test"s);
				Assert::AreEqual(a.Name(), "Test"s);

				const ActionIncrement a2 = a;
				Assert::AreEqual(a2.Name(), "Test"s);
			}
		}

		TEST_METHOD(TestActionIncrement)
		{
			auto gt = std::make_shared<GameTime>();
			World w(gt);
			ActionIncrement ai;
			Assert::ExpectException<std::exception>([&ai, &w]() { ai.Update(w.GetWorldState()); });

			auto& score = ai.Append("Target"s, Datum(""s));
			Assert::ExpectException<std::exception>([&ai, &w]() { ai.Update(w.GetWorldState()); });

			score = "Score"s;
			ai.Append("Score"s, Datum("error"s));
			Assert::ExpectException<std::exception>([&ai, &w]() { ai.Update(w.GetWorldState()); });
		}

		TEST_METHOD(TestCreateDeleteAction)
		{
			GameClock gc;
			auto gt = std::make_shared<GameTime>();
			World w(gt);
			EventQueue eq;
			w.SetEventQueue(eq);

			TableSharedData data(w);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			SectorFactory sectorFactory;
			EntityFactory entityFactory;
			ActionCreateActionFactory acaf;
			ActionDeleteActionFactory adaf;
			ActionIncrementFactory aif;

			master.Initialize();
			master.ParseFromFile("content/actions.json");

			{
				World w2 = w;
				Assert::AreEqual(w2, w);

				World w3 = std::move(w2);
				Assert::AreNotEqual(w2, w3);
			}

			const auto& score = w["Score"s].Get<float>();
			Assert::AreEqual(score, 0.0f);
			Assert::AreEqual(w["Sectors"s][0]["Entities"s][0]["Actions"s].Size(), 2_z);

			w.Update(w.GetWorldState());
			Assert::AreEqual(score, 0.0f);
			Assert::AreEqual(w["Sectors"s][0]["Entities"s][0]["Actions"s].Size(), 2_z);

			{
				World w2 = w;
				Assert::AreEqual(w2, w);

				World w3 = std::move(w2);
				Assert::AreNotEqual(w2, w3);
			}

			w.Update(w.GetWorldState());
			Assert::AreEqual(score, 5.5f);
			Assert::AreEqual(w["Sectors"s][0]["Entities"s][0]["Actions"s].Size(), 0_z);
		}

		TEST_METHOD(TestMisc)
		{
			ActionCreateAction aca("ActionIncrement"s, "increment"s, "create"s);
			Assert::AreEqual(aca.Prototype(), "ActionIncrement"s);
			Assert::AreEqual(aca.ActionName(), "increment"s);

			ActionDeleteAction ada("increment", "delete");
			Assert::AreEqual(ada.ActionName(), "increment"s);

			const auto a1 = aca;
			Assert::AreEqual(a1.Prototype(), "ActionIncrement"s);
			Assert::AreEqual(a1.ActionName(), "increment"s);

			const auto a2 = ada;
			Assert::AreEqual(a2.ActionName(), "increment"s);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState ActionTest::sStartMemState;
}
