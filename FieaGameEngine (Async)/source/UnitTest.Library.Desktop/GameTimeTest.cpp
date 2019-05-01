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
#include "GameClock.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(GameTimeTest)
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

		TEST_METHOD(TestGameTime)
		{
			GameTime gt;
			GameClock gc;

			gt.CurrentTime();
			gt.ElapsedGameTime();
			gt.ElapsedGameTimeSeconds();
			gt.TotalGameTime();
			gt.TotalGameTimeSeconds();

			gc.StartTime();
			gc.CurrentTime();
			gc.LastTime();
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState GameTimeTest::sStartMemState;
}
