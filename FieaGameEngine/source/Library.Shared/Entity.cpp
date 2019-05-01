#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity)

	const std::string Entity::ActionsKey = "Actions";

	const std::size_t Entity::ActionsIndex = 2;

	Vector<Signature> Entity::Signatures()
	{
		return Vector<Signature>(
			{
				{ "Name", Datum::DatumType::STRING, 1, offsetof(Entity, mName) },
				{ ActionsKey, Datum::DatumType::TABLE, 0, 0 }
			});
	}

	Entity::Entity() : Entity(TypeIdClass())
	{
	}

	Entity::Entity(const std::string& name) : Entity(TypeIdClass(), name)
	{
	}

	gsl::owner<Entity*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	void Entity::Update(WorldState& worldState)
	{
		Datum& actions = Actions();
		size_t size = actions.Size();
		for (size_t i = 0; i < size; ++i)
		{
			Scope& scope = actions[i];
			assert(scope.Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(&scope);
			worldState.Action = action;
			action->Update(worldState);
			worldState.Action = nullptr;
		}
	}

	void Entity::SetSector(Sector& sector)
	{
		sector.Adopt(Sector::EntitiesKey, *this);
	}

	gsl::owner<Action*> Entity::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* scope = Factory<Scope>::Create(className);
		if (scope == nullptr)
		{
			throw std::exception("Factory not registered.");
		}

		assert(scope->Is(Action::TypeIdClass()));
		Action* action = static_cast<Action*>(scope);
		action->SetName(instanceName);
		Adopt(ActionsKey, *action);

		return action;
	}

	Action* Entity::FindAction(const std::string& actionName)
	{
		Action* action = nullptr;
		auto& actions = Actions();
		for (size_t i = 0; i < actions.Size(); ++i)
		{
			assert(actions[i].Is(Action::TypeIdClass()));
			action = static_cast<Action*>(&actions[i]);
			if (action->Name() == actionName)
			{
				break;
			}
		}

		return action;
	}

	const Action* Entity::FindAction(const std::string& actionName) const
	{
		return const_cast<Entity*>(this)->FindAction(actionName);
	}

	Datum& Entity::Actions()
	{
		return operator[](ActionsIndex);
	}

	const Datum& Entity::Actions() const
	{
		return operator[](ActionsIndex);
	}

	std::string& Entity::Name()
	{
		return mName;
	}

	const std::string& Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const std::string& name)
	{
		mName = name;
	}

	Entity::Entity(RTTI::IdType type) : Entity(type, std::string())
	{
	}

	Entity::Entity(RTTI::IdType type, const std::string& name) : Attributed(type), mName(name)
	{
		assert(mList[ActionsIndex]->first == ActionsKey);
	}
}