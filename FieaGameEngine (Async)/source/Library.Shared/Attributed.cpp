#include "pch.h"
#include <cstddef>
#include <string>
#include "Datum.h"
#include "TypeManager.h"
#include "Attributed.h"

using namespace std::literals::string_literals;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed(RTTI::IdType type)
	{
		Populate(type);
	}

	Attributed::Attributed(const Attributed& other) :
		Scope::Scope(other)
	{
		UpdatePrescribedAttributes(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other) :
		Scope::Scope(std::move(other))
	{
		UpdatePrescribedAttributes(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		if (this != &other)
		{
			Scope::operator=(other);
			UpdatePrescribedAttributes(TypeIdInstance());
		}

		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other)
	{
		if (this != &other)
		{
			Scope::operator=(std::move(other));
			UpdatePrescribedAttributes(TypeIdInstance());
		}

		return *this;
	}

	bool Attributed::operator==(const Attributed& other) const
	{
		if (Size() != other.Size())
		{
			return false;
		}

		// all attributed derived objects must have at least the 'this' attribute
		assert((*this)[0].Get<RTTI*>() == this && other[0].Get<RTTI*>() == &other);

		for (size_t i = 1; i < Size(); ++i)
		{
			auto thisPair = mList[i];
			auto thatPair = other.mList[i];
			if (*thisPair != *thatPair)
			{
				return false;
			}
		}

		return true;
	}

	bool Attributed::operator!=(const Attributed& other) const
	{
		return !operator==(other);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		PrescribedNameGuard(name);

		return Append(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name, const Datum& datum)
	{
		PrescribedNameGuard(name);

		return Append(name, datum);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const Scope::EntryType& pair)
	{
		PrescribedNameGuard(pair.first);

		return Append(pair);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(Scope::EntryType&& pair)
	{
		PrescribedNameGuard(pair.first);

		return Append(std::move(pair));
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return mMap.ContainsKey(name);
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		if (name == "this"s)
		{
			return true;
		}

		for (const auto& signature : GetSignatures())
		{
			if (signature.Name == name)
			{
				assert(IsAttribute(name));
				return true;
			}
		}

		return false;
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	Vector<Scope::EntryType*> Attributed::Attributes()
	{
		return mList;
	}

	Vector<const Scope::EntryType*> Attributed::Attributes() const
	{
		Vector<const EntryType*> result(Size());
		for (auto entry : mList)
		{
			result.PushBack(entry);
		}

		return result;
	}

	Vector<Scope::EntryType*> Attributed::PrescribedAttributes()
	{
		size_t count = GetPrescribedAttributeCount();
		Vector<EntryType*> result(count);

		for (size_t i = 0; i < count; ++i)
		{
			result.PushBack(mList[i]);
		}

		return result;
	}

	Vector<const Scope::EntryType*> Attributed::PrescribedAttributes() const
	{
		size_t count = GetPrescribedAttributeCount();
		Vector<const EntryType*> result(count);

		for (size_t i = 0; i < count; ++i)
		{
			result.PushBack(mList[i]);
		}

		return result;
	}

	Vector<Scope::EntryType*> Attributed::AuxiliaryAttributes()
	{
		size_t count = mList.Size() - GetPrescribedAttributeCount();
		Vector<EntryType*> result(count);

		for (size_t i = GetPrescribedAttributeCount(); i < mList.Size(); ++i)
		{
			result.PushBack(mList[i]);
		}

		return result;
	}

	Vector<const Scope::EntryType*> Attributed::AuxiliaryAttributes() const
	{
		size_t count = mList.Size() - GetPrescribedAttributeCount();
		Vector<const EntryType*> result(count);

		for (size_t i = GetPrescribedAttributeCount(); i < mList.Size(); ++i)
		{
			result.PushBack(mList[i]);
		}

		return result;
	}

	void Attributed::Clear()
	{
		_Clear();
		Populate(TypeIdInstance());
	}

	void Attributed::ClearAuxiliaryAttributes()
	{
		auto auxBegin = std::next(mList.begin(), GetPrescribedAttributeCount());
		for (auto it = auxBegin; it != mList.end(); ++it)
		{
			mMap.Remove((*it)->first);
		}

		mList.Remove(auxBegin, mList.end());
	}

	bool Attributed::Equals(const RTTI* other) const
	{
		if (other == nullptr)
		{
			return false;
		}

		Attributed* attributed = other->As<Attributed>();
		return attributed == nullptr ? false : operator==(*attributed);
	}

	std::string Attributed::ToString() const
	{
		return TypeName();
	}

	Vector<Signature> Attributed::GetSignatures() const
	{
		size_t total = GetPrescribedAttributeCount() - 1;
		Vector<Signature> signatures(total);

		RTTI::IdType type = TypeIdInstance();
		while (type != Attributed::TypeIdClass())
		{
			for (auto& signature : TypeManager::GetSignatures(type))
			{
				signatures.PushBack(signature);
			}

			auto[found, id] = TypeManager::FindBaseId(type);
			assert(found);
			type = id;
		}

		return signatures;
	}

	size_t Attributed::GetPrescribedAttributeCount() const
	{
		RTTI::IdType type = TypeIdInstance();
		size_t total = 1;
		while (type != Attributed::TypeIdClass())
		{
			total += TypeManager::GetSignatureCount(type);

			auto[found, id] = TypeManager::FindBaseId(type);
			assert(found);
			type = id;
		}

		return total;
	}

	void Attributed::Populate(RTTI::IdType type)
	{
		(*this)["this"s] = this;

		auto stack = TypeManager::GetInheritanceChain(type);
		while (!stack.IsEmpty())
		{
			const auto& signatures = TypeManager::GetSignatures(stack.Top());
			for (const auto& signature : signatures)
			{
				if (signature.Type == Datum::DatumType::UNKNOWN)
				{
					throw std::exception("Unsupported type");
				}

				// TODO: what to do if the header tool supplies a signature with an already present name AND the same type

				Datum& datum = Append(signature.Name);
				if (signature.Type != Datum::DatumType::TABLE)
				{
					datum.SetStorage(reinterpret_cast<std::uint8_t*>(this) + signature.Offset, signature.Size, signature.Type);
				}
				else
				{
					datum.SetType(Datum::DatumType::TABLE);
					for (size_t i = 0; i < signature.Size; ++i)
					{
						AppendScope(signature.Name);
					}
				}
			}

			stack.Pop();
		}
	}

	void Attributed::UpdatePrescribedAttributes(RTTI::IdType type)
	{
		assert(Size() > 0 && (*this)[0].Type() == Datum::DatumType::POINTER);
		(*this)[0] = this;

		size_t i = 1;
		auto stack = TypeManager::GetInheritanceChain(type);
		while (!stack.IsEmpty())
		{
			const auto& signatures = TypeManager::GetSignatures(stack.Top());
			size_t count = signatures.Size();

			// update external storage pointers with the correct address
			for (size_t j = 0; j < count; ++j)
			{
				const auto& signature = signatures[j];
				assert(signature.Type != Datum::DatumType::UNKNOWN);

				assert(mList[i]->first == signature.Name);
				assert(mList[i]->second.Type() == signature.Type);
				if (signature.Type != Datum::DatumType::TABLE)
				{
					(*this)[i].SetStorage(reinterpret_cast<std::int8_t*>(this) + signature.Offset, signature.Size, signature.Type);
				}

				++i;
			}

			stack.Pop();
		}
	}

	void Attributed::PrescribedNameGuard(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::exception("Name already used by a member attribute.");
		}
	}
}
