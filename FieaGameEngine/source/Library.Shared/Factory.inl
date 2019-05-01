#include "Factory.h"

namespace FieaGameEngine
{
	template <typename T>
	typename Factory<T>::FactoryMap Factory<T>::sFactories(16);

	template <typename T>
	const Factory<T>* Factory<T>::Find(const std::string& name)
	{
		typename FactoryMap::Iterator it;
		if (sFactories.ContainsKey(name, it))
		{
			auto factory = (*it).second;
			assert(factory != nullptr);
			return factory;
		}

		return nullptr;
	}

	template <typename T>
	gsl::owner<T*> Factory<T>::Create(const std::string& name)
	{
		typename FactoryMap::Iterator it;
		if (sFactories.ContainsKey(name, it))
		{
			auto factory = (*it).second;
			assert(factory != nullptr);
			return factory->Create();
		}

		return nullptr;
	}

	template <typename T>
	std::size_t Factory<T>::Size()
	{
		return sFactories.Size();
	}

	template <typename T>
	bool Factory<T>::IsEmpty()
	{
		return Size() == std::size_t(0);
	}

	template <typename T>
	inline void Factory<T>::Add(const Factory& factory)
	{
		if (sFactories.ContainsKey(factory.ClassName()))
		{
			throw std::exception("Factory already present.");
		}

		sFactories.Emplace(factory.ClassName(), &factory);
	}

	template <typename T>
	inline void Factory<T>::Remove(const Factory& factory)
	{
		sFactories.Remove(factory.ClassName());
	}
}