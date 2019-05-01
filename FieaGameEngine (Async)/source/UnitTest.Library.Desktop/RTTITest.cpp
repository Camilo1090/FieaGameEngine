#include "pch.h"
#include "CppUnitTest.h"
#include "RTTI.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace UnitTestLibraryDesktop
{
	class Derived : public RTTI
	{
	public:
		static IdType TypeIdClass() { return sRunTimeTypeId; }
		virtual IdType TypeIdInstance() const override { return TypeIdClass(); }

	private:
		static IdType sRunTimeTypeId;
	};

	RTTI::IdType Derived::sRunTimeTypeId = reinterpret_cast<FieaGameEngine::RTTI::IdType>(&sRunTimeTypeId);

	TEST_CLASS(RTTITest)
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
			Derived d;
			Assert::IsNull(d.QueryInterface(d.TypeIdInstance()));
			Assert::AreEqual(d.ToString(), "RTTI"s);
			Assert::IsTrue(d.Equals(&d));
		}

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState RTTITest::sStartMemState;
}
