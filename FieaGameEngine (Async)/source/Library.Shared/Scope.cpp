#include "pch.h"
#include <cstddef>
#include <string>
#include <exception>
#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(size_t capacity) :
		mMap(capacity), mList(capacity)
	{		
	}

	Scope::Scope(const Scope& other) :
		mMap(other.mMap.BucketCount()), mList(other.mList.Capacity())
	{
		DeepCopy(other);
	}

	Scope::Scope(Scope&& other) : 
		mMap(std::move(other.mMap)), mList(std::move(other.mList)), mParent(other.mParent)
	{
		Reparent(std::move(other));
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (this != &other)
		{
			DeleteChildren();
			ClearContainers();

			DeepCopy(other);
		}

		return *this;
	}

	Scope& Scope::operator=(Scope&& other)
	{
		if (this != &other)
		{
			DeleteChildren();

			mMap = std::move(other.mMap);
			mList = std::move(other.mList);
			mParent = other.mParent;

			Reparent(std::move(other));
		}

		return *this;
	}

	Scope::~Scope()
	{
		_Clear();
	}

	bool Scope::operator==(const Scope& other) const
	{
		if (mList.Size() != other.mList.Size())
		{
			return false;
		}

		for (size_t i = 0; i < mList.Size(); ++i)
		{
			if (*mList[i] != *other.mList[i])
			{
				return false;
			}
		}

		return true;
	}

	bool Scope::operator!=(const Scope& other) const
	{
		return !operator==(other);
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	Datum& Scope::operator[](std::string&& name)
	{
		return Append(std::move(name));
	}

	Datum& Scope::operator[](size_t index)
	{
		return mList[index]->second;
	}

	const Datum& Scope::operator[](size_t index) const
	{
		return mList[index]->second;
	}

	Datum& Scope::At(const std::string& name)
	{
		return mMap.At(name);
	}

	const Datum& Scope::At(const std::string& name) const
	{
		return mMap.At(name);
	}

	Datum& Scope::At(size_t index)
	{
		return mList[index]->second;
	}

	const Datum& Scope::At(size_t index) const
	{
		return mList[index]->second;
	}

	Datum& Scope::Append(const std::string& name)
	{
		EmptyNameGuard(name);

		auto[it, inserted] = mMap.Emplace(name, Datum());

		if (inserted)
		{
			mList.PushBack(&(*it));
		}

		return (*it).second;
	}

	Datum& Scope::Append(std::string&& name)
	{
		EmptyNameGuard(name);

		auto[it, inserted] = mMap.Emplace(std::move(name), Datum());

		if (inserted)
		{
			mList.PushBack(&(*it));
		}

		return (*it).second;
	}

	Datum& Scope::Append(const std::string& name, bool& inserted)
	{
		EmptyNameGuard(name);

		auto[it, mapInserted] = mMap.Emplace(name, Datum());

		if (mapInserted)
		{
			mList.PushBack(&(*it));
		}

		inserted = mapInserted;
		return (*it).second;
	}

	Datum& Scope::Append(const std::string& name, const Datum& datum)
	{
		EmptyNameGuard(name);

		auto[it, inserted] = mMap.Emplace(name, datum);

		if (inserted)
		{
			mList.PushBack(&(*it));
		}

		return (*it).second;
	}

	Datum& Scope::Append(const EntryType& pair)
	{
		EmptyNameGuard(pair.first);

		auto[it, inserted] = mMap.Emplace(pair);

		if (inserted)
		{
			mList.PushBack(&(*it));
		}

		return (*it).second;
	}

	Datum& Scope::Append(EntryType&& pair)
	{
		EmptyNameGuard(pair.first);

		auto[it, inserted] = mMap.Emplace(std::move(pair));

		if (inserted)
		{
			mList.PushBack(&(*it));
		}

		return (*it).second;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		Datum& datum = Append(name);
		
		if (datum.Type() != Datum::DatumType::UNKNOWN && datum.Type() != Datum::DatumType::TABLE)
		{
			throw std::exception("Cannot append Scope. Datum type is not Table.");
		}

		Scope* scope = new Scope();
		scope->mParent = this;
		datum.PushBack(*scope);

		return *scope;
	}

	Scope& Scope::AppendScope(std::string&& name)
	{
		Datum& datum = Append(std::move(name));

		if (datum.Type() != Datum::DatumType::UNKNOWN && datum.Type() != Datum::DatumType::TABLE)
		{
			throw std::exception("Cannot append Scope. Datum type is not Table.");
		}

		Scope* scope = new Scope();
		scope->mParent = this;
		datum.PushBack(*scope);

		return *scope;
	}

	Datum* Scope::Find(const std::string& name)
	{
		MapIterator it;
		if (mMap.ContainsKey(name, it))
		{
			return &((*it).second);
		}

		return nullptr;
	}

	const Datum* Scope::Find(const std::string& name) const
	{
		return const_cast<Scope*>(this)->Find(name);
	}

	std::pair<Datum*, size_t> Scope::FindChildScope(const Scope* scope)
	{
		for (auto entry : mList)
		{
			Datum& datum = entry->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					if (&datum[i] == scope)
					{
						return std::pair(&datum, i);
					}
				}
			}
		}

		return std::pair(nullptr, 0);
	}

	std::pair<const Datum*, size_t> Scope::FindChildScope(const Scope* scope) const
	{
		return const_cast<Scope*>(this)->FindChildScope(scope);
	}

	Datum* Scope::Search(const std::string& name, Scope** scope)
	{
		// look for an entry with the given name
		MapIterator it;
		if (mMap.ContainsKey(name, it))
		{
			if (scope != nullptr)
			{
				*scope = this;
			}

			return &((*it).second);
		}
		else if (mParent != nullptr)
		{
			// recursively call parent search if not null
			return mParent->Search(name, scope);
		}

		if (scope != nullptr)
		{
			*scope = nullptr;
		}

		return nullptr;
	}

	const Datum* Scope::Search(const std::string& name, Scope** scope) const
	{
		return const_cast<Scope*>(this)->Search(name, scope);
	}

	std::string Scope::FindName(const Scope* scope) const
	{
		for (const auto entry : mList)
		{
			const Datum& datum = entry->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					if (&datum[i] == scope)
					{
						return entry->first;
					}
				}
			}
		}

		return "";
	}

	Datum& Scope::Adopt(const std::string& name, Scope& scope)
	{
		if (this == &scope)
		{
			throw std::exception("Cannot adopt self.");
		}

		if (IsAncestor(scope))
		{
			throw std::exception("Cannot adopt an ancestor.");
		}

		Datum& datum = Append(name);

		if (datum.Type() != Datum::DatumType::UNKNOWN && datum.Type() != Datum::DatumType::TABLE)
		{
			throw std::exception("Datum of given Name is not of type Table.");
		}

		scope.ChangeParent(*this);
		datum.PushBack(scope);

		return datum;
	}

	void Scope::Clear()
	{
		_Clear();
	}

	/*Scope* Scope::GetParent()
	{
		return mParent;
	}*/

	/*const Scope* Scope::GetParent() const
	{
		return mParent;
	}*/

	size_t Scope::Size() const
	{
		return mMap.Size();
	}

	bool Scope::Equals(const RTTI* other) const
	{
		if (other == nullptr)
		{
			return false;
		}

		Scope* scope = other->As<Scope>();

		return scope == nullptr ? false : operator==(*scope);
	}

	std::string Scope::ToString() const
	{
		return TypeName();
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	void Scope::DeleteChildren()
	{
		for (const auto& entry : mList)
		{
			Datum& datum = entry->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					// all nested (one level) scopes must have this as their parent
					assert(datum[i].mParent == this);

					datum[i].mParent = nullptr;
					delete &(datum[i]);
				}
			}
		}
	}

	void Scope::ClearContainers()
	{
		mList.Clear();
		mMap.Clear();
	}

	void Scope::Emancipate()
	{
		if (mParent != nullptr)
		{
			mParent->RemoveChildScope(*this);
			mParent = nullptr;
		}
	}

	void Scope::_Clear()
	{
		Emancipate();
		DeleteChildren();
		ClearContainers();
	}

	void Scope::RemoveChildScope(Scope& scope)
	{
		auto[datum, index] = FindChildScope(&scope);

		assert(datum != nullptr);

		datum->RemoveAt(index);
	}

	bool Scope::IsAncestor(const Scope& scope)
	{
		if (mParent == &scope)
		{
			return true;
		}

		return mParent == nullptr ? false : mParent->IsAncestor(scope);
	}

	void Scope::ChangeParent(Scope& scope)
	{
		Emancipate();
		mParent = &scope;
	}

	void Scope::Reparent(Scope&& other)
	{
		// inform the parent of the new address of its child
		// and correctly delete the current child scope
		if (other.mParent != nullptr)
		{
			auto[datum, index] = other.mParent->FindChildScope(&other);
			assert(datum != nullptr);
			datum->Get<Scope*>(index) = this;
			other.mParent = nullptr;
			delete &other;
		}

		// update the parent address of all children
		for (auto entry : mList)
		{
			Datum& datum = entry->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					datum[i].mParent = this;
				}
			}
		}
	}

	void Scope::DeepCopy(const Scope& other)
	{
		for (auto entry : other.mList)
		{
			// all insertions must return true for insertion,
			// since we are deep copying this scope should be empty
			auto[mapIterator, inserted] = mMap.Emplace(*entry);
			assert(inserted);
			mList.PushBack(&(*mapIterator));

			// for each child table we must create a new table deep copying the other table
			// and assigning its parent to this
			Datum& datum = (*mapIterator).second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Scope*& scope = datum.Get<Scope*>(i);
					//scope = new Scope(entry->second[i]);
					scope = scope->Clone();
					scope->mParent = this;
				}
			}
		}
	}

	inline void Scope::EmptyNameGuard(const std::string& name)
	{
		if (name.empty())
		{
			throw std::exception("Cannot use an empty name.");
		}
	}
}
