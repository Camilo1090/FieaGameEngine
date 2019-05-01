#pragma once

#include "HashMap.h"
#include "vector.h"
#include "Datum.h"
#include "RTTI.h"
#include <gsl/gsl>

namespace FieaGameEngine
{
	/// <summary>
	/// Represents a scope that holds associations of data and names
	/// </summary>
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	protected:
		using MapType = HashMap<std::string, Datum>;
		using EntryType = MapType::PairType;
		using ListType = Vector<EntryType*>;
		using MapIterator = MapType::Iterator;
		using ListIterator = ListType::Iterator;

	public:
		/// <summary>
		/// Constructs an empty scope
		/// </summary>
		/// <param name="capacity">Initial capacity of entries for this scope</param>
		explicit Scope(size_t capacity = 12);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other">scope</param>
		Scope(const Scope& other);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">scope</param>
		Scope(Scope&& other);

		/// <summary>
		/// Copy assignment
		/// </summary>
		/// <param name="other">scope</param>
		/// <returns>reference to self</returns>
		Scope& operator=(const Scope& other);

		/// <summary>
		/// Move assignment
		/// </summary>
		/// <param name="other">scope</param>
		/// <returns>reference to self</returns>
		Scope& operator=(Scope&& other);

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Compares two scopes for matching content
		/// </summary>
		/// <param name="other">scope</param>
		/// <returns>true if contents are equal, false otherwise</returns>
		bool operator==(const Scope& other) const;

		/// <summary>
		/// Compares two scopes for not matching content
		/// </summary>
		/// <param name="other">scope</param>
		/// <returns>true if contents are not equal, false otherwise</returns>
		bool operator!=(const Scope& other) const;

		/// <summary>
		/// Returns the datum mapped to the given name.
		/// Creates entry if not present already
		/// </summary>
		/// <param name="name">name of entry</param>
		/// <returns>reference to datum</returns>
		Datum& operator[](const std::string& name);

		/// <summary>
		/// Returns the datum mapped to the given name.
		/// Creates entry if not present already
		/// </summary>
		/// <param name="name">name of entry</param>
		/// <returns>reference to datum</returns>
		Datum& operator[](std::string&& name);

		/// <summary>
		/// Gets the datum at the given index considering order of insertion
		/// </summary>
		/// <param name="index">index</param>
		/// <returns>reference to datum</returns>
		Datum& operator[](size_t index);

		/// <summary>
		/// Gets the datum at the given index considering order of insertion
		/// </summary>
		/// <param name="index">index</param>
		/// <returns>reference to datum</returns>
		const Datum& operator[](size_t index) const;

		Datum& At(const std::string& name);
		const Datum& At(const std::string& name) const;

		/// <summary>
		/// Gets the datum at the given index considering order of insertion
		/// </summary>
		/// <param name="index">index</param>
		/// <returns>reference to datum</returns>
		Datum& At(size_t index);

		/// <summary>
		/// Gets the datum at the given index considering order of insertion
		/// </summary>
		/// <param name="index">index</param>
		/// <returns>reference to datum</returns>
		const Datum& At(size_t index) const;

		/// <summary>
		/// Adds a new entry to the scope with the given name
		/// and a default constructed datum, if not already present
		/// </summary>
		/// <param name="name">name</param>
		/// <returns>reference to datum</returns>
		Datum& Append(const std::string& name);

		/// <summary>
		/// Adds a new entry to the scope with the given name
		/// and a default constructed datum, if not already present
		/// </summary>
		/// <param name="name">name</param>
		/// <returns>reference to datum</returns>
		Datum& Append(std::string&& name);

		Datum& Append(const std::string& name, bool& inserted);

		Datum& Append(const std::string& name, const Datum& datum);
		Datum& Append(const EntryType& pair);
		Datum& Append(EntryType&& pair);

		/// <summary>
		/// Adds a new entry to this scope with the given name and
		/// a datum of type table, a new scope is pushed to the just
		/// created datum. If already present, a scope is pushed to
		/// that datum if possible
		/// </summary>
		/// <exception cref="std::exception">thrown if the existing datum is not of type table</exception>
		/// <param name="name">name</param>
		/// <returns>reference to scope</returns>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		/// Adds a new entry to this scope with the given name and
		/// a datum of type table, a new scope is pushed to the just
		/// created datum. If already present, a scope is pushed to
		/// that datum if possible
		/// </summary>
		/// <exception cref="std::exception">thrown if the existing datum is not of type table</exception>
		/// <param name="name">name</param>
		/// <returns>reference to scope</returns>
		Scope& AppendScope(std::string&& name);

		/// <summary>
		/// looks for an entry with the given name
		/// </summary>
		/// <param name="name">name</param>
		/// <returns>if found, address of the datum, nullptr otherwise</returns>
		Datum* Find(const std::string& name);

