#include "pch.h"
#include "World.h"
#include "Sector.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(World)

	const std::string World::SectorsKey = "Sectors";
	const std::size_t World::SectorsIndex = 2;

	Vector<Signature> World::Signatures()
	{
		return Vector<Signature>(
			{
				{ "Name", Datum::DatumType::STRING, 1, offsetof(World, mName) },
				{ SectorsKey, Datum::DatumType::TABLE, 0, 0 }
			});
	}

	World::World(const std::shared_ptr<GameTime>& gameTime) : World(gameTime, std::string())
	{
	}

	World::World(const std::shared_ptr<GameTime>& gameTime, const std::string& name) : Attributed(TypeIdClass()), mName(name), mWorldState(gameTime)
	{
		assert(mList[SectorsIndex]->first == SectorsKey);
	}

	gsl::owner<World*> World::Clone() const
	{
		return new World(*this);
	}

	void World::Update(WorldState& worldState)
	{
		worldState.World = this;
		mEventQueue->Update(*worldState.GameTime);

		Datum& sectors = Sectors();
		for (size_t i = 0; i < sectors.Size(); ++i)
		{
			Scope& scope = sectors[i];
			assert(scope.Is(Scope::TypeIdClass()));
			Sector* sector = static_cast<Sector*>(&scope);
			worldState.Sector = sector;
			sector->Update(worldState);
			worldState.Sector = nullptr;
		}

		DeleteMarkedScopes();
		worldState.World = nullptr;
	}

	Sector* World::CreateSector(const std::string& instanceName)
	{
		Sector* sector = new Sector();
		sector->SetName(instanceName);
		sector->SetWorld(*this);

		return sector;
	}

	Datum& World::Sectors()
	{
		return operator[](SectorsIndex);
	}

	const Datum& World::Sectors() const
	{
		return operator[](SectorsIndex);
	}

	void World::MarkForDeletion(Scope& scope)
	{
		mDeleteList.PushBackUnique(&scope);
	}

	std::string& World::Name()
	{
		return mName;
	}

	const std::string& World::Name() const
	{
		return mName;
	}

	void World::SetName(const std::string& name)
	{
		mName = name;
	}

	WorldState& World::GetWorldState()
	{
		return mWorldState;
	}

	const WorldState& World::GetWorldState() const
	{
		return mWorldState;
	}

	EventQueue* World::GetEventQueue()
	{
		return mEventQueue;
	}

	void World::SetEventQueue(EventQueue& queue)
	{
		mEventQueue = &queue;
	}

	void World::DeleteMarkedScopes()
	{
		for (auto ptr : mDeleteList)
		{
			delete ptr;
		}

		mDeleteList.Clear();
	}
}
