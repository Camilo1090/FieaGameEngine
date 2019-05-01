#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionIncrement.h"
#include "Avatar.h"
#include "Reaction.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "ActionCreateAction.h"
#include "ActionDeleteAction.h"
#include "EventMessageAttributed.h"
#include "TypeManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::literals::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_MODULE_INITIALIZE(ModuleInitialize)
	{
		REGISTER_TYPE(AttributedFoo);
		REGISTER_TYPE(World);
		REGISTER_TYPE(Sector);
		REGISTER_TYPE(Entity);
		REGISTER_TYPE(Avatar);
		REGISTER_TYPE(Action);
		REGISTER_TYPE(ActionList);
		REGISTER_TYPE(ActionListIf);
		REGISTER_TYPE(ActionIncrement);
		REGISTER_TYPE(Reaction);
		REGISTER_TYPE(ReactionAttributed);
		REGISTER_TYPE(ActionEvent);
		REGISTER_TYPE(EventMessageAttributed);
		REGISTER_TYPE(ActionCreateAction);
		REGISTER_TYPE(ActionDeleteAction);
	}

	TEST_MODULE_CLEANUP(ModuleCleanup)
	{
		TypeManager::UnregisterType(AttributedFoo::TypeIdClass());
		TypeManager::Clear();
	}
}