#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Scope.h"
#include "ToStringSpecializations.h"
#include "Utility.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTest)
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
			Scope s;
			RTTI* p = &s;
			Assert::IsTrue(p->Is(Scope::TypeName()));
			Assert::IsFalse(p->Is(Foo::TypeName()));
			Assert::IsNotNull(p->As<Scope>());
			Assert::IsNull(p->As<Foo>());
			Assert::IsTrue(p->Is(Scope::TypeIdClass()));
			Assert::IsFalse(p->Is(Foo::TypeIdClass()));
		}

		TEST_METHOD(TestConstructor)
		{
			Scope s;
			Assert::AreEqual(s.Size(), 0_z);
			Assert::IsNull(s.GetParent());
		}

		TEST_METHOD(TestCopyConstructor)
		{
			Scope s1;
			s1.Append("field1"s);
			s1.Append("field2"s);
			s1.AppendScope("table1"s).AppendScope("table1.table1"s);
			s1.AppendScope("table2"s).Append("table2.field1"s);
			Assert::AreEqual(s1.Size(), 4_z);

			Scope s2 = s1;
			Assert::AreEqual(s2.Size(), 4_z);
			Assert::AreEqual(s1, s2);
			Scope* s = nullptr;
			Datum* d = s2["table2"s][0].Search("table2.field1"s, &s);
			Assert::IsNotNull(s);
			Assert::IsNotNull(d);
			d->PushBack(100);
			Assert::AreNotEqual(s1, s2);
		}

		TEST_METHOD(TestMoveConstructor)
		{
			Scope s1;
			s1.Append("field1"s);
			s1.Append("field2"s);
			s1.AppendScope("table1"s).AppendScope("table1.table1"s);
			s1.AppendScope("table2"s).Append("table2.field1"s);
			Assert::AreEqual(s1.Size(), 4_z);

			Scope s2 = std::move(s1);
			Assert::AreEqual(s2.Size(), 4_z);
			Assert::AreNotEqual(s1, s2);
			Scope* s = nullptr;
			Datum* d = s2["table2"s][0].Search("table2.field1"s, &s);
			Assert::IsNotNull(s);
			Assert::IsNotNull(d);

			Scope& s4 = s2.AppendScope("child"s);
			Assert::IsNotNull(s2.FindChildScope(&s4).first);
			Scope s3 = std::move(s4);
			Assert::IsNull(s2.FindChildScope(&s4).first);
			Assert::IsNotNull(s2.FindChildScope(&s3).first);
		}

		TEST_METHOD(TestCopyAssignment)
		{
			Scope s1;
			s1.Append("f1"s);
			s1.Append("f2"s);
			s1.AppendScope("t1"s).AppendScope("t1.t1"s);
			s1.AppendScope("t2"s).Append("t2.f1"s);

			Scope* s = nullptr;
			Datum* d = s1["t2"s][0].Search("t2.f1"s, &s);
			Assert::IsNotNull(s);
			Assert::IsNotNull(d);

			Scope s2;
			s2.Append("health"s).PushBack(100);
			s2["armor"s] = 100.0f;
			s2.AppendScope("t1"s).AppendScope("t1.t1"s);

			Assert::AreNotEqual(s1, s2);
			s1 = s2;
			s = nullptr;
			d = s1.Search("t2"s, &s);
			Assert::IsNull(s);
			Assert::IsNull(d);
		}

		TEST_METHOD(TestMoveAssignment)
		{
			Scope s1;
			s1.Append("f1"s);
			s1.Append("f2"s);
			s1.AppendScope("t1"s).AppendScope("t1.t1"s);
			s1.AppendScope("t2"s).Append("t2.f1"s);

			Scope* s = nullptr;
			Datum* d = s1["t2"s][0].Search("t2.f1"s, &s);
			Assert::IsNotNull(s);
			Assert::IsNotNull(d);

			Scope s2;
			s2.Append("health"s).PushBack(100);
			s2["armor"s] = 100.0f;
			s2.AppendScope("t1"s).AppendScope("t1.t1"s);

			Assert::AreNotEqual(s1, s2);
			s1 = std::move(s2);
			Assert::AreNotEqual(s1, s2);
			s = nullptr;
			d = s1.Search("t2"s, &s);
			Assert::IsNull(s);
			Assert::IsNull(d);
			s = nullptr;
			d = s1.Search("health"s, &s);
			Assert::IsNotNull(s);
			Assert::IsNotNull(d);

			Scope s3;
			Scope& s4 = s1.AppendScope("child"s);
			Assert::IsNotNull(s1.FindChildScope(&s4).first);
			s3 = std::move(s4);
			Assert::IsNull(s1.FindChildScope(&s4).first);
			Assert::IsNotNull(s1.FindChildScope(&s3).first);
		}

		TEST_METHOD(TestDestructor)
		{
			Scope s;
			s.Append("health"s);
			Scope* child = new Scope();
			s.Append("powers"s);
			s.AppendScope("powers"s);
			Assert::AreEqual(s["powers"s].Size(), 1_z);
			s.Adopt("powers"s, *child);
			Assert::AreEqual(s["powers"s].Size(), 2_z);
			delete child;
			Assert::AreEqual(s["powers"s].Size(), 1_z);
		}

		TEST_METHOD(TestComparison)
		{
			Scope s1;
			Scope s2;
			Assert::AreEqual(s1, s2);
			s1.Append("health"s).PushBack(100.0f);
			Assert::IsTrue(s1 != s2);
			s2.Append("health"s).PushBack(100.0f);
			Assert::AreEqual(s1, s2);
			s1.Append("position"s).PushBack(glm::vec4());
			s2.Append("name"s).PushBack("NPC"s);
			Assert::IsTrue(s1 != s2);
			Assert::IsFalse(s1.Equals(nullptr));
		}

		TEST_METHOD(TestIndexOperator)
		{
			// string name parameter
			{
				Scope s;
				Assert::AreEqual(s.Size(), 0_z);
				s["health"s].PushBack(100.0f);
				Assert::AreEqual(s.Size(), 1_z);
				Assert::AreEqual(s["health"s].Get<float>(), 100.0f);

				Scope s2;
				Assert::AreEqual(s2.Size(), 0_z);
				s2.Append("health"s);
				Assert::AreEqual(s2.Size(), 1_z);
				s2["health"s].PushBack(100.0f);
				Assert::AreEqual(s2.Size(), 1_z);
				Assert::AreEqual(s2["health"s].Get<float>(), 100.0f);
			}

			// index parameter
			{
				Scope s;
				Assert::ExpectException<std::exception>([&s] { s[0]; });
				s.Append("health"s).PushBack(100.0f);
				Assert::AreEqual(s[0].Get<float>(), 100.0f);

				const Scope s2 = s;
				Assert::AreEqual(s2[0].Get<float>(), 100.0f);
			}
		}

		TEST_METHOD(TestAt)
		{
			Scope s;
			Assert::ExpectException<std::exception>([&s] { s.At(0); });
			s.Append("health"s).PushBack(100.0f);
			Assert::AreEqual(s.At(0).Get<float>(), 100.0f);

			const Scope s2 = s;
			Assert::AreEqual(s2.At(0).Get<float>(), 100.0f);
		}

		TEST_METHOD(TestAppend)
		{
			// Copy
			{
				std::string h = "health"s;
				Scope s;
				Assert::AreEqual(s.Size(), 0_z);
				s.Append(h).PushBack(100.0f);
				Assert::AreEqual(s.Size(), 1_z);
				Assert::AreEqual(s.Append(h).Get<float>(), 100.0f);

				Scope s2;
				Assert::AreEqual(s2.Size(), 0_z);
				s2.Append(h);
				Assert::AreEqual(s2.Size(), 1_z);
				s2.Append(h).PushBack(100.0f);
				Assert::AreEqual(s2.Size(), 1_z);
				Assert::AreEqual(s2.Append(h).Get<float>(), 100.0f);
			}

			// Move
			{
				Scope s;
				Assert::AreEqual(s.Size(), 0_z);
				s.Append("health"s).PushBack(100.0f);
				Assert::AreEqual(s.Size(), 1_z);
				Assert::AreEqual(s.Append("health"s).Get<float>(), 100.0f);

				Scope s2;
				Assert::AreEqual(s2.Size(), 0_z);
				s2.Append("health"s);
				Assert::AreEqual(s2.Size(), 1_z);
				s2.Append("health"s).PushBack(100.0f);
				Assert::AreEqual(s2.Size(), 1_z);
				Assert::AreEqual(s2.Append("health"s).Get<float>(), 100.0f);
			}

			// empty name
			{
				Scope s;
				Assert::ExpectException<std::exception>([&s] { s.Append(""s); });
			}

			// bool reference
			{
				Scope s;
				bool inserted = false;
				s.Append("new"s, inserted);
				Assert::IsTrue(inserted);
				s.Append("new"s, inserted);
				Assert::IsFalse(inserted);
			}
		}

		TEST_METHOD(TestAppendScope)
		{
			// Copy
			{
				std::string name = "powers"s;
				Scope s;
				Assert::AreEqual(s.Size(), 0_z);
				s.AppendScope(name);
				Assert::AreEqual(s.Size(), 1_z);
				Assert::AreEqual(s[name].Size(), 1_z);

				Scope s2;
				Assert::AreEqual(s2.Size(), 0_z);
				s2.AppendScope(name);
				Assert::AreEqual(s2.Size(), 1_z);
				Assert::AreEqual(s[name].Size(), 1_z);
				s2.AppendScope(name);
				Assert::AreEqual(s2.Size(), 1_z);
				Assert::AreEqual(s2[name].Size(), 2_z);
			}

			// Move
			{
				Scope s;
				Assert::AreEqual(s.Size(), 0_z);
				s.AppendScope("health"s);
				Assert::AreEqual(s.Size(), 1_z);
				Assert::AreEqual(s["health"s].Size(), 1_z);

				Scope s2;
				Assert::AreEqual(s2.Size(), 0_z);
				s2.AppendScope("health"s);
				Assert::AreEqual(s2.Size(), 1_z);
				Assert::AreEqual(s["health"s].Size(), 1_z);
				s2.AppendScope("health"s);
				Assert::AreEqual(s2.Size(), 1_z);
				Assert::AreEqual(s2["health"s].Size(), 2_z);
			}

			// exceptions
			{
				Scope s;
				s.Append("integer"s).PushBack(100);
				Assert::ExpectException<std::exception>([&s] { s.AppendScope("integer"s); });
				auto string = "integer"s;
				Assert::ExpectException<std::exception>([&s, &string] { s.AppendScope(string); });
			}

			// datum
			{
				Scope s;
				Scope& child = s.AppendScope("child"s);
				Assert::AreNotEqual(s["child"s].Find(child), s["child"s].end());
				Assert::AreEqual(s["child"s].Find(s), s["child"s].end());
			}
		}

		TEST_METHOD(TestFind)
		{
			Scope s;
			s.Append("name"s);
			s.Append("health"s);
			s.Append("armor"s);
			Assert::IsNotNull(s.Find("name"s));
			Assert::IsNotNull(s.Find("health"s));
			Assert::IsNotNull(s.Find("armor"s));
			Assert::IsNull(s.Find("error"s));

			const Scope cs = s;
			Assert::IsNotNull(cs.Find("armor"s));
			Assert::IsNull(cs.Find("error"s));
		}

		TEST_METHOD(TestFindChildScope)
		{
			Scope s;
			Scope* child1 = new Scope();
			s.Adopt("powers"s, *child1);
			auto[d1, i1] = s.FindChildScope(child1);
			Assert::IsNotNull(d1);
			Scope* child2 = new Scope();
			auto[d2, i2] = s.FindChildScope(child2);
			Assert::IsNull(d2);
			child1->Adopt("subpowers"s, *child2);
			auto[d3, i3] = child1->FindChildScope(child2);
			Assert::IsNotNull(d3);

			const Scope cs = s;
			auto[d4, i4] = cs.FindChildScope(child1);
			Assert::IsNull(d4);
		}

		TEST_METHOD(TestSearch)
		{
			Scope s; // level 0
			s.Append("datum1"s);
			s.Append("datum2"s);
			s.Append("datum3"s);
			s.Append("datum4"s);
			Scope& l1 = s.AppendScope("level1"s);
			l1.Append("datum1"s);
			l1.Append("datum2"s);
			l1.Append("datum3"s);
			Scope& l2 = l1.AppendScope("level2"s);
			l2.Append("datum1"s);
			l2.Append("datum2"s);
			Scope& l3 = l2.AppendScope("level3"s);
			l3.Append("datum1"s);

			Assert::AreNotSame(*l2.Search("datum1"s), *l3.Search("datum1"s));
			Assert::AreSame(*l2.Search("datum2"s), *l3.Search("datum2"s));
			Assert::AreSame(*l2.Search("datum4"s), *l3.Search("datum4"s));
			Assert::AreNotSame(*s.Search("datum1"s), *l3.Search("datum1"s));
			Assert::IsNull(s.Search("datum5"s));
			Assert::IsNull(l3.Search("datum5"s));

			Scope* scope = nullptr;
			Assert::IsNotNull(l3.Search("datum4"s, &scope));
			Assert::IsNotNull(scope);

			const Scope cs = s;
			Assert::IsNotNull(cs.Search("datum1"s));

			Scope* out;
			Assert::IsNull(l3.Search("datum5"s, &out));
			Assert::IsNull(out);
		}

		TEST_METHOD(TestFindName)
		{
			Scope s;
			Scope& scope = s.AppendScope("table"s);
			Assert::AreEqual(s.FindName(&scope), "table"s);
			Scope error;
			Assert::AreEqual(s.FindName(&error), ""s);
		}

		TEST_METHOD(TestAdopt)
		{
			{
				Scope s1;
				s1.AppendScope("t1"s).AppendScope("t2"s);
				Scope s2 = s1;
				Assert::AreEqual(s1, s2);
				Assert::AreEqual(s1.Size(), 1_z);
				Assert::AreEqual(s2["t1"s].Size(), 1_z);
				s1.Adopt("t2"s, s2["t1"s][0]);
				Assert::AreNotEqual(s1, s2);
				Assert::AreEqual(s1.Size(), 2_z);
				Assert::AreEqual(s2["t1"s].Size(), 0_z);

				Scope* ptr = new Scope();
				s1.Adopt("t1"s, *ptr);
				Assert::AreEqual(s1.Size(), 2_z);
				Assert::AreEqual(s1["t1"s].Size(), 2_z);
			}

			// exceptions
			{
				Scope s;
				Assert::ExpectException<std::exception>([&s] { s.Adopt("error"s, s); });

				Scope& child = s.AppendScope("child"s);
				Assert::ExpectException<std::exception>([&s, &child] { child.Adopt("error"s, s); });

				s.Append("health"s).PushBack(100.0f);
				Scope* t = new Scope();
				Assert::ExpectException<std::exception>([&s, &t] { s.Adopt("health"s, *t); });
				s.Adopt("table"s, *t);
			}
		}

		TEST_METHOD(TestGetParent)
		{
			Scope s;
			Scope* c = new Scope();
			Assert::IsNull(c->GetParent());
			s.Adopt("table"s, *c);
			Assert::AreEqual(&s, c->GetParent());

			const Scope cs;
			Assert::IsNull(cs.GetParent());

			const Scope scope = s;
			Assert::IsNotNull(scope.Find("table"s)->operator[](0).GetParent());
		}

		TEST_METHOD(TestIsAncestor)
		{
			Scope parent;
			Scope& child = parent.AppendScope("childs"s);
			Assert::IsTrue(child.IsAncestor(parent));
			Scope& sibling = parent.AppendScope("childs"s);
			Assert::IsTrue(sibling.IsAncestor(parent));

			Scope& grandchild = child.AppendScope("childs"s);
			Assert::IsTrue(grandchild.IsAncestor(parent));
			Assert::IsFalse(grandchild.IsAncestor(sibling));
		}

		TEST_METHOD(TestClear)
		{
			Scope s;
			Assert::AreEqual(s.Size(), 0_z);
			s.Append("1"s);
			s.Append("2"s);
			Assert::AreEqual(s.Size(), 2_z);
			s.Clear();
			Assert::AreEqual(s.Size(), 0_z);
		}

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState ScopeTest::sStartMemState;
}
