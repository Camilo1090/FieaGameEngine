#include "pch.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	HashMap<RTTI::IdType, TypeManager::TypeRegistryEntry> TypeManager::sTypeRegistry(16);

	void TypeManager::RegisterType(RTTI::IdType id, const Vector<Signature>& signatures, RTTI::IdType parentId)
	{
		sTypeRegistry.Emplace(id, TypeRegistryEntry(signatures, parentId));
	}

	void TypeManager::RegisterType(RTTI::IdType id, Vector<Signature>&& signatures, RTTI::IdType parentId)
	{
		sTypeRegistry.Emplace(id, TypeRegistryEntry(std::move(signatures), parentId));
	}

	void TypeManager::UnregisterType(RTTI::IdType id)
	{
		sTypeRegistry.Remove(id);
	}

	const Vector<Signature>& TypeManager::GetSignatures(RTTI::IdType id)
	{
		return sTypeRegistry.At(id).Signatures;
	}

	size_t TypeManager::GetSignatureCount(RTTI::IdType id)
	{
		return sTypeRegistry.At(id).Signatures.Size();
	}

	std::pair<bool, RTTI::IdType> TypeManager::FindBaseId(RTTI::IdType id)
	{
		TypeRegistryType::Iterator it;
		if (sTypeRegistry.ContainsKey(id, it))
		{
			return { true, (*it).second.BaseId };
		}

		return { false, 0 };
	}

	Stack<RTTI::IdType> TypeManager::GetInheritanceChain(RTTI::IdType id)
	{
		Stack<RTTI::IdType> stack;

		TypeRegistryType::Iterator it;
		while (sTypeRegistry.ContainsKey(id, it))
		{
			stack.Push(id);
			id = (*it).second.BaseId;
		}

		return stack;
	}

	void TypeManager::Clear()
	{
		sTypeRegistry.Clear();
	}

	TypeManager::TypeRegistryEntry::TypeRegistryEntry(const Vector<Signature>& signatures, RTTI::IdType baseId) : 
		Signatures(signatures), BaseId(baseId)
	{
	}

	TypeManager::TypeRegistryEntry::TypeRegistryEntry(Vector<Signature>&& signatures, RTTI::IdType baseId) :
		Signatures(std::move(signatures)), BaseId(std::move(baseId))
	{
	}
}
