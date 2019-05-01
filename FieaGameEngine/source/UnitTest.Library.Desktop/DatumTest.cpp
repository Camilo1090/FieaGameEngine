#include "pch.h"
#include <string>
#include "CppUnitTest.h"
#include "Datum.h"
#include "Utility.h"
#include "ToStringSpecializations.h"
#include "Foo.h"
#include <glm/glm.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(DatumTest)
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
			// Default
			{
				Datum datum;
				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Capacity(), 0_z);
				Assert::AreEqual(datum.Type(), Datum::DatumType::UNKNOWN);
				Assert::IsTrue(datum.OwnsStorage());
				Assert::ExpectException<std::exception>([] { Datum(Datum::DatumType::UNKNOWN, 10); });

				Datum datum2(Datum::DatumType::INTEGER, 10);
				Assert::AreEqual(datum2.Size(), 0_z);
				Assert::AreEqual(datum2.Capacity(), 10_z);
				Assert::AreEqual(datum2.Type(), Datum::DatumType::INTEGER);
			}

			// Datum Copy
			{
				Datum d1 = { "1"s, "2"s, "3"s };
				Datum d2 = d1;
				Assert::AreEqual(d2, d1);
				d2.Set("dasdas"s, 1);
				Assert::IsTrue(d1 != d2);

				std::string values[3] = { "1"s, "2"s, "3"s };
				d1.SetStorage(values, 3);
				Datum d3 = d1;
				Assert::AreEqual(d1, d3);
				Assert::ExpectException<std::exception>([&d3] { d3.PushBack("1"s); });
			}

			// Datum Move
			{
				Datum datum("Hello"s);
				Datum d = std::move(datum);
				Assert::AreEqual(datum.Size(), 0_z);
				Assert::AreEqual(datum.Capacity(), 0_z);
				Assert::AreEqual(datum.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(datum.OwnsStorage());
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());

				std::string values[3] = { "1"s, "2"s, "3"s };
				d.SetStorage(values, 3);
				Datum d2 = std::move(d);
				Assert::AreNotEqual(d2, d);
				Assert::AreEqual(d2.Size(), 3_z);
				Assert::AreEqual(d2.Capacity(), 3_z);
				Assert::AreEqual(d2.Type(), Datum::DatumType::STRING);
				Assert::IsFalse(d2.OwnsStorage());
				Assert::ExpectException<std::exception>([&d2] { d2.PushBack("1"s); });
			}

			// Integer Copy
			{
				Datum d = 10;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<std::int32_t>(), 10);
				Assert::ExpectException<std::exception>([&d] { d = "bad"s; });

				d = { 1, 2, 3, 4, 5 };
				Datum d2;
				d2 = d;
				Assert::AreEqual(d, d2);
				d2.Set(10);
				Assert::AreNotEqual(d, d2);

				int values[5] = { 1, 2, 3, 4, 5 };
				d2.SetStorage(values, 5);
				d = d2;
				Assert::AreEqual(d, d2);
				values[0] = 10;
				Assert::AreEqual(d, d2);
			}

			// Float Copy
			{
				Datum d = 10.0f;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<float>(), 10.0f);
				Assert::ExpectException<std::exception>([&d] { d = "bad"s; });

				d = { 1.0f, 2.0f };
				Datum d2;
				d2 = d;
				Assert::AreEqual(d, d2);
				d2.Set(10.0f);
				Assert::AreNotEqual(d, d2);
			}

			// Vector copy
			{
				Datum d = glm::vec4();
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<glm::vec4>(), glm::vec4());
				Assert::ExpectException<std::exception>([&d] { d = "bad"s; });

				d = { glm::vec4(1), glm::vec4(2) };
				Datum d2;
				d2 = d;
				Assert::AreEqual(d, d2);
				d2.Set(glm::vec4());
				Assert::AreNotEqual(d, d2);
			}

			// Matrix Copy
			{
				Datum d = glm::mat4();
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<glm::mat4>(), glm::mat4());
				Assert::ExpectException<std::exception>([&d] { d = "bad"s; });

				d = { glm::mat4(1), glm::mat4(2) };
				Datum d2;
				d2 = d;
				Assert::AreEqual(d, d2);
				d2.Set(glm::mat4());
				Assert::AreNotEqual(d, d2);
			}

			// String Copy
			{
				std::string s = "hello"s;
				Datum d = s;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<std::string>(), s);
				Assert::ExpectException<std::exception>([&d] { d = 1.0f; });
			}

			// String Move
			{
				Datum d = "kkkkkk"s;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<std::string>(), "kkkkkk"s);
				Assert::ExpectException<std::exception>([&d] { d = 1.0f; });
			}

			// RTTI Copy
			{
				Foo foo1(1), foo2(2), foo3(3);
				Datum d = &foo1;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<RTTI*>()->As<Foo>(), &foo1);
				Assert::ExpectException<std::exception>([&d] { d = "bad"s; });

				d = { &foo1, &foo2 };
				Datum d2;
				d2 = d;
				Assert::AreEqual(d, d2);
				d2.Set(&foo3);
				Assert::AreNotEqual(d, d2);
			}

			// Integer initializer list
			{
				Datum d = { 1, 2, 3 };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());
			}

			// Float initializer list
			{
				Datum d = { 1.0f, 2.0f, 3.0f };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::IsTrue(d.OwnsStorage());
			}

			// Vector initializer list
			{
				Datum d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::IsTrue(d.OwnsStorage());
			}

			// Matrix initializer list
			{
				Datum d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::IsTrue(d.OwnsStorage());
			}

			// String initializer list
			{
				Datum d = { "1", "2", "3" };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
			}

			// RTTI initializer list
			{
				Foo foo1(1), foo2(2), foo3(3);
				Datum d = { &foo1, &foo2, &foo3 };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::IsTrue(d.OwnsStorage());
			}
		}

		TEST_METHOD(TestAssignment)
		{
			// Datum copy assignment
			{
				Datum d1 = { "hello", "world" };
				Datum d2 = "";
				Assert::AreNotEqual(d2, d1);
				Assert::AreEqual(d2.Size(), 1_z);
				Assert::AreEqual(d2.Capacity(), 1_z);
				Assert::AreEqual(d2.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d2.OwnsStorage());
				Assert::AreEqual(d2.Get<std::string>(), ""s);
				d2 = d1;
				Assert::AreEqual(d2, d1);
				d2.Set("just here");
				Assert::AreNotEqual(d2, d1);
			}

			// Datum move assignment
			{
				Datum d1 = { "1", "2", "3", "4" };
				Datum d2 = "100";
				Assert::AreNotEqual(d2, d1);
				Assert::AreEqual(d2.Size(), 1_z);
				Assert::AreEqual(d2.Capacity(), 1_z);
				Assert::AreEqual(d2.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d2.OwnsStorage());
				d2 = std::move(d1);
				Assert::AreNotEqual(d2, d1);
				Assert::AreEqual(d2.Size(), 4_z);
				Assert::AreEqual(d2.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d2.OwnsStorage());
			}

			// Integer assignment
			{
				Datum d = { 1, 2, 3 };
				d = 100;
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<std::int32_t>(), 100);
			}

			// Float assignment
			{
				Datum d = { 1.0f, 2.0f, 3.0f };
				d = 100.0f;
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<float>(), 100.0f);
			}

			// Vector assignment
			{
				Datum d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				d = glm::vec4(50);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<glm::vec4>(), glm::vec4(50));
			}

			// Matrix assignment
			{
				Datum d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				d = glm::mat4(50);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<glm::mat4>(), glm::mat4(50));
			}

			// String copy assignment
			{
				std::string s = "hello";
				Datum d = { "1"s, "2"s, "3"s };
				d = s;
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<std::string>(), s);
			}

			// String move assignment
			{
				Datum d = { "1"s, "2"s, "3"s };
				d = "100"s;
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<std::string>(), "100"s);
			}

			// RTTI assignment
			{
				Foo foo1(1), foo2(2);
				Datum d = &foo1;
				d = &foo2;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::IsTrue(d.OwnsStorage());
				Assert::AreEqual(d.Get<RTTI*>()->As<Foo>(), &foo2);
				Assert::AreEqual(foo1, Foo(1));
			}

			// Integer initializer list
			{
				Datum d = 100;
				d = { 1, 2, 3 };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());
			}

			// Float initializer list
			{
				Datum d = 100.0f;
				d = { 1.0f, 2.0f, 3.0f };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::IsTrue(d.OwnsStorage());
			}

			// Vector initializer list
			{
				Datum d = glm::vec4();
				d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::IsTrue(d.OwnsStorage());
			}

			// Matrix initializer list
			{
				Datum d = glm::mat4();
				d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::IsTrue(d.OwnsStorage());
			}

			// String initializer list
			{
				Datum d = "henlo frens";
				d = { "1", "2", "3" };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
			}

			// RTTI initializer list
			{
				Foo foo1(1), foo2(2), foo3(3);
				Datum d = &foo2;
				d = { &foo1, &foo2, &foo3 };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::IsTrue(d.OwnsStorage());
			}
		}

		TEST_METHOD(TestComparisons)
		{
			// Unknown
			{
				Datum d1;
				Datum d2;
				Assert::AreEqual(d1, d2);
				d2 = 1;
				Assert::IsTrue(d1 != d2);
			}

			// Datum
			{
				Datum d1 = { "1", "2","3" };
				Datum d2 = 1;
				Assert::IsTrue(d1 != d2);
				d2 = d1;
				Assert::AreEqual(d1, d2);
			}

			// Integer
			{
				Datum d1 = 1;
				int d2 = 1;
				Assert::IsTrue(d1 == d2);
				d2 = 25;
				Assert::IsTrue(d1 != d2);
			}

			// Float
			{
				Datum d1 = 1.0f;
				float d2 = 1.0f;
				Assert::IsTrue(d1 == d2);
				d2 = 25.0f;
				Assert::IsTrue(d1 != d2);
			}

			// Vector
			{
				Datum d1 = glm::vec4();
				glm::vec4 d2(0);
				Assert::IsTrue(d1 == d2);
				d2 = glm::vec4(5);
				Assert::IsTrue(d1 != d2);
			}

			// Matrix
			{
				Datum d1 = glm::mat4();
				glm::mat4 d2(0);
				Assert::IsTrue(d1 == d2);
				d2 = glm::mat4(5);
				Assert::IsTrue(d1 != d2);
			}

			// String
			{
				Datum d1 = "hello"s;
				auto d2 = "hello"s;
				Assert::IsTrue(d1 == d2);
				d2 = "world"s;
				Assert::IsTrue(d1 != d2);
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3);
				Datum d1 = &foo1;
				RTTI* d2 = &foo1;
				Assert::IsTrue(d1 == d2);
				d2 = &foo2;
				Assert::IsTrue(d1 != d2);

				Datum d3 = &foo1;
				Assert::AreEqual(d1, d3);
				d3 = &foo2;
				Assert::IsTrue(d1 != d3);
				RTTI* ptr = nullptr;
				d1.Set(ptr);
				d3.Set(ptr);
				Assert::AreEqual(d1, d3);
				Assert::AreEqual(d3, d1);
				d3.Set(&foo1);
				Assert::IsTrue(d1 != d3);
				Assert::IsTrue(d3 != d1);
			}
		}

		TEST_METHOD(TestPushBack)
		{
			// Integer
			{
				Datum d;
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::UNKNOWN);
				Assert::IsTrue(d.OwnsStorage());
				const int count = 10;
				for (int i = 1; i <= count; ++i)
				{
					d.PushBack(i);
					Assert::AreEqual(d.Size(), static_cast<size_t>(i));
					Assert::IsTrue(d.Capacity() >= d.Size());
					Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
					Assert::IsTrue(d.OwnsStorage());
					Assert::AreEqual(d.Back<std::int32_t>(), i);
				}
				Assert::ExpectException<std::exception>([&d] { d.PushBack("error"s); });
				int data[] = { 1, 2, 3 };
				d.SetStorage(data, 3);
				Assert::ExpectException<std::exception>([&d] { d.PushBack(4); });
			}

			// Float
			{
				Datum d;
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::UNKNOWN);
				Assert::IsTrue(d.OwnsStorage());
				const int count = 10;
				for (int i = 1; i <= count; ++i)
				{
					d.PushBack(i + 0.0f);
					Assert::AreEqual(d.Size(), static_cast<size_t>(i));
					Assert::IsTrue(d.Capacity() >= d.Size());
					Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
					Assert::IsTrue(d.OwnsStorage());
					Assert::AreEqual(d.Back<float>(), i + 0.0f);
				}
				Assert::ExpectException<std::exception>([&d] { d.PushBack("error"s); });
				float data[] = { 1.0f, 2.0f, 3.0f };
				d.SetStorage(data, 3);
				Assert::ExpectException<std::exception>([&d] { d.PushBack(4.0f); });
			}

			// Vector
			{
				Datum d;
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::UNKNOWN);
				Assert::IsTrue(d.OwnsStorage());
				const int count = 10;
				for (int i = 1; i <= count; ++i)
				{
					d.PushBack(glm::vec4());
					Assert::AreEqual(d.Size(), static_cast<size_t>(i));
					Assert::IsTrue(d.Capacity() >= d.Size());
					Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
					Assert::IsTrue(d.OwnsStorage());
					Assert::AreEqual(d.Back<glm::vec4>(), glm::vec4());
				}
				Assert::ExpectException<std::exception>([&d] { d.PushBack("error"s); });
				glm::vec4 data[] = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				d.SetStorage(data, 3);
				Assert::ExpectException<std::exception>([&d] { d.PushBack(glm::vec4(4)); });
			}

			// Matrix
			{
				Datum d;
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::UNKNOWN);
				Assert::IsTrue(d.OwnsStorage());
				const int count = 10;
				for (int i = 1; i <= count; ++i)
				{
					d.PushBack(glm::mat4());
					Assert::AreEqual(d.Size(), static_cast<size_t>(i));
					Assert::IsTrue(d.Capacity() >= d.Size());
					Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
					Assert::IsTrue(d.OwnsStorage());
					Assert::AreEqual(d.Back<glm::mat4>(), glm::mat4());
				}
				Assert::ExpectException<std::exception>([&d] { d.PushBack("error"s); });
				glm::mat4 data[] = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				d.SetStorage(data, 3);
				Assert::ExpectException<std::exception>([&d] { d.PushBack(glm::mat4(4)); });
			}

			// String copy
			{
				Datum d;
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::UNKNOWN);
				Assert::IsTrue(d.OwnsStorage());
				const int count = 10;
				for (int i = 1; i <= count; ++i)
				{
					auto s = std::to_string(i);
					d.PushBack(s);
					Assert::AreEqual(d.Size(), static_cast<size_t>(i));
					Assert::IsTrue(d.Capacity() >= d.Size());
					Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
					Assert::IsTrue(d.OwnsStorage());
					Assert::AreEqual(d.Back<std::string>(), std::to_string(i));
				}
				Assert::ExpectException<std::exception>([&d] { d.PushBack(4.5f); });
				std::string data[] = { "1"s, "2"s, "3"s };
				d.SetStorage(data, 3);
				Assert::ExpectException<std::exception>([&d] { auto s = "4"s; d.PushBack(s); });
			}

			// String move
			{
				Datum d;
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::UNKNOWN);
				Assert::IsTrue(d.OwnsStorage());
				const int count = 10;
				for (int i = 1; i <= count; ++i)
				{
					d.PushBack(std::to_string(i));
					Assert::AreEqual(d.Size(), static_cast<size_t>(i));
					Assert::IsTrue(d.Capacity() >= d.Size());
					Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
					Assert::IsTrue(d.OwnsStorage());
					Assert::AreEqual(d.Back<std::string>(), std::to_string(i));
				}
				Assert::ExpectException<std::exception>([&d] { d.PushBack(4.5f); });
				std::string data[] = { "1"s, "2"s, "3"s };
				d.SetStorage(data, 3);
				Assert::ExpectException<std::exception>([&d] { d.PushBack("4"s); });
			}

			// RTTI
			{
				Datum d;
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::UNKNOWN);
				Assert::IsTrue(d.OwnsStorage());
				const int count = 5;
				Foo foos[count] = { Foo(1), Foo(2), Foo(3), Foo(4), Foo(5) };
				for (int i = 0; i < count; ++i)
				{
					d.PushBack(&foos[i]);
					Assert::AreEqual(d.Size(), static_cast<size_t>(i + 1));
					Assert::IsTrue(d.Capacity() >= d.Size());
					Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
					Assert::IsTrue(d.OwnsStorage());
					Assert::AreEqual(d.Back<RTTI*>()->As<Foo>(), &foos[i]);
				}
				Assert::ExpectException<std::exception>([&d] { d.PushBack("error"s); });
				RTTI* data[] = { &foos[0], &foos[1], &foos[2] };
				d.SetStorage(data, 3);
				Assert::ExpectException<std::exception>([&d, &foos] { d.PushBack(&foos[3]); });
			}

			// first element push wrong type
			{
				Datum d = { "1", "2", "3" };
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				d.Clear();
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::ExpectException<std::exception>([&d] { d.PushBack(1); });
			}

			// external storage
			{
				std::string values[3] = { "1"s, "2"s, "3"s };
				Datum d;
				d.SetStorage(values, 3);
				Assert::ExpectException<std::exception>([&d] { d.PushBack("error"s); });
			}
		}

		TEST_METHOD(TestSet)
		{
			// Integer
			{
				Datum d = { 1, 2, 3 };
				Assert::AreEqual(d.Get<std::int32_t>(), 1);
				d.Set(55);
				Assert::AreEqual(d.Get<std::int32_t>(), 55);
				d.Set(10, 2);
				Assert::AreEqual(d.Get<std::int32_t>(2), 10);
				Assert::ExpectException<std::exception>([&d] { d.Set(0, 10); });
				Assert::ExpectException<std::exception>([&d] { d.Set("error"s, 1); });
			}

			// Float
			{
				Datum d = { 1.0f, 2.0f, 3.0f };
				Assert::AreEqual(d.Get<float>(), 1.0f);
				d.Set(55.0f);
				Assert::AreEqual(d.Get<float>(), 55.0f);
				d.Set(10.0f, 2);
				Assert::AreEqual(d.Get<float>(2), 10.0f);
				Assert::ExpectException<std::exception>([&d] { d.Set(0.0f, 10); });
				Assert::ExpectException<std::exception>([&d] { d.Set("error"s, 1); });
			}

			// Vector
			{
				Datum d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::AreEqual(d.Get<glm::vec4>(), glm::vec4(1));
				d.Set(glm::vec4(55));
				Assert::AreEqual(d.Get<glm::vec4>(), glm::vec4(55));
				d.Set(glm::vec4(10), 2);
				Assert::AreEqual(d.Get<glm::vec4>(2), glm::vec4(10));
				Assert::ExpectException<std::exception>([&d] { d.Set(glm::vec4(), 10); });
				Assert::ExpectException<std::exception>([&d] { d.Set("error"s, 1); });
			}

			// Matrix
			{
				Datum d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::AreEqual(d.Get<glm::mat4>(), glm::mat4(1));
				d.Set(glm::mat4(55));
				Assert::AreEqual(d.Get<glm::mat4>(), glm::mat4(55));
				d.Set(glm::mat4(10), 2);
				Assert::AreEqual(d.Get<glm::mat4>(2), glm::mat4(10));
				Assert::ExpectException<std::exception>([&d] { d.Set(glm::mat4(), 10); });
				Assert::ExpectException<std::exception>([&d] { d.Set("error"s, 1); });
			}

			// String
			{
				Datum d = { "1"s, "2"s, "3"s };
				Assert::AreEqual(d.Get<std::string>(), "1"s);
				d.Set("55"s);
				Assert::AreEqual(d.Get<std::string>(), "55"s);
				auto s = "10"s;
				d.Set(s, 2);
				Assert::AreEqual(d.Get<std::string>(2), "10"s);
				Assert::ExpectException<std::exception>([&d] { d.Set("1"s, 10); });
				Assert::ExpectException<std::exception>([&d] { d.Set(10.0f, 1); });
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3), foo4(4), foo5(5);
				Datum d = { &foo1, &foo2, &foo3 };
				Assert::AreEqual(d.Get<RTTI*>()->As<Foo>(), &foo1);
				d.Set(&foo4);
				Assert::AreEqual(d.Get<RTTI*>()->As<Foo>(), &foo4);
				d.Set(&foo5, 2);
				Assert::AreEqual(d.Get<RTTI*>(2)->As<Foo>(), &foo5);
				Assert::ExpectException<std::exception>([&d] { d.Set("1"s, 10); });
				Assert::ExpectException<std::exception>([&d] { d.Set(10.0f, 1); });
			}

			// external storage
			{
				std::string values[3] = { "1"s, "2"s, "3"s };
				Datum d;
				d.SetStorage(values, 3);
				d.Set("new"s, 1);
				Assert::AreEqual(d.Get<std::string>(1), "new"s);
				//Assert::ExpectException<std::exception>([&d] { d.Set("error"s, 1); });
			}
		}

		TEST_METHOD(TestSetFromString)
		{
			// Integer
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("1"s); });
				d.SetType(Datum::DatumType::INTEGER);
				d.Resize(1);
				d.SetFromString("   10 "s);
				Assert::AreEqual(d, Datum(10));
			}

			// Float
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("1.0f"s); });
				d.SetType(Datum::DatumType::FLOAT);
				d.Resize(1);
				d.SetFromString("   10.0f "s);
				Assert::AreEqual(d, Datum(10.0f));
			}

			// Vector
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("  vec4(1 ,   2, 3    ,4 ) "s); });
				d.SetType(Datum::DatumType::VECTOR);
				d.Resize(1);
				d.SetFromString("  vec4(1 ,   2, 3    ,4 ) "s);
				Assert::AreEqual(d, Datum(glm::vec4(1, 2, 3, 4)));
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("  vec4(1    2, 3    ,4 ) "s); });
			}

			// Matrix
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("  mat4x4((1 ,   2, 3    ,4), (1 ,   2, 3    ,4), (1 ,   2, 3    ,4), (1 ,   2, 3    ,4)) "s); });
				d.SetType(Datum::DatumType::MATRIX);
				d.Resize(1);
				d.SetFromString("  mat4x4((1 ,   2, 3    ,4), (1 ,   2, 3    ,4), (1 ,   2, 3    ,4), (1 ,   2, 3    ,4)) "s);
				Assert::AreEqual(d, Datum(glm::mat4(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4)));
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("  mat44((), (1 ,   2, 3  2, 3    ,4)) "s); });
			}

			// String
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("1"s); });
				d.SetType(Datum::DatumType::STRING);
				d.Resize(1);
				d.SetFromString("   10 "s);
				Assert::AreEqual(d, Datum("   10 "s));
			}

			// RTTI
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("1"s); });
				d.SetType(Datum::DatumType::POINTER);
				d.Resize(1);
				Assert::ExpectException<std::exception>([&d] { d.SetFromString("no string representation"s); });
			}

			// external storage
			{
				std::string values[3] = { "1"s, "2"s, "3"s };
				Datum d;
				d.SetStorage(values, 3);
				d.SetFromString("new"s, 1);
				Assert::AreEqual(d.Get<std::string>(1), "new"s);
				//Assert::ExpectException<std::exception>([&d] { d.SetFromString("error"s, 1); });
			}
		}

		TEST_METHOD(TestSetStorage)
		{
			// Integer
			{
				int v[3] = { 1, 2, 3 };
				Datum d = 100;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsFalse(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsFalse(d.OwnsStorage());
			}

			// Float
			{
				float v[3] = { 1.0f, 2.0f, 3.0f };
				Datum d = 100.0f;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::IsFalse(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::IsFalse(d.OwnsStorage());
			}

			// Vector
			{
				glm::vec4 v[3] = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Datum d = glm::vec4(100);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::IsFalse(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::IsFalse(d.OwnsStorage());
			}

			// Matrix
			{
				glm::mat4 v[3] = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Datum d = glm::mat4(100);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::IsFalse(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::IsFalse(d.OwnsStorage());
			}

			// String
			{
				std::string v[3] = { "1"s, "2"s, "3"s };
				Datum d = "100"s;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsFalse(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsFalse(d.OwnsStorage());
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3);
				RTTI* v[3] = { &foo1, &foo2, &foo3 };
				Datum d = &foo3;
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::IsFalse(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::IsTrue(d.OwnsStorage());
				d.SetStorage(v, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::IsFalse(d.OwnsStorage());
			}

			// Type guard
			{
				int v[3] = { 1, 2, 3 };
				Datum d = "1"s;
				Assert::ExpectException<std::exception>([&d, &v] { d.SetStorage(v, 3); });
			}
		}

		TEST_METHOD(TestFind)
		{
			// Integer
			{
				Datum d = { 1, 2, 3 };
				Datum::Iterator it = d.Find(2);
				Assert::AreEqual(*(reinterpret_cast<std::int32_t*>(*it)), 2);
				it = d.Find(555);
				Assert::AreEqual(it, d.end());
				Assert::ExpectException<std::exception>([&d] { d.Find("error"s); });

				const Datum d2 = { 1, 2, 3 };
				Datum::ConstIterator it2 = d2.Find(2);
				Assert::AreEqual(*(reinterpret_cast<std::int32_t*>(*it2)), 2);
				it2 = d2.Find(555);
				Assert::AreEqual(it2, d2.end());
				Assert::ExpectException<std::exception>([&d2] { d2.Find("error"s); });
			}

			// Float
			{
				Datum d = { 1.0f, 2.0f, 3.0f };
				Datum::Iterator it = d.Find(3.0f);
				Assert::AreEqual(*(reinterpret_cast<float*>(*it)), 3.0f);
				it = d.Find(555.0f);
				Assert::AreEqual(it, d.end());
				Assert::ExpectException<std::exception>([&d] { d.Find("error"s); });

				const Datum d2 = { 1.0f, 2.0f, 3.0f };
				Datum::ConstIterator it2 = d2.Find(3.0f);
				Assert::AreEqual(*(reinterpret_cast<float*>(*it2)), 3.0f);
				it2 = d2.Find(555.0f);
				Assert::AreEqual(it2, d2.end());
				Assert::ExpectException<std::exception>([&d2] { d2.Find("error"s); });
			}

			// Vector
			{
				Datum d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Datum::Iterator it = d.Find(glm::vec4(3));
				Assert::AreEqual(*(reinterpret_cast<glm::vec4*>(*it)), glm::vec4(3));
				it = d.Find(glm::vec4(555));
				Assert::AreEqual(it, d.end());
				Assert::ExpectException<std::exception>([&d] { d.Find("error"s); });

				const Datum d2 = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Datum::ConstIterator it2 = d2.Find(glm::vec4(3));
				Assert::AreEqual(*(reinterpret_cast<glm::vec4*>(*it2)), glm::vec4(3));
				it2 = d2.Find(glm::vec4(555));
				Assert::AreEqual(it2, d2.end());
				Assert::ExpectException<std::exception>([&d2] { d2.Find("error"s); });
			}

			// Matrix
			{
				Datum d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Datum::Iterator it = d.Find(glm::mat4(3));
				Assert::AreEqual(*(reinterpret_cast<glm::mat4*>(*it)), glm::mat4(3));
				it = d.Find(glm::mat4(555));
				Assert::AreEqual(it, d.end());
				Assert::ExpectException<std::exception>([&d] { d.Find("error"s); });

				const Datum d2 = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Datum::ConstIterator it2 = d2.Find(glm::mat4(3));
				Assert::AreEqual(*(reinterpret_cast<glm::mat4*>(*it2)), glm::mat4(3));
				it2 = d2.Find(glm::mat4(555));
				Assert::AreEqual(it2, d2.end());
				Assert::ExpectException<std::exception>([&d2] { d2.Find("error"s); });
			}

			// String
			{
				Datum d = { "1"s, "2"s, "3"s };
				Datum::Iterator it = d.Find("1"s);
				Assert::AreEqual(*(reinterpret_cast<std::string*>(*it)), "1"s);
				it = d.Find("555"s);
				Assert::AreEqual(it, d.end());
				Assert::ExpectException<std::exception>([&d] { d.Find(100); });

				const Datum d2 = { "1"s, "2"s, "3"s };
				Datum::ConstIterator it2 = d2.Find("1"s);
				Assert::AreEqual(*(reinterpret_cast<std::string*>(*it2)), "1"s);
				it2 = d2.Find("555"s);
				Assert::AreEqual(it2, d2.end());
				Assert::ExpectException<std::exception>([&d2] { d2.Find(100); });
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3), foo4(4);
				Datum d = { &foo1, &foo2, &foo3 };
				Datum::Iterator it = d.Find(&foo3);
				Assert::AreEqual((*(reinterpret_cast<RTTI**>(*it)))->As<Foo>(), &foo3);
				it = d.Find(&foo4);
				Assert::AreEqual(it, d.end());
				Assert::ExpectException<std::exception>([&d] { d.Find("error"s); });

				const Datum d2 = { &foo1, &foo2, &foo3 };
				Datum::ConstIterator it2 = d2.Find(&foo3);
				Assert::AreEqual((*(reinterpret_cast<RTTI**>(*it2)))->As<Foo>(), &foo3);
				it2 = d2.Find(&foo4);
				Assert::AreEqual(it2, d2.end());
				Assert::ExpectException<std::exception>([&d2] { d2.Find("error"s); });
			}
		}

		TEST_METHOD(TestRemove)
		{
			// Integer
			{
				Datum d = { 1, 2, 3 };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreNotEqual(d.Find(2), d.end());
				Assert::IsTrue(d.Remove(2));
				Assert::AreEqual(d.Size(), 2_z);
				Assert::AreEqual(d.Find(2), d.end());
				Assert::IsFalse(d.Remove(55));
				Assert::ExpectException<std::exception>([&d] { d.Remove("error"s); });
			}

			// Float
			{
				Datum d = { 1.0f, 2.0f, 3.0f };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreNotEqual(d.Find(2.0f), d.end());
				Assert::IsTrue(d.Remove(2.0f));
				Assert::AreEqual(d.Size(), 2_z);
				Assert::AreEqual(d.Find(2.0f), d.end());
				Assert::IsFalse(d.Remove(55.0f));
				Assert::ExpectException<std::exception>([&d] { d.Remove("error"s); });
			}

			// Vector
			{
				Datum d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreNotEqual(d.Find(glm::vec4(2)), d.end());
				Assert::IsTrue(d.Remove(glm::vec4(2)));
				Assert::AreEqual(d.Size(), 2_z);
				Assert::AreEqual(d.Find(glm::vec4(2)), d.end());
				Assert::IsFalse(d.Remove(glm::vec4(55)));
				Assert::ExpectException<std::exception>([&d] { d.Remove("error"s); });
			}

			// Matrix
			{
				Datum d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreNotEqual(d.Find(glm::mat4(2)), d.end());
				Assert::IsTrue(d.Remove(glm::mat4(2)));
				Assert::AreEqual(d.Size(), 2_z);
				Assert::AreEqual(d.Find(glm::mat4(2)), d.end());
				Assert::IsFalse(d.Remove(glm::mat4(55)));
				Assert::ExpectException<std::exception>([&d] { d.Remove("error"s); });
			}

			// String
			{
				Datum d = { "1"s, "2"s, "3"s };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreNotEqual(d.Find("2"s), d.end());
				Assert::IsTrue(d.Remove("2"s));
				Assert::AreEqual(d.Size(), 2_z);
				Assert::AreEqual(d.Find("2"s), d.end());
				Assert::IsFalse(d.Remove("55"s));
				Assert::ExpectException<std::exception>([&d] { d.Remove(1); });
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3), foo4(4);
				Datum d = { &foo1, &foo2, &foo3 };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreNotEqual(d.Find(&foo2), d.end());
				Assert::IsTrue(d.Remove(&foo2));
				Assert::AreEqual(d.Size(), 2_z);
				Assert::AreEqual(d.Find(&foo2), d.end());
				Assert::IsFalse(d.Remove(&foo4));
				Assert::ExpectException<std::exception>([&d] { d.Remove("error"s); });
			}

			// Iterator
			{
				Datum d = { "1"s, "2"s, "3"s };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreNotEqual(d.Find("1"s), d.end());
				Assert::IsTrue(d.Remove(d.begin()));
				Assert::AreEqual(d.Size(), 2_z);
				Assert::AreEqual(d.Find("1"s), d.end());
				Assert::IsFalse(d.Remove("55"s));
				Assert::ExpectException<std::exception>([&d] { d.Remove(Datum::Iterator()); });
			}

			// Index
			{
				Datum d = { "1"s, "2"s, "3"s };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreNotEqual(d.Find("3"s), d.end());
				d.RemoveAt(2);
				Assert::AreEqual(d.Size(), 2_z);
				Assert::AreEqual(d.Find("3"s), d.end());
				Assert::ExpectException<std::exception>([&d] { d.RemoveAt(55); });
			}

			// external storage
			{
				int values[3] = { 1, 2, 3 };
				Datum d = 100;
				d.SetStorage(values, 3);
				Assert::ExpectException<std::exception>([&d] { d.RemoveAt(1); });
			}
		}

		TEST_METHOD(TestPopBack)
		{
			const int count = 10;
			Datum d;
			for (int i = 0; i < count; ++i)
			{
				d.PushBack(i);
			}
			for (int i = 1; i <= count; ++i)
			{
				d.PopBack();
				size_t s = count - i;
				Assert::AreEqual(d.Size(), s);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
			}
			Assert::ExpectException<std::exception>([&d] { d.PopBack(); });
		}

		TEST_METHOD(TestResize)
		{
			// Integer
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Resize(1); });
				d = 1;
				d.Resize(10);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				d.Resize(0);
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				d.Resize(1, 1);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::ExpectException<std::exception>([&d] { d.Resize(1, "error"s); });
			}

			// Float
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Resize(1); });
				d = 1.0f;
				d.Resize(10);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				d.Resize(0);
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				d.Resize(1, 1.0f);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::FLOAT);
				Assert::ExpectException<std::exception>([&d] { d.Resize(1, "error"s); });
			}

			// Vector
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Resize(1); });
				d = glm::vec4();
				d.Resize(10);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				d.Resize(0);
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				d.Resize(1, glm::vec4());
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::VECTOR);
				Assert::ExpectException<std::exception>([&d] { d.Resize(1, "error"s); });
			}

			// Matrix
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Resize(1); });
				d = glm::mat4();
				d.Resize(10);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				d.Resize(0);
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				d.Resize(1, glm::mat4());
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::MATRIX);
				Assert::ExpectException<std::exception>([&d] { d.Resize(1, "error"s); });
			}

			// String copy
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Resize(1); });
				d = "1"s;
				d.Resize(10);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				d.Resize(0);
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				auto s = "1"s;
				d.Resize(10, s);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::ExpectException<std::exception>([&d] { d.Resize(1, 1); });
				d.Resize(0, s);
			}

			// String move
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Resize(1); });
				d = "1"s;
				d.Resize(10);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				d.Resize(0);
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				d.Resize(10, "1"s);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::ExpectException<std::exception>([&d] { d.Resize(1, 1); });
				d.Resize(0, "1"s);
			}

			// RTTI
			{
				Foo foo1(1), foo2(2);
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Resize(1); });
				d = &foo1;
				d.Resize(10);
				Assert::AreEqual(d.Size(), 10_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				d.Resize(0);
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				d.Resize(1, &foo2);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::POINTER);
				Assert::ExpectException<std::exception>([&d] { d.Resize(1, "error"s); });
			}

			// external storage
			{
				int values[3] = { 1, 2, 3 };
				Datum d;
				d.SetStorage(values, 3);
				Assert::ExpectException<std::exception>([&d] { d.Resize(1); });
				Assert::ExpectException<std::exception>([&d] { d.Resize(1, 1); });
			}
		}

		TEST_METHOD(TestReserve)
		{
			// Integer
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Reserve(10); });
				d = 1;
				d.Clear();
				const int count = 10;
				d.Reserve(count);
				for (int i = 0; i < count; ++i)
				{
					d.PushBack(i);
					Assert::AreEqual(d.Capacity(), static_cast<size_t>(count));
				}
			}

			// Float
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Reserve(10); });
				d = 1.0f;
				d.Clear();
				const int count = 10;
				d.Reserve(count);
				for (int i = 0; i < count; ++i)
				{
					d.PushBack(i + 0.0f);
					Assert::AreEqual(d.Capacity(), static_cast<size_t>(count));
				}
			}

			// Vector
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Reserve(10); });
				d = glm::vec4(1);
				d.Clear();
				const int count = 10;
				d.Reserve(count);
				for (int i = 0; i < count; ++i)
				{
					d.PushBack(glm::vec4(1));
					Assert::AreEqual(d.Capacity(), static_cast<size_t>(count));
				}
			}

			// Matrix
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Reserve(10); });
				d = glm::mat4();
				d.Clear();
				const int count = 10;
				d.Reserve(count);
				for (int i = 0; i < count; ++i)
				{
					d.PushBack(glm::mat4());
					Assert::AreEqual(d.Capacity(), static_cast<size_t>(count));
				}
			}

			// String
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Reserve(10); });
				d = "1"s;
				d.Clear();
				const int count = 10;
				d.Reserve(count);
				for (int i = 0; i < count; ++i)
				{
					d.PushBack(std::to_string(i));
					Assert::AreEqual(d.Capacity(), static_cast<size_t>(count));
				}
			}

			// RTTI
			{
				Foo foo(1);
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.Reserve(10); });
				d = &foo;
				d.Clear();
				const int count = 10;
				d.Reserve(count);
				Foo foos[count];
				for (int i = 0; i < count; ++i)
				{
					foos[i] = Foo(i);
					d.PushBack(&foos[i]);
					Assert::AreEqual(d.Capacity(), static_cast<size_t>(count));
				}
			}

			// external storage
			{
				int values[3] = { 1, 2, 3 };
				Datum d;
				d.SetStorage(values, 3);
				Assert::ExpectException<std::exception>([&d] { d.Reserve(10); });
			}
		}

		TEST_METHOD(TestShrinkToFit)
		{
			// Integer
			{
				Datum d = 1;
				d.Reserve(10);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				d.ShrinkToFit();
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
			}

			// Float
			{
				Datum d = 1.0f;
				d.Reserve(10);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				d.ShrinkToFit();
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
			}

			// Vector
			{
				Datum d = glm::vec4();
				d.Reserve(10);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				d.ShrinkToFit();
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
			}

			// Matrix
			{
				Datum d = glm::mat4();
				d.Reserve(10);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				d.ShrinkToFit();
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
			}

			// String
			{
				Datum d = "1"s;
				d.Reserve(10);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				d.ShrinkToFit();
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
			}

			// RTTI
			{
				Foo foo(1);
				Datum d = &foo;
				d.Reserve(10);
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 10_z);
				d.ShrinkToFit();
				Assert::AreEqual(d.Size(), 1_z);
				Assert::AreEqual(d.Capacity(), 1_z);
			}

			// external storage
			{
				int values[3] = { 1, 2, 3 };
				Datum d;
				d.SetStorage(values, 3);
				Assert::ExpectException<std::exception>([&d] { d.ShrinkToFit(); });
			}
		}

		TEST_METHOD(TestClear)
		{
			// Integer, Float, Vector, Matrix, RTTI
			{
				Datum d = { 1, 2, 3 };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());

				int values[3] = { 1, 2, 3 };
				d.SetStorage(values, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsFalse(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::INTEGER);
				Assert::IsTrue(d.OwnsStorage());
			}

			// String
			{
				Datum d = { "1"s, "2"s, "3"s };
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());

				std::string values[3] = { "1"s, "2"s, "3"s };
				d.SetStorage(values, 3);
				Assert::AreEqual(d.Size(), 3_z);
				Assert::AreEqual(d.Capacity(), 3_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsFalse(d.OwnsStorage());
				d.Clear();
				Assert::AreEqual(d.Size(), 0_z);
				Assert::AreEqual(d.Capacity(), 0_z);
				Assert::AreEqual(d.Type(), Datum::DatumType::STRING);
				Assert::IsTrue(d.OwnsStorage());
			}
		}

		TEST_METHOD(TestGet)
		{
			// Integer
			{
				Datum d = { 1, 2, 3 };
				Assert::ExpectException<std::exception>([&d] { d.Get<std::int32_t>(10); });
				Assert::ExpectException<std::exception>([&d] { d.Get<std::string>(0); });
				Assert::AreEqual(d.Get<std::int32_t>(2), 3);

				const Datum d2 = { 1, 2, 3 };
				Assert::ExpectException<std::exception>([&d2] { d2.Get<std::int32_t>(10); });
				Assert::ExpectException<std::exception>([&d2] { d2.Get<std::string>(0); });
				Assert::AreEqual(d2.Get<std::int32_t>(2), 3);
			}

			// Float
			{
				Datum d = { 1.0f, 2.0f, 3.0f };
				Assert::ExpectException<std::exception>([&d] { d.Get<float>(10); });
				Assert::ExpectException<std::exception>([&d] { d.Get<std::string>(0); });
				Assert::AreEqual(d.Get<float>(2), 3.0f);

				const Datum d2 = { 1.0f, 2.0f, 3.0f };
				Assert::ExpectException<std::exception>([&d2] { d2.Get<float>(10); });
				Assert::ExpectException<std::exception>([&d2] { d2.Get<std::string>(0); });
				Assert::AreEqual(d2.Get<float>(2), 3.0f);
			}

			// Vector
			{
				Datum d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::ExpectException<std::exception>([&d] { d.Get<glm::vec4>(10); });
				Assert::ExpectException<std::exception>([&d] { d.Get<std::string>(0); });
				Assert::AreEqual(d.Get<glm::vec4>(2), glm::vec4(3));

				const Datum d2 = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::ExpectException<std::exception>([&d2] { d2.Get<glm::vec4>(10); });
				Assert::ExpectException<std::exception>([&d2] { d2.Get<std::string>(0); });
				Assert::AreEqual(d2.Get<glm::vec4>(2), glm::vec4(3));
			}

			// Matrix
			{
				Datum d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::ExpectException<std::exception>([&d] { d.Get<glm::mat4>(10); });
				Assert::ExpectException<std::exception>([&d] { d.Get<std::string>(0); });
				Assert::AreEqual(d.Get<glm::mat4>(2), glm::mat4(3));

				const Datum d2 = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::ExpectException<std::exception>([&d2] { d2.Get<glm::mat4>(10); });
				Assert::ExpectException<std::exception>([&d2] { d2.Get<std::string>(0); });
				Assert::AreEqual(d2.Get<glm::mat4>(2), glm::mat4(3));
			}

			// String
			{
				Datum d = { "1"s, "2"s, "3"s };
				Assert::ExpectException<std::exception>([&d] { d.Get<std::string>(10); });
				Assert::ExpectException<std::exception>([&d] { d.Get<float>(0); });
				Assert::AreEqual(d.Get<std::string>(2), "3"s);

				const Datum d2 = { "1"s, "2"s, "3"s };
				Assert::ExpectException<std::exception>([&d2] { d2.Get<std::string>(10); });
				Assert::ExpectException<std::exception>([&d2] { d2.Get<float>(0); });
				Assert::AreEqual(d2.Get<std::string>(2), "3"s);
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3);
				Datum d = { &foo1, &foo2, &foo3 };
				Assert::ExpectException<std::exception>([&d] { d.Get<RTTI*>(10); });
				Assert::ExpectException<std::exception>([&d] { d.Get<float>(0); });
				Assert::AreEqual(d.Get<RTTI*>(2)->As<Foo>(), &foo3);

				const Datum d2 = { &foo1, &foo2, &foo3 };
				Assert::ExpectException<std::exception>([&d2] { d2.Get<RTTI*>(10); });
				Assert::ExpectException<std::exception>([&d2] { d2.Get<float>(0); });
				Assert::AreEqual(d2.Get<RTTI*>(1)->As<Foo>(), &foo2);
			}
		}

		TEST_METHOD(TestFront)
		{
			// Integer
			{
				Datum d = 1;
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Front<std::int32_t>(); });
				d = { 1, 2, 3 };
				Assert::ExpectException<std::exception>([&d] { d.Front<std::string>(); });
				Assert::AreEqual(d.Front<std::int32_t>(), 1);

				const Datum d2 = { 1, 2, 3 };
				Assert::ExpectException<std::exception>([&d2] { d2.Front<std::string>(); });
				Assert::AreEqual(d2.Front<std::int32_t>(), 1);
			}

			// Float
			{
				Datum d = 1.0f;
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Front<float>(); });
				d = { 1.0f, 2.0f, 3.0f };
				Assert::ExpectException<std::exception>([&d] { d.Front<std::string>(); });
				Assert::AreEqual(d.Front<float>(), 1.0f);

				const Datum d2 = { 1.0f, 2.0f, 3.0f };
				Assert::ExpectException<std::exception>([&d2] { d2.Front<std::string>(); });
				Assert::AreEqual(d2.Front<float>(), 1.0f);
			}

			// Vector
			{
				Datum d = glm::vec4(1);
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Front<glm::vec4>(); });
				d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::ExpectException<std::exception>([&d] { d.Front<std::string>(); });
				Assert::AreEqual(d.Front<glm::vec4>(), glm::vec4(1));

				const Datum d2 = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::ExpectException<std::exception>([&d2] { d2.Front<std::string>(); });
				Assert::AreEqual(d2.Front<glm::vec4>(), glm::vec4(1));
			}

			// Matrix
			{
				Datum d = glm::mat4(1);
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Front<glm::mat4>(); });
				d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::ExpectException<std::exception>([&d] { d.Front<std::string>(); });
				Assert::AreEqual(d.Front<glm::mat4>(), glm::mat4(1));

				const Datum d2 = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::ExpectException<std::exception>([&d2] { d2.Front<std::string>(); });
				Assert::AreEqual(d2.Front<glm::mat4>(), glm::mat4(1));
			}

			// String
			{
				Datum d = "1"s;
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Front<std::string>(); });
				d = { "1"s, "2"s, "3"s };
				Assert::ExpectException<std::exception>([&d] { d.Front<float>(); });
				Assert::AreEqual(d.Front<std::string>(), "1"s);

				const Datum d2 = { "1"s, "2"s, "3"s };
				Assert::ExpectException<std::exception>([&d2] { d2.Front<float>(); });
				Assert::AreEqual(d2.Front<std::string>(), "1"s);
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3);
				Datum d = &foo2;
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Front<std::string>(); });
				d = { &foo1, &foo2, &foo3 };
				Assert::ExpectException<std::exception>([&d] { d.Front<float>(); });
				Assert::AreEqual(d.Front<RTTI*>()->As<Foo>(), &foo1);

				const Datum d2 = { &foo1, &foo2, &foo3 };
				Assert::ExpectException<std::exception>([&d2] { d2.Front<float>(); });
				Assert::AreEqual(d2.Front<RTTI*>()->As<Foo>(), &foo1);
			}
		}

		TEST_METHOD(TestBack)
		{
			// Integer
			{
				Datum d = 1;
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Back<std::int32_t>(); });
				d = { 1, 2, 3 };
				Assert::ExpectException<std::exception>([&d] { d.Back<std::string>(); });
				Assert::AreEqual(d.Back<std::int32_t>(), 3);

				const Datum d2 = { 1, 2, 3 };
				Assert::ExpectException<std::exception>([&d2] { d2.Back<std::string>(); });
				Assert::AreEqual(d2.Back<std::int32_t>(), 3);
			}

			// Float
			{
				Datum d = 1.0f;
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Back<float>(); });
				d = { 1.0f, 2.0f, 3.0f };
				Assert::ExpectException<std::exception>([&d] { d.Back<std::string>(); });
				Assert::AreEqual(d.Back<float>(), 3.0f);

				const Datum d2 = { 1.0f, 2.0f, 3.0f };
				Assert::ExpectException<std::exception>([&d2] { d2.Back<std::string>(); });
				Assert::AreEqual(d2.Back<float>(), 3.0f);
			}

			// Vector
			{
				Datum d = glm::vec4(1);
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Back<glm::vec4>(); });
				d = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::ExpectException<std::exception>([&d] { d.Back<std::string>(); });
				Assert::AreEqual(d.Back<glm::vec4>(), glm::vec4(3));

				const Datum d2 = { glm::vec4(1), glm::vec4(2), glm::vec4(3) };
				Assert::ExpectException<std::exception>([&d2] { d2.Back<std::string>(); });
				Assert::AreEqual(d2.Back<glm::vec4>(), glm::vec4(3));
			}

			// Matrix
			{
				Datum d = glm::mat4(1);
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Back<glm::mat4>(); });
				d = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::ExpectException<std::exception>([&d] { d.Back<std::string>(); });
				Assert::AreEqual(d.Back<glm::mat4>(), glm::mat4(3));

				const Datum d2 = { glm::mat4(1), glm::mat4(2), glm::mat4(3) };
				Assert::ExpectException<std::exception>([&d2] { d2.Back<std::string>(); });
				Assert::AreEqual(d2.Back<glm::mat4>(), glm::mat4(3));
			}

			// String
			{
				Datum d = "1"s;
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Back<std::string>(); });
				d = { "1"s, "2"s, "3"s };
				Assert::ExpectException<std::exception>([&d] { d.Back<float>(); });
				Assert::AreEqual(d.Back<std::string>(), "3"s);

				const Datum d2 = { "1"s, "2"s, "3"s };
				Assert::ExpectException<std::exception>([&d2] { d2.Back<float>(); });
				Assert::AreEqual(d2.Back<std::string>(), "3"s);
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3);
				Datum d = &foo2;
				d.Clear();
				Assert::ExpectException<std::exception>([&d] { d.Back<std::string>(); });
				d = { &foo1, &foo2, &foo3 };
				Assert::ExpectException<std::exception>([&d] { d.Back<float>(); });
				Assert::AreEqual(d.Back<RTTI*>()->As<Foo>(), &foo3);

				const Datum d2 = { &foo1, &foo2, &foo3 };
				Assert::ExpectException<std::exception>([&d2] { d2.Back<float>(); });
				Assert::AreEqual(d2.Back<RTTI*>()->As<Foo>(), &foo3);
			}
		}

		TEST_METHOD(TestIterators)
		{
			// Integer
			{
				Datum d = { 1, 2, 3, 4, 5 };
				int c = 1;
				for (const auto& ptr : d)
				{
					switch (d.Type())
					{
					case Datum::DatumType::INTEGER:
						Assert::AreEqual(*(reinterpret_cast<std::int32_t*>(ptr)), c);
						break;
					case Datum::DatumType::FLOAT:
					case Datum::DatumType::VECTOR:
					case Datum::DatumType::MATRIX:
					case Datum::DatumType::STRING:
					case Datum::DatumType::POINTER:
					case Datum::DatumType::UNKNOWN:
					default:
						Assert::Fail();
						break;
					}

					++c;
				}

				Datum::Iterator t;
				Assert::ExpectException<std::exception>([&t] { ++t; });
			}

			// Float
			{
				Datum d = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
				float c = 1.0f;
				for (const auto& ptr : d)
				{
					switch (d.Type())
					{
					case Datum::DatumType::FLOAT:
						Assert::AreEqual(*(reinterpret_cast<float*>(ptr)), c);
						break;
					case Datum::DatumType::INTEGER:
					case Datum::DatumType::VECTOR:
					case Datum::DatumType::MATRIX:
					case Datum::DatumType::STRING:
					case Datum::DatumType::POINTER:
					case Datum::DatumType::UNKNOWN:
					default:
						Assert::Fail();
						break;
					}

					++c;
				}
			}

			// Vector
			{
				Datum d = { glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4(), glm::vec4() };
				for (const auto& ptr : d)
				{
					switch (d.Type())
					{
					case Datum::DatumType::VECTOR:
						Assert::AreEqual(*(reinterpret_cast<glm::vec4*>(ptr)), glm::vec4());
						break;
					case Datum::DatumType::INTEGER:
					case Datum::DatumType::FLOAT:
					case Datum::DatumType::MATRIX:
					case Datum::DatumType::STRING:
					case Datum::DatumType::POINTER:
					case Datum::DatumType::UNKNOWN:
					default:
						Assert::Fail();
						break;
					}
				}
			}

			// Matrix
			{
				Datum d = { glm::mat4(), glm::mat4(), glm::mat4(), glm::mat4(), glm::mat4() };
				for (const auto& ptr : d)
				{
					switch (d.Type())
					{
					case Datum::DatumType::MATRIX:
						Assert::AreEqual(*(reinterpret_cast<glm::mat4*>(ptr)), glm::mat4());
						break;
					case Datum::DatumType::VECTOR:
					case Datum::DatumType::INTEGER:
					case Datum::DatumType::FLOAT:
					case Datum::DatumType::STRING:
					case Datum::DatumType::POINTER:
					case Datum::DatumType::UNKNOWN:
					default:
						Assert::Fail();
						break;
					}
				}
			}

			// String
			{
				Datum d = { "1"s, "2"s, "3"s, "4"s, "5"s };
				int c = 1;
				for (const auto& ptr : d)
				{
					switch (d.Type())
					{
					case Datum::DatumType::STRING:
						Assert::AreEqual(*(reinterpret_cast<std::string*>(ptr)), std::to_string(c));
						break;
					case Datum::DatumType::INTEGER:
					case Datum::DatumType::FLOAT:
					case Datum::DatumType::VECTOR:
					case Datum::DatumType::MATRIX:
					case Datum::DatumType::POINTER:
					case Datum::DatumType::UNKNOWN:
					default:
						Assert::Fail();
						break;
					}

					++c;
				}
			}

			// RTTI
			{
				Foo foo1(1), foo2(2), foo3(3), foo4(4), foo5(5);
				Datum d = { &foo1, &foo2, &foo3, &foo4, &foo5 };
				int c = 1;
				for (const auto& ptr : d)
				{
					switch (d.Type())
					{
					case Datum::DatumType::POINTER:
						Assert::AreEqual(*((*(reinterpret_cast<RTTI**>(ptr)))->As<Foo>()), Foo(c));
						break;
					case Datum::DatumType::STRING:
					case Datum::DatumType::INTEGER:
					case Datum::DatumType::FLOAT:
					case Datum::DatumType::VECTOR:
					case Datum::DatumType::MATRIX:
					case Datum::DatumType::UNKNOWN:
					default:
						Assert::Fail();
						break;
					}

					++c;
				}
			}

			// Const
			{
				const Datum d;
				Assert::ExpectException<std::exception>([&d] { *(d.begin()); });
				Assert::ExpectException<std::exception>([&d] { *(d.end()); });
				Assert::ExpectException<std::exception>([] { Datum::ConstIterator().Type(); });
				Assert::AreEqual(d.begin().Type(), Datum::DatumType::UNKNOWN);
				Datum d2(1);
				Assert::AreEqual(++(d2.cbegin()), d2.cend());
				Assert::AreEqual((d2.cbegin())++, d2.cbegin());
				Datum::ConstIterator(d2.begin());
				Datum::Iterator it = d2.begin();
				Datum::ConstIterator it2(it);
				it++;
				Datum::ConstIterator cit;
				cit = it;
				cit = std::move(it);

				Assert::ExpectException<std::exception>([&d2] { d2.Remove(Datum::Iterator()); });
			}
		}

		TEST_METHOD(TestSetType)
		{
			Datum d;
			d.SetType(Datum::DatumType::INTEGER);
			Assert::ExpectException<std::exception>([&d] { d.SetType(Datum::DatumType::STRING); });
			Assert::ExpectException<std::exception>([&d] { d.SetType(Datum::DatumType::UNKNOWN); });
		}

		TEST_METHOD(TestToString)
		{
			// Integer
			{
				Datum d;
				Assert::ExpectException<std::exception>([&d] { d.ToString(); });
				d = 1;
				Assert::AreEqual(d.ToString(), std::to_string(1));
			}

			// Float
			{
				Datum d = 1.0f;
				Assert::AreEqual(d.ToString(), std::to_string(1.0f));
			}

			// Vector
			{
				Datum d = glm::vec4(1);
				Assert::AreEqual(d.ToString(), glm::to_string(glm::vec4(1)));
			}

			// Matrix
			{
				Datum d = glm::mat4(1);
				Assert::AreEqual(d.ToString(), glm::to_string(glm::mat4(1)));
			}

			// String
			{
				Datum d = "hello"s;
				Assert::AreEqual(d.ToString(), "hello"s);
			}

			// RTTI
			{
				Foo foo;
				Datum d = &foo;
				Assert::AreEqual(d.ToString(), foo.ToString());
			}
		}

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState DatumTest::sStartMemState;
}