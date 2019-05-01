#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <sstream>
#include <json/json.h>
#include "Foo.h"
#include "JsonParseMaster.h"
#include "JsonDepthHelper.h"
#include "JsonIntHelper.h"
#include "ToStringSpecializations.h"
#include "Utility.h"
#include "DefaultComparator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParseMasterTest)
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
			JsonDepthHelper s;
			RTTI* p = &s;
			Assert::IsTrue(p->Is(JsonDepthHelper::TypeName()));
			Assert::IsFalse(p->Is(Foo::TypeName()));
			Assert::IsNotNull(p->As<JsonDepthHelper>());
			Assert::IsNull(p->As<Foo>());
			Assert::IsTrue(p->Is(JsonDepthHelper::TypeIdClass()));
			Assert::IsFalse(p->Is(Foo::TypeIdClass()));
		}

		TEST_METHOD(TestDepthWithString)
		{
			{
				const std::string string = R"({ "Health": { "type": "integer", "value": 100 } })"s;
				JsonDepthHelper::DepthSharedData data;
				JsonParseMaster master(data);
				JsonDepthHelper helper;
				master.AddHelper(helper);

				master.Initialize();
				master.Parse(string);

				Assert::AreEqual(data.MaxDepth, 1_z);
				Assert::AreEqual(data.Depth(), 0_z);
				Assert::IsTrue(helper.InitializeCalled);
				Assert::AreEqual(helper.StartHandlerCount, 3_z);
				Assert::AreEqual(helper.EndHandlerCount, 3_z);
			}

			{
				const std::string string = R"({ "Health": { "type": "integer", "value": 100 } })"s;
				JsonDepthHelper::DepthSharedData data;
				JsonParseMaster master(data);
				JsonDepthHelper helper;
				master.AddHelper(helper);

				master.Initialize();
				Assert::ExpectException<std::exception>([&master] { master.Parse(R"([1, 2, 3])"s); });
			}

			{
				const std::string string = R"({ "Health": { "type": "integer", "value": 100 } })"s;
				JsonDepthHelper::DepthSharedData data;
				JsonParseMaster master(data);
				//JsonDepthHelper helper;
				//master.AddHelper(helper);

				master.Initialize();
				Assert::ExpectException<std::exception>([&master, &string] { master.Parse(string); });
			}
		}

		TEST_METHOD(TestDepthWithFile)
		{
			{
				JsonDepthHelper::DepthSharedData data;
				JsonParseMaster master(data);
				JsonDepthHelper helper;
				master.AddHelper(helper);

				master.Initialize();
				master.ParseFromFile("content/json/depth.json");

				Assert::AreEqual(data.MaxDepth, 5_z);
				Assert::AreEqual(data.Depth(), 0_z);
				Assert::IsTrue(helper.InitializeCalled);
				Assert::AreEqual(helper.StartHandlerCount, 16_z);
				Assert::AreEqual(helper.EndHandlerCount, 16_z);

				// Get file
				Assert::AreEqual(master.GetFileName(), "content/json/depth.json"s);
			}

			{
				JsonDepthHelper::DepthSharedData data;
				JsonParseMaster master(data);
				JsonDepthHelper helper;
				master.AddHelper(helper);

				master.Initialize();
				Assert::ExpectException<std::exception>([&master] { master.ParseFromFile("error"); });
			}
		}

		TEST_METHOD(TestIntParse)
		{
			JsonIntHelper::IntSharedData data;
			JsonParseMaster master(data);
			JsonIntHelper helper;
			master.AddHelper(helper);

			master.Initialize();
			master.Parse(R"({ "integer": 100 })"s);

			Assert::AreEqual(data.Data, 100);
			Assert::AreEqual(data.Depth(), 0_z);
		}

		TEST_METHOD(TestConstructor)
		{
			JsonDepthHelper::DepthSharedData data;
			Assert::IsNull(data.GetJsonParseMaster());
			JsonParseMaster master(data);
			Assert::IsNotNull(data.GetJsonParseMaster());
			Assert::AreEqual(data.GetJsonParseMaster(), &master);
		}

		TEST_METHOD(TestMoveContructor)
		{
			JsonDepthHelper::DepthSharedData data;
			JsonParseMaster master(data);
			Assert::AreEqual(data.GetJsonParseMaster(), &master);
			JsonParseMaster m2 = std::move(master);
			Assert::AreEqual(data.GetJsonParseMaster(), &m2);
		}

		TEST_METHOD(TestMoveAssignment)
		{
			JsonDepthHelper::DepthSharedData data;
			JsonParseMaster master(data);
			Assert::AreEqual(data.GetJsonParseMaster(), &master);
			JsonDepthHelper::DepthSharedData data2;
			JsonParseMaster m2(data2);
			JsonDepthHelper h1;
			JsonIntHelper h2;
			m2.AddHelper(h1);
			m2.AddHelper(h2);
			JsonParseMaster* clone = m2.Clone();
			*clone = std::move(master);
			Assert::AreEqual(data.GetJsonParseMaster(), clone);
			delete clone;
		}

		TEST_METHOD(TestDestructor)
		{
			JsonDepthHelper::DepthSharedData data;
			JsonParseMaster m(data);
			JsonDepthHelper h1;
			JsonIntHelper h2;
			m.AddHelper(h1);
			m.AddHelper(h2);
			JsonParseMaster* clone = m.Clone();
			delete clone;
		}

		TEST_METHOD(TestClone)
		{
			JsonIntHelper::IntSharedData data;
			JsonParseMaster m(data);
			JsonDepthHelper h1;
			JsonIntHelper h2;
			m.AddHelper(h1);
			m.AddHelper(h2);
			m.Parse(R"({ "integer": 100 })"s);
			Assert::AreEqual(data.Data, 100);
			JsonParseMaster* clone = m.Clone();
			clone->Parse(R"({ "integer": 55 })"s);
			Assert::AreEqual(clone->GetSharedData()->As<JsonIntHelper::IntSharedData>()->Data, 55);
			Assert::AreEqual(data.Data, 100);
			delete clone;
		}

		TEST_METHOD(TestAddHelper)
		{
			JsonIntHelper::IntSharedData data;
			JsonParseMaster m(data);
			JsonDepthHelper h1;
			JsonIntHelper h2;
			m.AddHelper(h1);
			m.AddHelper(h2);
			JsonParseMaster* clone = m.Clone();
			Assert::ExpectException<std::exception>([&clone, &h1] { clone->AddHelper(h1); });
			delete clone;
		}

		TEST_METHOD(TestRemoveHelper)
		{
			JsonIntHelper::IntSharedData data;
			JsonParseMaster m(data);
			JsonDepthHelper h1;
			JsonIntHelper h2;
			m.AddHelper(h1);
			m.AddHelper(h2);
			JsonParseMaster* clone = m.Clone();
			Assert::AreEqual(m.GetHelperCount(), 2_z);
			Assert::AreEqual(clone->GetHelperCount(), 2_z);
			m.RemoveHelper(h1);
			Assert::AreEqual(m.GetHelperCount(), 1_z);
			Assert::AreEqual(clone->GetHelperCount(), 2_z);
			clone->RemoveHelper(h2);
			Assert::AreEqual(m.GetHelperCount(), 1_z);
			Assert::AreEqual(clone->GetHelperCount(), 1_z);
			m.RemoveHelper(h2);
			Assert::AreEqual(m.GetHelperCount(), 0_z);
			Assert::AreEqual(clone->GetHelperCount(), 1_z);
			clone->RemoveHelper(h1);
			Assert::AreEqual(m.GetHelperCount(), 0_z);
			Assert::AreEqual(clone->GetHelperCount(), 0_z);
			delete clone;
		}

		TEST_METHOD(GetSharedData)
		{
			JsonIntHelper::IntSharedData data;
			const JsonParseMaster m(data);
			Assert::AreEqual(static_cast<void*>(const_cast<JsonParseMaster::SharedData*>(m.GetSharedData())), static_cast<void*>(&data));
		}

		TEST_METHOD(SetSharedData)
		{
			{
				JsonIntHelper::IntSharedData data;
				JsonIntHelper::IntSharedData data2;
				JsonParseMaster m(data);
				m.SetSharedData(data2);
				JsonParseMaster* clone = m.Clone();
				Assert::ExpectException<std::exception>([&clone, &data2] { clone->SetSharedData(data2); });
				delete clone;
				const JsonIntHelper::IntSharedData* data3 = &data;
				Assert::AreEqual(const_cast<JsonParseMaster*>(data3->GetJsonParseMaster()), &m);
			}

			{
				DefaultComparator<IJsonParseHelper*> c;
				Assert::IsTrue(c(nullptr, nullptr));
				JsonDepthHelper a;
				JsonIntHelper b;
				IJsonParseHelper& h1 = a;
				IJsonParseHelper& h2 = b;
				Assert::IsFalse(c(&h1, nullptr));
				Assert::IsTrue(h1 != h2);
				h1.ToString();
				h1.Equals(nullptr);
			}
		}

		TEST_METHOD(TestIsClone)
		{
			JsonIntHelper::IntSharedData data;
			JsonParseMaster m(data);
			JsonParseMaster* clone = m.Clone();
			Assert::IsFalse(m.IsClone());
			Assert::IsTrue(clone->IsClone());
			delete clone;
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonParseMasterTest::sStartMemState;
}
