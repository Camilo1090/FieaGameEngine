#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <json/json.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::literals::string_literals;

namespace UnitTestLibraryDesktop
{

	TEST_CLASS(JsonCppTest)
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

		void Indent(std::ostream& ofs, int indent) {
			for (int i = 0; i < indent; i++)
				ofs << ' ';
		}

		void MyPrint(std::ostream& ofs, const Json::Value& val, int indent = 0) {
			switch (val.type()) {
			case Json::nullValue: ofs << "null"; break;
			case Json::booleanValue: ofs << (val.asBool() ? "true" : "false"); break;
			case Json::intValue: ofs << val.asLargestInt(); break;
			case Json::uintValue: ofs << val.asLargestUInt(); break;
			case Json::realValue: ofs << val.asDouble(); break;
			case Json::stringValue: ofs << '"' << val.asString() << '"'; break;
			case Json::arrayValue: {
				Json::ArrayIndex size = val.size();
				if (size == 0)
					ofs << "[]";
				else {
					ofs << "[\n";
					int newIndent = indent + 4;
					for (Json::ArrayIndex i = 0; i < size; i++) {
						Indent(ofs, newIndent);
						MyPrint(ofs, val[i], newIndent);
						ofs << (i + 1 == size ? "\n" : ",\n");
					}
					Indent(ofs, indent);
					ofs << ']';
				}
				break;
			}
			case Json::objectValue: {
				if (val.empty())
					ofs << "{}";
				else {
					ofs << "{\n";
					int newIndent = indent + 4;
					std::vector<std::string> keys = val.getMemberNames();
					for (size_t i = 0; i < keys.size(); i++) {
						Indent(ofs, newIndent);
						const std::string& key = keys[i];
						ofs << '"' << key << '"' << " : ";
						MyPrint(ofs, val[key], newIndent);
						ofs << (i + 1 == keys.size() ? "\n" : ",\n");
					}
					Indent(ofs, indent);
					ofs << '}';
				}
				break;
			}
			default:
				std::cerr << "Wrong type!" << std::endl;
				exit(0);
			}
		}

		TEST_METHOD(TestJsonCpp)
		{
			std::stringstream ss;
			ss << R"({"name": "camilo"})"s;

			Json::Value value;
			ss >> value;
			Assert::IsTrue(value.isObject());
			Assert::IsTrue(value["name"].isString());
			std::string name = value["name"].asString();
			Assert::AreEqual(name, "camilo"s);

			/*std::filebuf fb;
			if (fb.open("content/json/depth.json", std::ios::in))
			{
				std::istream stream(&fb);
				Json::Value val;
				stream >> val;
				std::stringstream ss;

				MyPrint(ss, val);

				Logger::WriteMessage(ss.str().c_str());

				fb.close();
			}
			else
			{
				Assert::Fail();
			}*/
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState JsonCppTest::sStartMemState;
}