		/// <summary>
		/// looks for an entry with the given name
		/// </summary>
		/// <param name="name">name</param>
		/// <returns>if found, address of the datum, nullptr otherwise</returns>
		const Datum* Find(const std::string& name) const;

		/// <summary>
		/// looks for a scope within this scope
		/// </summary>
		/// <param name="scope">scope</param>
		/// <returns>pair of address to datum containing the scope and its position in it if found, nullptr and 0 otherwise</returns>
		std::pair<Datum*, size_t> FindChildScope(const Scope* scope);

		/// <summary>
		/// looks for a scope within this scope
		/// </summary>
		/// <param name="scope">scope</param>
		/// <returns>pair of address to datum containing the scope and its position in it if found, nullptr and 0 otherwise</returns>
		std::pair<const Datum*, size_t> FindChildScope(const Scope* scope) const;

		/// <summary>
		/// looks for the closest entry with the given name in this scope and recursively up the hierarchy to the root
		/// </summary>
		/// <param name="name">name</param>
		/// <param name="scope">out parameter to store the address of the scope containing the name if found</param>
		/// <returns>address of the datum mapped to the given name if found, nullptr otherwise</returns>
		Datum* Search(const std::string& name, Scope** scope = nullptr);

		/// <summary>
		/// looks for the closest entry with the given name in this scope and recursively up the hierarchy to the root
		/// </summary>
		/// <param name="name">name</param>
		/// <param name="scope">out parameter to store the address of the scope containing the name if found</param>
		/// <returns>address of the datum mapped to the given name if found, nullptr otherwise</returns>
		const Datum* Search(const std::string& name, Scope** scope = nullptr) const;

		/// <summary>
		/// looks for the name of the entry that contains the given scope
		/// </summary>
		/// <param name="scope">scope</param>
		/// <returns>name of the entry if found, empty string otherwise</returns>
		std::string FindName(const Scope* scope) const;

		/// <summary>
		/// Adopts a scope under this scope.
		/// Given scope is added to the entry with the given name.
		/// Given scope is removed from its parent if not null
		/// </summary>
		/// <param name="name">name</param>
		/// <param name="scope">scope</param>
		/// <returns>datum reference containing the given scope</returns>
		Datum& Adopt(const std::string& name, Scope& scope);

		virtual void Clear();

		/// <summary>
		/// whether or not the given scope is an ancestor of this scope
		/// </summary>
		/// <param name="scope">scope</param>
		/// <returns>true if ancestor, false otherwise</returns>
		bool IsAncestor(const Scope& scope);

		/// <summary>
		/// gets the parent of this scope
		/// </summary>
		/// <returns>parent address</returns>
		template <typename T = Scope>
		T* GetParent();

		/// <summary>
		/// gets the parent of this scope
		/// </summary>
		/// <returns>parent address</returns>
		template <typename T = Scope>
		const T* GetParent() const;

		/// <summary>
		/// gets the size of this scope, number of entries
		/// </summary>
		/// <returns>number of entries</returns>
		size_t Size() const;

		/// <summary>
		/// compares two scopes for equality
		/// </summary>
		/// <param name="other">scope</param>
		/// <returns>true if equal, false otherwise</returns>
		virtual bool Equals(const RTTI* other) const override;

		/// <summary>
		/// return a string representation of this scope
		/// </summary>
		/// <returns>string</returns>
		virtual std::string ToString() const override;

	protected:
		/// <summary>
		/// dictionary holding the entries
		/// </summary>
		MapType mMap;

		/// <summary>
		/// vector holding the insertion order of the entries
		/// </summary>
		ListType mList;

		/// <summary>
		/// address of parent
		/// </summary>
		Scope* mParent = nullptr;

		virtual gsl::owner<Scope*> Clone() const;
		void _Clear();
	
	private:
		/// <summary>
		/// deletes all children scopes of this scope
		/// </summary>
		void DeleteChildren();

		/// <summary>
		/// clears the contents of both the list and dictionary
		/// </summary>
		void ClearContainers();

		/// <summary>
		/// remove self from parent
		/// </summary>
		void Emancipate();

		/// <summary>
		/// remove the given scope
		/// </summary>
		/// <param name="scope">scope</param>
		void RemoveChildScope(Scope& scope);

		/// <summary>
		/// sets a new parent
		/// </summary>
		/// <param name="scope">scope</param>
		void ChangeParent(Scope& scope);

		void Reparent(Scope&& scope);

		/// <summary>
		/// deep copies the entries from the other scope to this
		/// </summary>
		/// <param name="other">scope</param>
		void DeepCopy(const Scope& other);

		/// <summary>
		/// guards against empty string names
		/// </summary>
		/// <param name="name">name</param>
		void EmptyNameGuard(const std::string& name);
	};
}

#include "Scope.inl"
