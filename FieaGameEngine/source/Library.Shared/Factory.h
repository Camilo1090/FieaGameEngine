#pragma once

#include "HashMap.h"
#include <string>
#include <gsl/gsl>
#include <exception>

namespace FieaGameEngine
{
	/// <summary>
	/// Base abstract class for creating factories of defined types
	/// </summary>
	template <class T>
	class Factory
	{
	public:
		/// <summary>
		/// destructor
		/// </summary>
		virtual ~Factory() = default;

		/// <summary>
		/// Looks for the registered factory of the given type
		/// </summary>
		/// <param name="name">class name</param>
		/// <returns>the address of the factory if found, nullptr otherwise</returns>
		static const Factory<T>* Find(const std::string& name);

		/// <summary>
		/// creates a new object of the given type using the corresponding factory if found
		/// </summary>
		/// <param name="name">class name</param>
		/// <returns>new object of the given type</returns>
		static gsl::owner<T*> Create(const std::string& name);

		static std::size_t Size();
		static bool IsEmpty();

		/// <summary>
		/// creates a new object of this factory's type
		/// </summary>
		/// <returns>new object</returns>
		virtual gsl::owner<T*> Create() const = 0;

		/// <summary>
		/// gets the name of this factory's type
		/// </summary>
		/// <returns>factory type name</returns>
		virtual const std::string& ClassName() const = 0;

	protected:
		/// <summary>
		/// registers the given factory within this group of factories
		/// </summary>
		/// <exception cref="std::exception">thrown if the factory is already present</exception>
		/// <param name="factory">factory</param>
		static void Add(const Factory& factory);

		/// <summary>
		/// removes the given factory from this group of factories
		/// </summary>
		/// <param name="factory">factory</param>
		static void Remove(const Factory& factory);

	private:
		using FactoryMap = HashMap<std::string, const Factory*>;

		/// <summary>
		/// list of factories of this group
		/// </summary>
		static FactoryMap sFactories;
	};

	/// <summary>
	/// Macro for creating a concrete factory of the given type
	/// </summary>
#define CONCRETE_FACTORY(ConcreteType, AbstractType)												\
	class ConcreteType##Factory final : public FieaGameEngine::Factory<AbstractType>				\
	{																								\
	public:																							\
		ConcreteType##Factory()	: mPrototype(#ConcreteType)	{ Add(*this); }							\
		virtual ~ConcreteType##Factory() { Remove(*this); }											\
		ConcreteType##Factory(const ConcreteType##Factory&) = delete;								\
		ConcreteType##Factory(ConcreteType##Factory&&) = delete;									\
		ConcreteType##Factory& operator=(const ConcreteType##Factory&) = delete;					\
		ConcreteType##Factory& operator=(ConcreteType##Factory&&) = delete;							\
																									\
		virtual gsl::owner<AbstractType*> Create() const override { return new ConcreteType(); }	\
		virtual const std::string& ClassName() const override { return mPrototype; }				\
																									\
	private:																						\
		const std::string mPrototype;																\
	};
}

#include "Factory.inl"