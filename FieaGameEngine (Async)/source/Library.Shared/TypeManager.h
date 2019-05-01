#pragma once

#include "RTTI.h"
#include "HashMap.h"
#include "vector.h"
#include "Stack.h"
#include "Signature.h"

#define REGISTER_TYPE(Type) TypeManager::RegisterType(Type::TypeIdClass(), Type::Signatures(), Type::ParentTypeIdClass());

namespace FieaGameEngine
{
	class TypeManager
	{
	private:
		struct TypeRegistryEntry final
		{
			TypeRegistryEntry(const Vector<Signature>& signatures, RTTI::IdType baseId);
			TypeRegistryEntry(Vector<Signature>&& signatures, RTTI::IdType baseId);

			Vector<Signature> Signatures;
			RTTI::IdType BaseId;
		};

	public:
		TypeManager() = delete;
		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager& operator=(const TypeManager&) = delete;
		TypeManager& operator=(TypeManager&&) = delete;
		~TypeManager() = default;

		static void RegisterType(RTTI::IdType id, const Vector<Signature>& signatures, RTTI::IdType parentId);
		static void RegisterType(RTTI::IdType id, Vector<Signature>&& signatures, RTTI::IdType parentId);
		static void UnregisterType(RTTI::IdType id);
		static const Vector<Signature>& GetSignatures(RTTI::IdType id);
		static size_t GetSignatureCount(RTTI::IdType id);
		static std::pair<bool, RTTI::IdType> FindBaseId(RTTI::IdType id);
		static Stack<RTTI::IdType> GetInheritanceChain(RTTI::IdType id);
		static void Clear();

	private:
		using TypeRegistryType = HashMap<RTTI::IdType, TypeRegistryEntry>;
		static TypeRegistryType sTypeRegistry;
	};
}
