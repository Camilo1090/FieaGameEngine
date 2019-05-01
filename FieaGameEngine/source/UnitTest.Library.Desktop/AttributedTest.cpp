#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"
#include "Attributed.h"
#include "TypeManager.h"
#include "ToStringSpecializations.h"
#include "Utility.h"
#include <string>
#include <tuple>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTest)
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
			AttributedFoo s;
			RTTI* p = &s;
			Assert::IsTrue(p->Is(AttributedFoo::TypeName()));
			Assert::IsFalse(p->Is(Foo::TypeName()));
			Assert::IsNotNull(p->As<AttributedFoo>());
			Assert::IsNull(p->As<Foo>());
			Assert::IsTrue(p->Is(AttributedFoo::TypeIdClass()));
			Assert::IsFalse(p->Is(Foo::TypeIdClass()));
		}

		TEST_METHOD(TestConstructor)
		{
			AttributedFoo foo;
			Assert::AreEqual(foo.Size(), TypeManager::GetSignatureCount(foo.TypeIdInstance()) + 1);
			Assert::IsTrue(foo["this"s] == &foo);
			for (const auto& signature : TypeManager::GetSignatures(foo.TypeIdInstance()))
			{
				Assert::IsNotNull(foo.Find(signature.Name));
			}
		}

		TEST_METHOD(TestCopyConstructor)
		{
			AttributedFoo foo1;
			foo1.ExternalFloat = 1.5f;
			AttributedFoo foo2 = foo1;
			Assert::AreEqual(foo1, foo2);
			foo2.ExternalInteger = 10;
			Assert::AreNotEqual(foo1, foo2);

			foo1.AppendScope("child1"s);
			AttributedFoo foo3 = foo1;
			Assert::AreEqual(foo1, foo3);
			Assert::IsNotNull(foo3.Find("child1"s));
			foo3["child1"s][0].Append("nested"s);
			Assert::AreNotEqual(foo1, foo3);
		}

		TEST_METHOD(TestMoveConstructor)
		{
			AttributedFoo foo1;
			foo1.ExternalFloat = 1.5f;
			AttributedFoo foo2 = std::move(foo1);
			Assert::AreNotEqual(foo1, foo2);
			Assert::IsTrue(foo2["ExternalFloat"s] == 1.5f);

			foo2.AppendScope("child"s);
			AttributedFoo foo3 = std::move(foo2);
			Assert::AreNotEqual(foo2, foo3);
			Assert::IsNotNull(foo3.Find("child"s));
		}

		TEST_METHOD(TestCopyAssignment)
		{
			AttributedFoo foo1;
			foo1["ExternalString"s] = "Hello"s;
			AttributedFoo foo2;
			Assert::AreNotEqual(foo1, foo2);
			foo2 = foo1;
			Assert::AreEqual(foo1, foo2);
			foo2["ExternalString"s] = "World"s;
			Assert::AreNotEqual(foo1, foo2);

			foo1.AppendScope("child1"s);
			foo2.AppendScope("child2"s);
			foo1 = foo2;
			Assert::AreEqual(foo1, foo2);
			Assert::IsNotNull(foo1.Find("child2"s));
			Assert::IsNotNull(foo2.Find("child2"s));
			Assert::IsNull(foo1.Find("child1"s));
			Assert::IsNull(foo2.Find("child1"s));
			foo1["child2"s][0].Append("nested"s);
			Assert::AreNotEqual(foo1, foo2);
		}

		TEST_METHOD(TestMoveAssignment)
		{
			AttributedFoo foo1;
			foo1["ExternalString"s] = "Hello"s;
			AttributedFoo foo2;
			Assert::AreNotEqual(foo1, foo2);
			foo2 = std::move(foo1);
			Assert::AreNotEqual(foo1, foo2);
			Assert::IsTrue(foo2["ExternalString"s] == "Hello"s);

			foo2.AppendScope("child2"s);
			AttributedFoo foo3;
			foo3.AppendScope("child3"s);
			foo2 = std::move(foo3);
			Assert::AreNotEqual(foo2, foo3);
			Assert::IsNotNull(foo2.Find("child3"s));
			Assert::IsNull(foo2.Find("child2"s));
		}

		TEST_METHOD(TestDestructor)
		{
			AttributedFoo foo;
		}

		TEST_METHOD(TestComparison)
		{
			AttributedFoo foo1;
			AttributedFoo foo2;
			Assert::AreEqual(foo1, foo2);
			foo1["ExternalInteger"s] = 10;
			Assert::AreNotEqual(foo1, foo2);
			foo2 = foo1;
			Assert::AreEqual(foo1, foo2);
			foo2.AppendAuxiliaryAttribute("Aux"s);
			Assert::IsTrue(foo1 != foo2);
			const Attributed& ref1 = foo1;
			const Attributed& ref2 = foo2;
			Assert::IsTrue(ref1 != ref2);
		}

		TEST_METHOD(TestAppendAuxiliary)
		{
			{
				AttributedFoo foo;
				size_t size = foo.Size();
				Assert::IsNull(foo.Find("Aux"s));
				foo.AppendAuxiliaryAttribute("Aux"s);
				Assert::AreEqual(foo.Size(), size + 1);
				Assert::IsNotNull(foo.Find("Aux"s));
				foo.AppendAuxiliaryAttribute("Aux"s);
				Assert::AreEqual(foo.Size(), size + 1);
				Assert::ExpectException<std::exception>([&foo] { foo.AppendAuxiliaryAttribute("this"s); });
				for (const auto& signature : TypeManager::GetSignatures(foo.TypeIdInstance()))
				{
					Assert::ExpectException<std::exception>([&foo, &signature] { foo.AppendAuxiliaryAttribute(signature.Name); });
				}
			}

			{
				AttributedFoo foo;
				size_t size = foo.Size();
				Assert::IsNull(foo.Find("Aux"s));
				foo.AppendAuxiliaryAttribute("Aux"s, Datum(10));
				Assert::AreEqual(foo.Size(), size + 1);
				Assert::IsNotNull(foo.Find("Aux"s));
				foo.AppendAuxiliaryAttribute("Aux"s, Datum(11));
				Assert::AreEqual(foo.Size(), size + 1);
				Assert::ExpectException<std::exception>([&foo] { foo.AppendAuxiliaryAttribute("this"s); });
				for (const auto& signature : TypeManager::GetSignatures(foo.TypeIdInstance()))
				{
					Assert::ExpectException<std::exception>([&foo, &signature] { foo.AppendAuxiliaryAttribute(signature.Name); });
				}
			}

			{
				AttributedFoo foo;
				size_t size = foo.Size();
				const std::pair<const std::string, Datum>& pair = std::make_pair("Aux"s, Datum(10));
				Assert::IsNull(foo.Find(pair.first));
				foo.AppendAuxiliaryAttribute(pair);
				Assert::AreEqual(foo.Size(), size + 1);
				Assert::IsNotNull(foo.Find(pair.first));
				foo.AppendAuxiliaryAttribute(pair);
				Assert::AreEqual(foo.Size(), size + 1);
				Assert::ExpectException<std::exception>([&foo] { foo.AppendAuxiliaryAttribute("this"s); });
				for (const auto& signature : TypeManager::GetSignatures(foo.TypeIdInstance()))
				{
					Assert::ExpectException<std::exception>([&foo, &signature] { foo.AppendAuxiliaryAttribute(signature.Name); });
				}
			}

			{
				AttributedFoo foo;
				size_t size = foo.Size();
				Assert::IsNull(foo.Find("Aux"s));
				foo.AppendAuxiliaryAttribute(std::pair("Aux"s, Datum(10)));
				Assert::AreEqual(foo.Size(), size + 1);
				Assert::IsNotNull(foo.Find("Aux"s));
				foo.AppendAuxiliaryAttribute(std::pair("Aux"s, Datum(11)));
				Assert::AreEqual(foo.Size(), size + 1);
				Assert::ExpectException<std::exception>([&foo] { foo.AppendAuxiliaryAttribute("this"s); });
				for (const auto& signature : TypeManager::GetSignatures(foo.TypeIdInstance()))
				{
					Assert::ExpectException<std::exception>([&foo, &signature] { foo.AppendAuxiliaryAttribute(signature.Name); });
				}
			}
		}

		TEST_METHOD(TestIsAttribute)
		{
			AttributedFoo foo;
			Assert::IsTrue(foo.IsAttribute("this"s));
			for (const auto& signature : TypeManager::GetSignatures(foo.TypeIdInstance()))
			{
				Assert::IsTrue(foo.IsAttribute(signature.Name));
			}
			Assert::IsFalse(foo.IsAttribute("name"s));
			foo.AppendAuxiliaryAttribute("name"s) = "Main"s;
			Assert::IsTrue(foo.IsAttribute("name"s));
		}

		TEST_METHOD(TestIsPrescribedAttribute)
		{
			AttributedFoo foo;
			Assert::IsTrue(foo.IsPrescribedAttribute("this"s));
			for (const auto& signature : TypeManager::GetSignatures(foo.TypeIdInstance()))
			{
				Assert::IsTrue(foo.IsPrescribedAttribute(signature.Name));
			}
			Assert::IsFalse(foo.IsPrescribedAttribute("name"s));
			foo.AppendAuxiliaryAttribute("name"s) = "Main"s;
			Assert::IsFalse(foo.IsPrescribedAttribute("name"s));
		}

		TEST_METHOD(TestIsAuxiliaryAttribute)
		{
			AttributedFoo foo;
			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"s));
			for (const auto& signature : TypeManager::GetSignatures(foo.TypeIdInstance()))
			{
				Assert::IsFalse(foo.IsAuxiliaryAttribute(signature.Name));
			}
			Assert::IsFalse(foo.IsAuxiliaryAttribute("name"s));
			foo.AppendAuxiliaryAttribute("name"s) = "Main"s;
			Assert::IsTrue(foo.IsAuxiliaryAttribute("name"s));
		}

		TEST_METHOD(TestGetAttributes)
		{
			AttributedFoo foo;
			auto attrs = foo.Attributes();
			Assert::IsTrue(attrs.Size() > 0);
			Assert::AreEqual(attrs[0]->first, "this"s);
			Assert::IsTrue(attrs[0]->second == &foo);
			for (size_t i = 1; i < attrs.Size(); ++i)
			{
				Assert::IsTrue(foo.IsAttribute(attrs[i]->first));
			}

			const AttributedFoo cfoo = foo;
			auto cattrs = cfoo.Attributes();
			Assert::IsTrue(cattrs.Size() > 0);
			Assert::AreEqual(cattrs[0]->first, "this"s);
			Assert::IsTrue(cattrs[0]->second == &cfoo);
			for (size_t i = 1; i < cattrs.Size(); ++i)
			{
				Assert::IsTrue(cfoo.IsAttribute(cattrs[i]->first));
			}
		}

		TEST_METHOD(TestGetPrescribedAttributes)
		{
			AttributedFoo foo;
			auto attrs = foo.PrescribedAttributes();
			Assert::IsTrue(attrs.Size() > 0);
			Assert::AreEqual(attrs[0]->first, "this"s);
			Assert::IsTrue(attrs[0]->second == &foo);
			for (size_t i = 1; i < attrs.Size(); ++i)
			{
				Assert::IsTrue(foo.IsPrescribedAttribute(attrs[i]->first));
			}

			const AttributedFoo cfoo = foo;
			auto cattrs = cfoo.PrescribedAttributes();
			Assert::IsTrue(cattrs.Size() > 0);
			Assert::AreEqual(cattrs[0]->first, "this"s);
			Assert::IsTrue(cattrs[0]->second == &foo);
			for (size_t i = 1; i < cattrs.Size(); ++i)
			{
				Assert::IsTrue(cfoo.IsPrescribedAttribute(cattrs[i]->first));
			}
		}

		TEST_METHOD(TestGetAuxiliaryAttributes)
		{
			AttributedFoo foo;
			foo.AppendAuxiliaryAttribute("1"s);
			foo.AppendAuxiliaryAttribute("2"s);
			foo.AppendAuxiliaryAttribute("3"s);
			auto attrs = foo.AuxiliaryAttributes();
			Assert::AreEqual(attrs.Size(), 3_z);
			for (size_t i = 0; i < attrs.Size(); ++i)
			{
				Assert::IsTrue(foo.IsAuxiliaryAttribute(attrs[i]->first));
			}

			const AttributedFoo cfoo = foo;
			auto cattrs = cfoo.AuxiliaryAttributes();
			Assert::AreEqual(cattrs.Size(), 3_z);
			for (size_t i = 0; i < cattrs.Size(); ++i)
			{
				Assert::IsTrue(cfoo.IsAuxiliaryAttribute(cattrs[i]->first));
			}
		}

		TEST_METHOD(TestClear)
		{
			AttributedFoo foo;
			foo.AppendAuxiliaryAttribute("1"s);
			foo.AppendAuxiliaryAttribute("2"s);
			foo.AppendAuxiliaryAttribute("3"s);
			Assert::AreEqual(foo.Attributes().Size(), TypeManager::GetSignatures(foo.TypeIdInstance()).Size() + 4);
			foo.Clear();
			Assert::AreEqual(foo.Attributes().Size(), TypeManager::GetSignatures(foo.TypeIdInstance()).Size() + 1);
			for (auto attr : foo.Attributes())
			{
				Assert::IsTrue(foo.IsPrescribedAttribute(attr->first));
			}
		}

		TEST_METHOD(TestClone)
		{
			AttributedFoo foo1;
			AttributedFoo& foo2 = *foo1.Clone()->As<AttributedFoo>();
			Assert::AreEqual(foo1, foo2);
			foo2.ExternalInteger = 10;
			Assert::AreNotEqual(foo1, foo2);
			delete &foo2;
		}

		TEST_METHOD(TestClearAuxiliary)
		{
			AttributedFoo foo1;
			Assert::IsNull(foo1.Find("A"s));
			foo1.Append("A");
			Assert::IsNotNull(foo1.Find("A"s));
			foo1.ClearAuxiliaryAttributes();
			Assert::IsNull(foo1.Find("A"s));
		}

		TEST_METHOD(TestEquals)
		{
			TypeManager::RegisterType(AttributedBar::TypeIdClass(), AttributedBar::Signatures(), AttributedBar::ParentTypeIdClass());

			AttributedFoo foo1;
			AttributedFoo foo2;
			Assert::IsTrue(foo1.Equals(&foo2));
			foo2["ExternalFloat"s] = 1.5f;
			Assert::IsFalse(foo1.Equals(&foo2));
			Assert::IsFalse(foo1.Equals(nullptr));
			Scope s;
			Assert::IsFalse(foo1.Equals(&s));

			AttributedBar bar1;
			AttributedBar bar2;
			Assert::IsTrue(bar1.Equals(&bar2));
			bar1.Integer = 10;
			Assert::IsFalse(bar1.Equals(&bar2));
			Assert::IsFalse(bar1.Equals(nullptr));

			Assert::AreEqual(bar1.ToString(), "Attributed"s);

			TypeManager::UnregisterType(AttributedBar::TypeIdClass());
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState AttributedTest::sStartMemState;
}
