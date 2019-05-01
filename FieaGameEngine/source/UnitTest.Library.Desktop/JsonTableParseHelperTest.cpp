#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <sstream>
#include <json/json.h>
#include "Foo.h"
#include "JsonParseMaster.h"
#include "JsonTableParseHelper.h"
#include "JsonDepthHelper.h"
#include "AttributedFoo.h"
#include "TypeManager.h"
#include "ToStringSpecializations.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonTableParseHelperTest)
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

		TEST_METHOD(TestFromString)
		{
			Scope table;
			TableSharedData data(table);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			Assert::AreEqual(table.Size(), 0_z);

			master.Initialize();
			master.Parse(R"({ "Health": { "type": "integer", "value": 100 } })"s);

			Assert::AreEqual(table.Size(), 1_z);
			Assert::AreEqual(table["Health"s].Get<std::int32_t>(), 100);
		}

		TEST_METHOD(TestFromFile)
		{
			Scope table;
			TableSharedData data(table);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			Assert::AreEqual(table.Size(), 0_z);

			master.Initialize();
			master.ParseFromFile("content/json/table.json");

			Assert::AreEqual(table.Size(), 3_z);
			Assert::AreEqual(table["Name"s].Get<std::string>(), "Camilo"s);
			Assert::AreEqual(table["Health"s].Get<std::int32_t>(), 100);
			Assert::AreEqual(table["ATable"s].Size(), 3_z);
			Assert::AreEqual(table["ATable"s][0]["Name"s].Size(), 1_z);
			Assert::AreEqual(table["ATable"s][0]["Name"s].Get<std::string>(), "Laser Sight"s);
			Assert::AreEqual(table["ATable"s][0]["Damage"s].Size(), 1_z);
			Assert::AreEqual(table["ATable"s][0]["Damage"s].Get<float>(), 5.25f);
			Assert::AreEqual(table["ATable"s][1]["Name"s].Size(), 1_z);
			Assert::AreEqual(table["ATable"s][1]["Name"s].Get<std::string>(), "Laser Sight"s);
			Assert::AreEqual(table["ATable"s][1]["Damage"s].Size(), 1_z);
			Assert::AreEqual(table["ATable"s][1]["Damage"s].Get<float>(), 5.25f);
			Assert::AreEqual(table["ATable"s][2]["Name"s].Size(), 1_z);
			Assert::AreEqual(table["ATable"s][2]["Name"s].Get<std::string>(), "Laser Sight"s);
			Assert::AreEqual(table["ATable"s][2]["Damage"s].Size(), 1_z);
			Assert::AreEqual(table["ATable"s][2]["Damage"s].Get<float>(), 5.25f);
		}

		TEST_METHOD(TestAttributedFoo)
		{
			AttributedFoo table;
			TableSharedData data(table);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			master.Initialize();
			master.ParseFromFile("content/json/attributed_foo.json");

			Assert::AreEqual(table.ExternalInteger, 100);
			Assert::AreEqual(table.ExternalFloat, 10.5f);
			Assert::AreEqual(table.ExternalVector, glm::vec4(1,2,3,4));
			Assert::AreEqual(table.ExternalMatrix, glm::mat4(1, 2, 3, 4,5,6,7,8,9,10,11,12,13,14,15,16));
			Assert::AreEqual(table.ExternalString, "Test String"s);

			Assert::AreEqual(table.ExternalIntegerArray[0], 1);
			Assert::AreEqual(table.ExternalIntegerArray[1], 2);
			Assert::AreEqual(table.ExternalIntegerArray[2], 3);
			Assert::AreEqual(table.ExternalIntegerArray[3], 4);
			Assert::AreEqual(table.ExternalIntegerArray[4], 5);

			Assert::AreEqual(table.ExternalFloatArray[0], 1.5f);
			Assert::AreEqual(table.ExternalFloatArray[1], 2.5f);
			Assert::AreEqual(table.ExternalFloatArray[2], 3.5f);
			Assert::AreEqual(table.ExternalFloatArray[3], 4.5f);
			Assert::AreEqual(table.ExternalFloatArray[4], 5.5f);

			Assert::AreEqual(table.ExternalVectorArray[0], glm::vec4(1, 2, 3, 4));
			Assert::AreEqual(table.ExternalVectorArray[1], glm::vec4(1, 2, 3, 4));
			Assert::AreEqual(table.ExternalVectorArray[2], glm::vec4(1, 2, 3, 4));
			Assert::AreEqual(table.ExternalVectorArray[3], glm::vec4(1, 2, 3, 4));
			Assert::AreEqual(table.ExternalVectorArray[4], glm::vec4(1, 2, 3, 4));

			Assert::AreEqual(table.ExternalMatrixArray[0], glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
			Assert::AreEqual(table.ExternalMatrixArray[1], glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
			Assert::AreEqual(table.ExternalMatrixArray[2], glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
			Assert::AreEqual(table.ExternalMatrixArray[3], glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
			Assert::AreEqual(table.ExternalMatrixArray[4], glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));

			Assert::AreEqual(table["NestedScope"s].Size(), 1_z);
			Assert::AreEqual(table["NestedScope"s][0]["Name"s].Get<std::string>(), "Camilo"s);

			Assert::IsNotNull(table.Find("NestedScopes"s));
			Assert::AreEqual(table["NestedScopes"s].Size(), 5_z);
			Assert::AreEqual(table["NestedScopes"s][0]["Name"s].Get<std::string>(), "Camilo"s);
			Assert::AreEqual(table["NestedScopes"s][1]["Name"s].Get<std::string>(), "Erika"s);
			Assert::AreEqual(table["NestedScopes"s][2]["Name"s].Get<std::string>(), "Jesus"s);
			Assert::AreEqual(table["NestedScopes"s][3]["Name"s].Get<std::string>(), "Melissa"s);
			Assert::AreEqual(table["NestedScopes"s][4]["Name"s].Get<std::string>(), "Martha"s);

			Assert::IsNotNull(table.Find("NestedScopeArray"s));
			Assert::AreEqual(table["NestedScopeArray"s].Size(), 5_z);

			Assert::AreEqual(table["NestedScopeArray"s][0]["Name"s].Get<std::string>(), "Camilo1"s);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][0]["Name"s].Get<std::string>(), "Camilo1-1"s);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["Name"s].Get<std::string>(), "Erika1-2"s);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][0]["Integers"s].Get<std::int32_t>(0), 1);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][0]["Integers"s].Get<std::int32_t>(1), 2);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][0]["Integers"s].Get<std::int32_t>(2), 3);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][0]["Integers"s].Get<std::int32_t>(3), 4);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][0]["Integers"s].Get<std::int32_t>(4), 5);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][1]["Integers"s].Get<std::int32_t>(0), 6);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][1]["Integers"s].Get<std::int32_t>(1), 7);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][1]["Integers"s].Get<std::int32_t>(2), 8);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][1]["Integers"s].Get<std::int32_t>(3), 9);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][1]["MoarNestedScopes"s][1]["Integers"s].Get<std::int32_t>(4), 10);
			Assert::AreEqual(table["NestedScopeArray"s][0]["EvenMoreScopes"s][2]["Name"s].Get<std::string>(), "Jesus1-3"s);
			Assert::AreEqual(table["NestedScopeArray"s][1]["Name"s].Get<std::string>(), "Erika"s);
			Assert::AreEqual(table["NestedScopeArray"s][2]["Name"s].Get<std::string>(), "Jesus"s);
			Assert::AreEqual(table["NestedScopeArray"s][3]["Name"s].Get<std::string>(), "Melissa"s);
			Assert::AreEqual(table["NestedScopeArray"s][4]["Name"s].Get<std::string>(), "Martha"s);
		}

		TEST_METHOD(TestClone)
		{
			Scope table;
			TableSharedData data(table);
			JsonParseMaster master(data);
			JsonTableParseHelper helper;
			master.AddHelper(helper);

			Assert::AreEqual(table.Size(), 0_z);

			master.Initialize();
			master.Parse(R"({ "Health": { "type": "integer", "value": 100 } })"s);

			Assert::AreEqual(table.Size(), 1_z);
			Assert::AreEqual(table["Health"s].Get<std::int32_t>(), 100);

			JsonParseMaster* clone = master.Clone();
			clone->Initialize();
			clone->Parse(R"({ "Damage": { "type": "float", "value": 1.5 } })"s);

			Assert::AreEqual(clone->GetSharedData()->As<TableSharedData>()->GetTable().Size(), 1_z);
			Assert::AreEqual(clone->GetSharedData()->As<TableSharedData>()->GetTable()["Damage"s].Get<float>(), 1.5f);

			delete clone;
		}

		TEST_METHOD(TestMultipleHelpers)
		{
			JsonDepthHelper::DepthSharedData data;
			JsonParseMaster master(data);
			JsonTableParseHelper tableHelper;
			JsonDepthHelper depthHelper;
			master.AddHelper(tableHelper);
			master.AddHelper(depthHelper);

			master.Initialize();
			master.ParseFromFile("content/json/table.json"s);

			Assert::AreEqual(data.Depth(), 0_z);
			Assert::AreEqual(data.MaxDepth, 4_z);
			Assert::AreEqual(depthHelper.StartHandlerCount, 32_z);
			Assert::AreEqual(depthHelper.EndHandlerCount, 32_z);

			Assert::IsTrue(master.RemoveHelper(tableHelper));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonTableParseHelperTest::sStartMemState;
}
