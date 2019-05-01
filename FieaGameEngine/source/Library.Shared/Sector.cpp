#include "pch.h"
#include "Sector.h"
#include "Entity.h"
#include "World.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Sector)

	const std::string Sector::EntitiesKey = "Entities";

	const std::size_t Sector::EntitiesIndex = 2;

	Vector<Signature> Sector::Signatures()
	{
		return Vector<Signature>(
			{
				{ "Name", Datum::DatumType::STRING, 1, offsetof(Sector, mName) },
				{ EntitiesKey, Datum::DatumType::TABLE, 0, 0 }
			});
	}

	Sector::Sector() : Sector(std::string())
	{
	}

	Sector::Sector(const std::string& name) : Attributed(TypeIdClass()), mName(name)
	{
		assert(mList[EntitiesIndex]->first == EntitiesKey);
	}

	gsl::owner<Sector*> Sector::Clone() const
	{
		return new Sector(*this);
	}

	void Sector::Update(WorldState& worldState)
	{
		Datum& entities = Entities();
		for (size_t i = 0; i < entities.Size(); ++i)
		{
			Scope& scope = entities[i];
			assert(scope.Is(Entity::TypeIdClass()));
			Entity* entity = static_cast<Entity*>(&scope);
			worldState.Entity = entity;
			entity->Update(worldState);
			worldState.Entity = nullptr;
		}
	}

	Entity* Sector::CreateEntity(const std::string& className, const std::string& instanceName)
	{
		Scope* scope = Factory<Scope>::Create(className);
		if (scope == nullptr)
		{
			throw std::exception("Factory not registered.");
		}

		assert(scope->Is(Entity::TypeIdClass()));
		Entity* entity = static_cast<Entity*>(scope);
		entity->SetName(instanceName);
		entity->SetSector(*this);

		return entity;
	}

	Datum& Sector::Entities()
	{
		return operator[](EntitiesIndex);
	}

	const Datum& Sector::Entities() const
	{
		return operator[](EntitiesIndex);
	}

	void Sector::SetWorld(World& world)
	{
		world.Adopt(World::SectorsKey, *this);
	}

	std::string& Sector::Name()
	{
		return mName;
	}

	const std::string& Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const std::string& name)
	{
		mName = name;
	}
}
