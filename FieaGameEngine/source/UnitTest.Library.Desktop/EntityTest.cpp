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
#include "ActionListIf.h"
#include "ActionIncrement.h"
#include "GameTime.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EntityTest)
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
			Entity entity;
			Assert::IsTrue(entity.Is(Entity::TypeIdClass()));
			Assert::AreSame(entity.Name(), entity["Name"s].Get<std::string>());
			Assert::AreEqual(entity.Name(), ""s);

			Avatar a("avatar"s, glm::vec4(10, 10, 10, 10));
			Assert::AreEqual(a["Name"s].Get<std::string>(), "avatar"s);

			Entity e("test"s);
			Assert::AreEqual(e.Name(), "test"s);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Entity e1;
			e1.SetName("e1"s);
			Entity e2 = e1;
			Assert::AreEqual(e1, e2);
			e2["Name"s] = "e2"s;
			Assert::AreNotEqual(e1, e2);

			Avatar a1;
			a1.SetName("a1"s);
			Avatar a2 = a1;
			Assert::AreEqual(a1, a2);
			a2["Position"s] = glm::vec4(213, 3123, 32213, 3);
			Assert::AreNotEqual(a1, a2);
		}

		TEST_METHOD(TestClone)
		{
			Entity entity;
			entity.SetName("Entity 1"s);
			auto p = entity.Clone();
			Assert::AreEqual(entity, *p);
			(*p)["Name"s] = "Entity 2"s;
			Assert::AreNotEqual(entity, *p);

			delete p;
		}

		TEST_METHOD(TestUpdate)
		{
			Entity e;
			TableSharedData data(e);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			EntityFactory f1;
			ActionListFactory f2;
			ActionListIfFactory f3;
			ActionIncrementFactory f4;

			master.Initialize();
			master.ParseFromFile("content/entity.json");

			auto& a = e["A"s].Get<std::int32_t>();
			auto& b = e["B"s].Get<float>();
			auto& c = e["C"s].Get<std::int32_t>();

			Assert::AreEqual(a, 0);
			Assert::AreEqual(b, 0.5f);
			Assert::AreEqual(c, 0);

			std::shared_ptr gt = std::make_shared<GameTime>();
			WorldState ws(gt);
			e.Update(ws);

			Assert::AreEqual(a, 1);
			Assert::AreEqual(b, 1.5f);
			Assert::AreEqual(c, 0);

			e.Update(ws);

			Assert::AreEqual(a, 2);
			Assert::AreEqual(b, 2.5f);
			Assert::AreEqual(c, 1);
		}

		TEST_METHOD(TestCreateAction)
		{
			ActionListFactory f1;
			ActionIncrementFactory f2;

			Entity e;
			Assert::AreEqual(e.Actions().Size(), 0_z);
			auto a = e.CreateAction(f1.ClassName(), "Action 1"s);
			Assert::AreEqual(e.Actions().Size(), 1_z);
			Assert::AreSame(e[Entity::ActionsKey][0], *a->As<Scope>());

			const Entity e2 = e;
			Assert::AreEqual(e2.Actions().Size(), 1_z);
			Assert::AreNotSame(e2.Actions()[0], *a->As<Scope>());

			Avatar av;
			auto ava = av.CreateAction(f2.ClassName(), "Avatar Action"s);
			Assert::AreEqual(av.Actions().Size(), 1_z);
			Assert::AreSame(av[Entity::ActionsKey][0], *ava->As<Scope>());
		}

		TEST_METHOD(TestSetSector)
		{
			Sector s;
			Entity e;
			Assert::IsNull(e.GetParent<Sector>());
			Assert::AreEqual(s.Entities().Size(), 0_z);
			e.SetSector(s);
			Assert::AreEqual(s.Entities().Size(), 1_z);
			Assert::IsNotNull(e.GetParent<Sector>());
			Assert::AreEqual(e.GetParent<Sector>(), &s);
		}

		TEST_METHOD(TestName)
		{
			Entity e;
			Assert::IsTrue(e.Name().empty());
			e.SetName("Test"s);
			Assert::AreEqual(e.Name(), "Test"s);

			const Entity e2 = e;
			Assert::AreEqual(e2.Name(), "Test"s);
		}

		TEST_METHOD(TestDeserialization)
		{
			Sector sector;
			TableSharedData data(sector);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			EntityFactory entityFactory;
			AvatarFactory avatarFactory;

			master.Initialize();
			master.ParseFromFile("content/sector.json");

			Assert::AreEqual(sector.Entities().Size(), 2_z);
			Assert::IsTrue(sector[Sector::EntitiesKey][0].Is(Entity::TypeIdClass()));
			Assert::AreEqual(sector[Sector::EntitiesKey][0]["Name"s].Get<std::string>(), "Entity 1"s);
			Avatar* avatar = sector[Sector::EntitiesKey][1].As<Avatar>();
			Assert::IsNotNull(avatar);
			Assert::AreEqual(avatar->Name(), "Avatar 1"s);
			Assert::AreEqual(avatar->Position, glm::vec4(1,2,3,0));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState EntityTest::sStartMemState;
}
