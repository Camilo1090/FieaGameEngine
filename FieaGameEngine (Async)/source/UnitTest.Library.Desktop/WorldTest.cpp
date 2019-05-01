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
#include "GameTime.h"
#include "EventQueue.h"
#include "WorldState.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(WorldTest)
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
			std::shared_ptr gameTime = std::make_shared<GameTime>();
			World world(gameTime);
			Assert::AreEqual(world.Name(), ""s);
			Assert::AreSame(world.Name(), world["Name"s].Get<std::string>());
			Assert::AreEqual(world.Sectors().Size(), 0_z);

			World w(gameTime, "world"s);
			Assert::AreEqual(w.Name(), "world"s);
		}

		TEST_METHOD(TestCopySemantics)
		{
			std::shared_ptr gt = std::make_shared<GameTime>();
			World w1(gt);
			TableSharedData data(w1);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			SectorFactory sectorFactory;
			EntityFactory entityFactory;

			master.Initialize();
			master.ParseFromFile("content/world.json");

			World w2 = w1;
			Assert::AreEqual(w1, w2);
			w2.SetName("w2"s);
			Assert::AreNotEqual(w1, w2);

			World w3(gt);
			w3 = w2;
			Assert::AreEqual(w2, w3);
			w3.SetName("w3"s);
			Assert::AreNotEqual(w2, w3);
		}

		TEST_METHOD(TestMoveSemantics)
		{
			std::shared_ptr gt = std::make_shared<GameTime>();
			World w1(gt);
			TableSharedData data(w1);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			SectorFactory sectorFactory;
			EntityFactory entityFactory;

			master.Initialize();
			master.ParseFromFile("content/world.json");

			World w2(std::move(w1));
			Assert::AreNotEqual(w2, w1);
		}

		TEST_METHOD(TestClone)
		{
			SectorFactory sf;

			std::shared_ptr gt = std::make_shared<GameTime>();
			World w1(gt);
			auto w2 = w1.Clone();
			Assert::IsNotNull(w2);
			Assert::AreEqual(w1, *w2);
			w2->CreateSector("s1"s);
			Assert::AreNotEqual(w1, *w2);

			delete w2;
		}

		TEST_METHOD(TestUpdate)
		{
			SectorFactory sf;
			EntityFactory ef;
			AvatarFactory af;

			GameClock gc;

			std::shared_ptr gt = std::make_shared<GameTime>();
			World w1(gt);
			EventQueue q;
			w1.SetEventQueue(q);
			auto first = w1.GetWorldState().GameTime->CurrentTime();
			auto sector = w1.CreateSector("sector"s);
			auto avatar = sector->CreateEntity(af.ClassName(), "avatar"s);
			Assert::AreEqual(avatar->Name(), "avatar"s);
			w1.Update(w1.GetWorldState());
			gc.UpdateGameTime(*gt);
			auto second = w1.GetWorldState().GameTime->CurrentTime();
			Assert::IsTrue(second > first);

			const World w2 = w1;
			Assert::IsTrue(w2.GetWorldState().GameTime->CurrentTime() == second);
		}

		TEST_METHOD(TestCreateSector)
		{
			SectorFactory sf;

			std::shared_ptr gameTime = std::make_shared<GameTime>();
			World world(gameTime);
			Assert::AreEqual(world.Sectors().Size(), 0_z);
			auto p = world.CreateSector("Sector 1"s);
			Assert::AreEqual(world.Sectors().Size(), 1_z);
			Assert::AreSame(world[World::SectorsKey][0], *p->As<Scope>());

			const World w(gameTime);
			Assert::AreEqual(w.Sectors().Size(), 0_z);
		}

		TEST_METHOD(TestName)
		{
			std::shared_ptr gameTime = std::make_shared<GameTime>();
			World world(gameTime);
			Assert::AreEqual(world.Name(), ""s);
			world.SetName("World 1"s);
			Assert::AreEqual(world.Name(), "World 1"s);

			const World w(gameTime);
			Assert::AreEqual(w.Name(), ""s);
		}

		TEST_METHOD(TestWorldDeserialization)
		{
			std::shared_ptr gameTime = std::make_shared<GameTime>();
			World world(gameTime);
			TableSharedData data(world);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			//WorldFactory worldFactory;
			SectorFactory sectorFactory;
			EntityFactory entityFactory;

			master.Initialize();
			master.ParseFromFile("content/world.json");

			Assert::AreEqual(world.Name(), "Test World"s);
			Assert::AreEqual(world.Sectors().Size(), 3_z);
			Assert::IsTrue(world[World::SectorsKey][0].Is(Sector::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][0]["Name"s].Get<std::string>(), "Sector 1"s);
			Assert::AreEqual(world[World::SectorsKey][0][Sector::EntitiesKey].Size(), 1_z);
			Assert::IsTrue(world[World::SectorsKey][0][Sector::EntitiesKey][0].Is(Entity::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][0][Sector::EntitiesKey][0]["Name"s].Get<std::string>(), "Sector 1 - Entity 1"s);
			Assert::IsTrue(world[World::SectorsKey][1].Is(Sector::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][1]["Name"s].Get<std::string>(), "Sector 2"s);
			Assert::AreEqual(world[World::SectorsKey][1][Sector::EntitiesKey].Size(), 2_z);
			Assert::IsTrue(world[World::SectorsKey][1][Sector::EntitiesKey][0].Is(Entity::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][1][Sector::EntitiesKey][0]["Name"s].Get<std::string>(), "Sector 2 - Entity 1"s);
			Assert::IsTrue(world[World::SectorsKey][1][Sector::EntitiesKey][1].Is(Entity::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][1][Sector::EntitiesKey][1]["Name"s].Get<std::string>(), "Sector 2 - Entity 2"s);
			Assert::IsTrue(world[World::SectorsKey][2].Is(Sector::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][2]["Name"s].Get<std::string>(), "Sector 3"s);
			Assert::AreEqual(world[World::SectorsKey][2][Sector::EntitiesKey].Size(), 3_z);
			Assert::IsTrue(world[World::SectorsKey][2][Sector::EntitiesKey][0].Is(Entity::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][2][Sector::EntitiesKey][0]["Name"s].Get<std::string>(), "Sector 3 - Entity 1"s);
			Assert::IsTrue(world[World::SectorsKey][2][Sector::EntitiesKey][1].Is(Entity::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][2][Sector::EntitiesKey][1]["Name"s].Get<std::string>(), "Sector 3 - Entity 2"s);
			Assert::IsTrue(world[World::SectorsKey][2][Sector::EntitiesKey][2].Is(Entity::TypeIdClass()));
			Assert::AreEqual(world[World::SectorsKey][2][Sector::EntitiesKey][2]["Name"s].Get<std::string>(), "Sector 3 - Entity 3"s);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState WorldTest::sStartMemState;
}
