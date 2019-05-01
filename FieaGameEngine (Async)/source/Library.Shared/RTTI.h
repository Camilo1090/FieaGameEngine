#pragma once

#include <string>
#include <cstddef>

namespace FieaGameEngine
{
	class RTTI
	{
	public:
		using IdType = std::size_t;

	public:
		static IdType TypeIdClass() { return 0; };

	public:
		virtual ~RTTI() = default;

		virtual IdType TypeIdInstance() const = 0;

		virtual RTTI* QueryInterface(const IdType)
		{
			return nullptr;
		}

		virtual bool Is(IdType) const
		{
			return false;
		}

		virtual bool Is(const std::string&) const
		{
			return false;
		}

		template <typename T>
		T* As() const
		{
			return (Is(T::TypeIdClass()) ? reinterpret_cast<T*>(const_cast<RTTI*>(this)) : nullptr);
		}

		virtual std::string ToString() const
		{
			return "RTTI";
		}

		virtual bool Equals(const RTTI* rhs) const
		{
			return this == rhs;
		}
	};

#define RTTI_DECLARATIONS(Type, ParentType)																												\
		public:																																			\
			static std::string TypeName() { return std::string(#Type); }																				\
			static IdType TypeIdClass() { return sRunTimeTypeId; }																						\
			static IdType ParentTypeIdClass() { return ParentType::TypeIdClass(); }																		\
			virtual IdType TypeIdInstance() const override { return TypeIdClass(); }																	\
			virtual FieaGameEngine::RTTI* QueryInterface(const IdType id) override																		\
            {																																			\
				return (id == sRunTimeTypeId ? reinterpret_cast<FieaGameEngine::RTTI*>(this) : ParentType::QueryInterface(id));							\
            }																																			\
			virtual bool Is(IdType id) const override																									\
			{																																			\
				return (id == sRunTimeTypeId ? true : ParentType::Is(id));																				\
			}																																			\
			virtual bool Is(const std::string& name) const override																						\
			{																																			\
				return (name == TypeName() ? true : ParentType::Is(name));																				\
			}																																			\
		private:																																		\
			static IdType sRunTimeTypeId;

#define RTTI_DEFINITIONS(Type) FieaGameEngine::RTTI::IdType Type::sRunTimeTypeId = reinterpret_cast<FieaGameEngine::RTTI::IdType>(&Type::sRunTimeTypeId);
}
