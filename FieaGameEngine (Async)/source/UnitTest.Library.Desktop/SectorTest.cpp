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
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SectorTest)
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
			Sector s;
			Assert::IsTrue(s.Is(Sector::TypeIdClass()));
			Assert::AreSame(s.Name(), s["Name"s].Get<std::string>());
			Assert::AreEqual(s.Name(), ""s);

			Sector sector("sector"s);
			Assert::AreEqual(sector.Name(), "sector"s);
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Sector e1;
			e1.SetName("e1"s);
			Sector e2 = e1;
			Assert::AreEqual(e1, e2);
			e2["Name"s] = "e2"s;
			Assert::AreNotEqual(e1, e2);
		}

		TEST_METHOD(TestClone)
		{
			Sector s;
			s.SetName("Sector 1"s);
			auto p = s.Clone();
			Assert::AreEqual(s, *p);
			(*p)["Name"s] = "Sector 2"s;
			Assert::AreNotEqual(s, *p);

			delete p;
		}

		TEST_METHOD(TestSetWorld)
		{
			std::shared_ptr gt = std::make_shared<GameTime>();
			World w(gt);
			Sector s;
			Assert::AreEqual(w.Sectors().Size(), 0_z);
			Assert::IsNull(s.GetParent<World>());
			s.SetWorld(w);
			Assert::AreEqual(w.Sectors().Size(), 1_z);
			Assert::IsNotNull(s.GetParent<World>());
			Assert::AreEqual(s.GetParent<World>(), &w);
		}

		TEST_METHOD(TestCreateEntity)
		{
			EntityFactory ef;

			Sector s;
			Assert::AreEqual(s.Entities().Size(), 0_z);
			auto e = s.CreateEntity(ef.ClassName(), "Entity 1"s);
			Assert::AreEqual(s.Entities().Size(), 1_z);
			Assert::AreSame(s[Sector::EntitiesKey][0], *e->As<Scope>());

			const Sector s2 = s;
			Assert::AreEqual(s2.Entities().Size(), 1_z);
			Assert::AreNotSame(s2.Entities()[0], *e->As<Scope>());
		}

		TEST_METHOD(TestName)
		{
			Sector s;
			Assert::AreEqual(s.Name(), ""s);
			s.SetName("Sector 1"s);
			Assert::AreEqual(s.Name(), "Sector 1"s);

			const Sector s2 = s;
			Assert::AreEqual(s2.Name(), "Sector 1"s);
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SectorTest::sStartMemState;
}
