#include "pch.h"
#include <glm/gtx/string_cast.hpp>
#include <algorithm>
#include <functional>
#include <string>
#include "RTTI.h"
#include "Datum.h"

namespace FieaGameEngine
{
	template <>
	inline void Datum::CreateDefault<std::int32_t>(size_t index)
	{
		new (mStorage.Integer + index)std::int32_t(0);
	}

	template <>
	inline void Datum::CreateDefault<float>(size_t index)
	{
		new (mStorage.Float + index)float(0);
	}

	template <>
	inline void Datum::CreateDefault<glm::vec4>(size_t index)
	{
		new (mStorage.Vector + index)glm::vec4(0.0f);
	}

	template <>
	inline void Datum::CreateDefault<glm::mat4>(size_t index)
	{
		new (mStorage.Matrix + index)glm::vec4(0.0f);
	}

	template <>
	inline void Datum::CreateDefault<std::string>(size_t index)
	{
		new (mStorage.String + index)std::string();
	}

	template <>
	inline void Datum::CreateDefault<RTTI*>(size_t index)
	{
		new (mStorage.Pointer + index)RTTI*(nullptr);
	}

	template <>
	inline std::string Datum::ToString<std::int32_t>(size_t index) const
	{
		return std::to_string(mStorage.Integer[index]);
	}

	template <>
	inline std::string Datum::ToString<float>(size_t index) const
	{
		return std::to_string(mStorage.Float[index]);
	}

	template <>
	inline std::string Datum::ToString<glm::vec4>(size_t index) const
	{
		return glm::to_string(mStorage.Vector[index]);
	}

	template <>
	inline std::string Datum::ToString<glm::mat4>(size_t index) const
	{
		return glm::to_string(mStorage.Matrix[index]);
	}

	template <>
	inline std::string Datum::ToString<std::string>(size_t index) const
	{
		return mStorage.String[index];
	}

	template <>
	inline std::string Datum::ToString<RTTI*>(size_t index) const
	{
		if (mStorage.Pointer[index] == nullptr)
		{
			throw std::exception("Null RTTI pointer.");
		}

		return mStorage.Pointer[index]->ToString();
	}

	template <>
	inline void Datum::SetFromString<std::int32_t>(const std::string& string, size_t index)
	{
		mStorage.Integer[index] = std::stoi(string);
	}

	template <>
	inline void Datum::SetFromString<float>(const std::string& string, size_t index)
	{
		mStorage.Float[index] = std::stof(string);
	}

	template <>
	inline void Datum::SetFromString<glm::vec4>(const std::string& string, size_t index)
	{
		mStorage.Vector[index] = StringToVector(string);
	}

	template <>
	inline void Datum::SetFromString<glm::mat4>(const std::string& string, size_t index)
	{
		mStorage.Matrix[index] = StringToMatrix(string);
	}

	template <>
	inline void Datum::SetFromString<std::string>(const std::string& string, size_t index)
	{
		mStorage.String[index] = string;
	}

	template <>
	inline void Datum::SetFromString<RTTI*>(const std::string&, size_t)
	{
		throw std::exception("Cannot set pointers from strings.");
	}

	size_t Datum::sTypeSizes[] =
	{
		0,
		sizeof(std::int32_t),
		sizeof(float),
		sizeof(glm::vec4),
		sizeof(glm::mat4),
		sizeof(std::string),
		sizeof(RTTI*),
		sizeof(Scope*)
	};

	const Datum::EqualityFunc Datum::sEqualityFuncs[] = 
	{
		&Datum::ComparePOD,
		&Datum::ComparePOD,
		&Datum::ComparePOD,
		&Datum::ComparePOD,
		&Datum::ComparePOD,
		&Datum::CompareStrings,
		&Datum::ComparePointers,
		&Datum::ComparePointers
	};

	const Datum::DeepCopyFunc Datum::sDeepCopyFuncs[] =
	{
		&Datum::DeepCopyPOD,
		&Datum::DeepCopyPOD,
		&Datum::DeepCopyPOD,
		&Datum::DeepCopyPOD,
		&Datum::DeepCopyPOD,
		&Datum::DeepCopyStrings,
		&Datum::DeepCopyPOD,
		&Datum::DeepCopyPOD
	};

	const Datum::CreateDefaultFunc Datum::sCreateDefaultFuncs[] =
	{
		nullptr,
		&Datum::CreateDefault<std::int32_t>,
		&Datum::CreateDefault<float>,
		&Datum::CreateDefault<glm::vec4>,
		&Datum::CreateDefault<glm::mat4>,
		&Datum::CreateDefault<std::string>,
		&Datum::CreateDefault<RTTI*>,
		nullptr
	};

	const Datum::ToStringFunc Datum::sToStringFuncs[] =
	{
		nullptr,
		&Datum::ToString<std::int32_t>,
		&Datum::ToString<float>,
		&Datum::ToString<glm::vec4>,
		&Datum::ToString<glm::mat4>,
		&Datum::ToString<std::string>,
		&Datum::ToString<RTTI*>,
		&Datum::ToString<RTTI*>
	};

	const Datum::SetFromStringFunc Datum::sSetFromStringFuncs[] =
	{
		nullptr,
		&Datum::SetFromString<std::int32_t>,
		&Datum::SetFromString<float>,
		&Datum::SetFromString<glm::vec4>,
		&Datum::SetFromString<glm::mat4>,
		&Datum::SetFromString<std::string>,
		&Datum::SetFromString<RTTI*>,
		&Datum::SetFromString<RTTI*>
	};

	void Datum::CopyValues(const Datum& source, Datum& destination)
	{
		assert(destination.Type() != Datum::DatumType::UNKNOWN);
		destination.TypeGuard(source.Type());
		
		if (destination.mOwnsStorage)
		{
			destination.Resize(source.Size());
		}

		if (destination.Type() == Datum::DatumType::STRING)
		{
			destination.CopyAssignStrings(source);
		}
		else
		{
			destination.DeepCopyPOD(source);
		}
	}

	void Datum::MoveValues(const Datum& source, Datum& destination)
	{
		assert(destination.Type() != Datum::DatumType::UNKNOWN);
		destination.TypeGuard(source.Type());

		if (destination.mOwnsStorage)
		{
			destination.Resize(source.Size());
		}

		if (destination.Type() == Datum::DatumType::STRING)
		{
			destination.MoveAssignStrings(source);
		}
		else
		{
			destination.DeepCopyPOD(source);
		}
	}

	Datum::Datum(DatumType type, size_t capacity) :
		mType(type)
	{
		if (type == DatumType::UNKNOWN && capacity > 0)
		{
			throw std::exception("Type error.");
		}
		else if (type != DatumType::UNKNOWN)
		{
			Reserve(capacity);
		}
	}

	Datum::Datum(const Datum& other) :
		mType(other.mType)
	{
		if (other.mOwnsStorage)
		{
			if (mType != DatumType::UNKNOWN)
			{
				Reserve(other.mSize);
				DeepCopy(other);
			}
		}
		else
		{
			mStorage = other.mStorage;
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mOwnsStorage = false;
		}
	}

	Datum::Datum(Datum&& other) :
		mStorage(std::move(other.mStorage)), mType(other.mType), mSize(other.mSize), mCapacity(other.mCapacity), mOwnsStorage(other.mOwnsStorage)
	{
		other.mStorage.Integer = nullptr;
		other.mSize = 0;
		other.mCapacity = 0;
		other.mOwnsStorage = true;
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			Destroy();
			mType = other.mType;

			if (other.mOwnsStorage)
			{
				Reserve(other.mSize);
				DeepCopy(other);
			}
			else
			{
				FreeMemory();

				mStorage = other.mStorage;
				mSize = other.mSize;
				mCapacity = other.mCapacity;
				mOwnsStorage = false;
			}
		}

		return *this;
	}

	Datum& Datum::operator=(Datum&& other)
	{
		if (this != &other)
		{
			Destroy();

			mStorage = other.mStorage;
			mType = other.mType;
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mOwnsStorage = other.mOwnsStorage;

			other.mStorage.Integer = nullptr;
			other.mSize = 0;
			other.mCapacity = 0;
			other.mOwnsStorage = true;
		}

		return *this;
	}

	Datum::~Datum()
	{
		Destroy();
	}

	Datum::Datum(const std::int32_t& value) :
		mType(DatumType::INTEGER)
	{
		Resize(1, value);
	}

	Datum::Datum(const float& value) :
		mType(DatumType::FLOAT)
	{
		Resize(1, value);
	}

	Datum::Datum(const glm::vec4& value) :
		mType(DatumType::VECTOR)
	{
		Resize(1, value);
	}

	Datum::Datum(const glm::mat4& value) :
		mType(DatumType::MATRIX)
	{
		Resize(1, value);
	}

	Datum::Datum(const std::string& value) :
		mType(DatumType::STRING)
	{
		Resize(1, value);
	}

	Datum::Datum(std::string&& value) :
		mType(DatumType::STRING)
	{
		Resize(1, std::move(value));
	}

	Datum::Datum(RTTI* value) :
		mType(DatumType::POINTER)
	{
		Resize(1, value);
	}

	Datum::Datum(Scope& value) :
		mType(DatumType::TABLE)
	{
		Resize(1, value);
	}

	Datum::Datum(std::initializer_list<std::int32_t> list) :
		mType(DatumType::INTEGER)
	{
		Reserve(list.size());
		CopyInitializerList(list);
	}

	Datum::Datum(std::initializer_list<float> list) :
		mType(DatumType::FLOAT)
	{
		Reserve(list.size());
		CopyInitializerList(list);
	}

	Datum::Datum(std::initializer_list<glm::vec4> list) :
		mType(DatumType::VECTOR)
	{
		Reserve(list.size());
		CopyInitializerList(list);
	}

	Datum::Datum(std::initializer_list<glm::mat4> list) :
		mType(DatumType::MATRIX)
	{
		Reserve(list.size());
		CopyInitializerList(list);
	}

	Datum::Datum(std::initializer_list<std::string> list) :
		mType(DatumType::STRING)
	{
		Reserve(list.size());
		CopyInitializerList(list);
	}

	Datum::Datum(std::initializer_list<RTTI*> list) :
		mType(DatumType::POINTER)
	{
		Reserve(list.size());
		CopyInitializerList(list);
	}

	Datum& Datum::operator=(const std::int32_t& value)
	{
		if (mSize == 0)
		{
			PushBack(value);
		}
		else
		{
			TypeGuard(DatumType::INTEGER);

			if (!mOwnsStorage)
			{
				mSize = mCapacity = 1;
			}

			mStorage.Integer[0] = value;
		}

		return *this;
	}

	Datum& Datum::operator=(const float& value)
	{
		if (mSize == 0)
		{
			PushBack(value);
		}
		else
		{
			TypeGuard(DatumType::FLOAT);

			if (!mOwnsStorage)
			{
				mSize = mCapacity = 1;
			}

			mStorage.Float[0] = value;
		}

		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& value)
	{
		if (mSize == 0)
		{
			PushBack(value);
		}
		else
		{
			TypeGuard(DatumType::VECTOR);

			if (!mOwnsStorage)
			{
				mSize = mCapacity = 1;
			}

			mStorage.Vector[0] = value;
		}

		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& value)
	{
		if (mSize == 0)
		{
			PushBack(value);
		}
		else
		{
			TypeGuard(DatumType::MATRIX);

			if (!mOwnsStorage)
			{
				mSize = mCapacity = 1;
			}

			mStorage.Matrix[0] = value;
		}

		return *this;
	}

	Datum& Datum::operator=(const std::string& value)
	{
		if (mSize == 0)
		{
			PushBack(value);
		}
		else
		{
			TypeGuard(DatumType::STRING);

			if (!mOwnsStorage)
			{
				mSize = mCapacity = 1;
			}

			mStorage.String[0] = value;
		}

		return *this;
	}

	Datum& Datum::operator=(std::string&& value)
	{
		if (mSize == 0)
		{
			PushBack(value);
		}
		else
		{
			TypeGuard(DatumType::STRING);

			if (!mOwnsStorage)
			{
				mSize = mCapacity = 1;
			}

			mStorage.String[0] = std::move(value);
		}

		return *this;
	}

	Datum& Datum::operator=(RTTI* value)
	{
		if (mSize == 0)
		{
			PushBack(value);
		}
		else
		{
			TypeGuard(DatumType::POINTER);

			if (!mOwnsStorage)
			{
				mSize = mCapacity = 1;
			}

			mStorage.Pointer[0] = value;
		}

		return *this;
	}

	Datum& Datum::operator=(Scope& value)
	{
		if (mSize == 0)
		{
			PushBack(value);
		}
		else
		{
			TypeGuard(DatumType::TABLE);

			if (!mOwnsStorage)
			{
				mSize = mCapacity = 1;
			}

			mStorage.Table[0] = &value;
		}

		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<std::int32_t> list)
	{
		Clear();
		mType = DatumType::INTEGER;
		Reserve(list.size());
		CopyInitializerList(list);

		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<float> list)
	{
		Clear();
		mType = DatumType::FLOAT;
		Reserve(list.size());
		CopyInitializerList(list);

		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<glm::vec4> list)
	{
		Clear();
		mType = DatumType::VECTOR;
		Reserve(list.size());
		CopyInitializerList(list);

		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<glm::mat4> list)
	{
		Clear();
		mType = DatumType::MATRIX;
		Reserve(list.size());
		CopyInitializerList(list);

		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<std::string> list)
	{
		Clear();
		mType = DatumType::STRING;
		Reserve(list.size());
		CopyInitializerList(list);

		return *this;
	}

	Datum& Datum::operator=(std::initializer_list<RTTI*> list)
	{
		Clear();
		mType = DatumType::POINTER;
		Reserve(list.size());
		CopyInitializerList(list);

		return *this;
	}

	bool Datum::operator==(const Datum& other) const
	{
		if (mType != other.mType || mSize != other.mSize)
		{
			return false;
		}

		EqualityFunc func = sEqualityFuncs[TypeOffset()];
		return std::invoke(func, this, other);
	}

	bool Datum::operator!=(const Datum& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const std::int32_t& other) const
	{
		if (mType != DatumType::INTEGER || mSize == 0)
		{
			return false;
		}

		return mStorage.Integer[0] == other;
	}

	bool Datum::operator!=(const std::int32_t& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const float& other) const
	{
		if (mType != DatumType::FLOAT || mSize == 0)
		{
			return false;
		}

		return mStorage.Float[0] == other;
	}

	bool Datum::operator!=(const float& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const glm::vec4& other) const
	{
		if (mType != DatumType::VECTOR || mSize == 0)
		{
			return false;
		}

		return mStorage.Vector[0] == other;
	}

	bool Datum::operator!=(const glm::vec4& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const glm::mat4& other) const
	{
		if (mType != DatumType::MATRIX || mSize == 0)
		{
			return false;
		}

		return mStorage.Matrix[0] == other;
	}

	bool Datum::operator!=(const glm::mat4& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const std::string& other) const
	{
		if (mType != DatumType::STRING || mSize == 0)
		{
			return false;
		}

		return mStorage.String[0] == other;
	}

	bool Datum::operator!=(const std::string& other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const RTTI* other) const
	{
		if (mType != DatumType::POINTER || mSize == 0)
		{
			return false;
		}

		return (mStorage.Pointer[0] == nullptr ? nullptr == other : mStorage.Pointer[0]->Equals(other));
	}

	bool Datum::operator!=(const RTTI* other) const
	{
		return !operator==(other);
	}

	bool Datum::operator==(const Scope& other) const
	{
		if (mType != DatumType::TABLE || mSize == 0)
		{
			return false;
		}

		assert(mStorage.Pointer[0] != nullptr);
		return mStorage.Pointer[0]->Equals(reinterpret_cast<const RTTI*>(&other));
	}

	bool Datum::operator!=(const Scope& other) const
	{
		return !operator==(other);
	}

	Scope& Datum::operator[](size_t index)
	{
		TypeGuard(DatumType::TABLE);
		IndexGuard(index);

		if (mStorage.Table[index] == nullptr)
		{
			throw std::exception("Cannot create a reference to a null scope.");
		}

		return *(mStorage.Table[index]);
	}

	const Scope& Datum::operator[](size_t index) const
	{
		return const_cast<Datum*>(this)->operator[](index);
	}

	void Datum::PushBack(const std::int32_t& value)
	{
		PushBackPreCheck(DatumType::INTEGER);

		new (mStorage.Integer + mSize)std::int32_t(value);
		++mSize;
	}

	void Datum::PushBack(const float& value)
	{
		PushBackPreCheck(DatumType::FLOAT);

		new (mStorage.Float + mSize)float(value);
		++mSize;
	}

	void Datum::PushBack(const glm::vec4& value)
	{
		PushBackPreCheck(DatumType::VECTOR);

		new (mStorage.Vector + mSize)glm::vec4(value);
		++mSize;
	}

	void Datum::PushBack(const glm::mat4& value)
	{
		PushBackPreCheck(DatumType::MATRIX);

		new (mStorage.Matrix + mSize)glm::mat4(value);
		++mSize;
	}

	void Datum::PushBack(const std::string& value)
	{
		PushBackPreCheck(DatumType::STRING);

		new (mStorage.String + mSize)std::string(value);
		++mSize;
	}

	void Datum::PushBack(std::string&& value)
	{
		PushBackPreCheck(DatumType::STRING);

		new (mStorage.String + mSize)std::string(std::move(value));
		++mSize;
	}

	void Datum::PushBack(RTTI* value)
	{
		PushBackPreCheck(DatumType::POINTER);

		new (mStorage.Pointer + mSize)RTTI*(value);
		++mSize;
	}

	void Datum::PushBack(Scope& value)
	{
		PushBackPreCheck(DatumType::TABLE);

		new (mStorage.Table + mSize)Scope*(&value);
		++mSize;
	}

	void Datum::Set(const std::int32_t& value, size_t index)
	{
		TypeGuard(DatumType::INTEGER);
		IndexGuard(index);

		new (mStorage.Integer + index)std::int32_t(value);
	}

	void Datum::Set(const float& value, size_t index)
	{
		TypeGuard(DatumType::FLOAT);
		IndexGuard(index);

		new (mStorage.Float + index)float(value);
	}

	void Datum::Set(const glm::vec4& value, size_t index)
	{
		TypeGuard(DatumType::VECTOR);
		IndexGuard(index);

		new (mStorage.Vector + index)glm::vec4(value);
	}

	void Datum::Set(const glm::mat4& value, size_t index)
	{
		TypeGuard(DatumType::MATRIX);
		IndexGuard(index);

		new (mStorage.Matrix + index)glm::mat4(value);
	}

	void Datum::Set(const std::string& value, size_t index)
	{
		TypeGuard(DatumType::STRING);
		IndexGuard(index);

		*(mStorage.String + index) = value;
	}

	void Datum::Set(std::string&& value, size_t index)
	{
		TypeGuard(DatumType::STRING);
		IndexGuard(index);

		*(mStorage.String + index) = std::move(value);
	}

	void Datum::Set(RTTI* value, size_t index)
	{
		TypeGuard(DatumType::POINTER);
		IndexGuard(index);

		new (mStorage.Pointer + index)RTTI*(value);
	}

	void Datum::Set(Scope& value, size_t index)
	{
		TypeGuard(DatumType::TABLE);
		IndexGuard(index);

		new (mStorage.Table + index)Scope*(&value);
	}

	void Datum::SetFromString(const std::string& string, size_t index)
	{
		IndexGuard(index);

		auto func = sSetFromStringFuncs[TypeOffset()];
		assert(func != nullptr);
		std::invoke(func, this, string, index);
	}

	void Datum::SetStorage(std::int32_t* data, size_t size)
	{
		return SetStorage(data, size, DatumType::INTEGER);
	}

	void Datum::SetStorage(float* data, size_t size)
	{
		return SetStorage(data, size, DatumType::FLOAT);
	}

	void Datum::SetStorage(glm::vec4* data, size_t size)
	{
		return SetStorage(data, size, DatumType::VECTOR);
	}

	void Datum::SetStorage(glm::mat4* data, size_t size)
	{
		return SetStorage(data, size, DatumType::MATRIX);
	}

	void Datum::SetStorage(std::string* data, size_t size)
	{
		return SetStorage(data, size, DatumType::STRING);
	}

	void Datum::SetStorage(RTTI** data, size_t size)
	{
		return SetStorage(data, size, DatumType::POINTER);
	}

	Datum::Iterator Datum::Find(const std::int32_t& value)
	{
		TypeGuard(DatumType::INTEGER);

		for (size_t i = 0; i < mSize; ++i)
		{
			if (mStorage.Integer[i] == value)
			{
				return Iterator(*this, i);
			}
		}

		return end();
	}

	Datum::Iterator Datum::Find(const float& value)
	{
		TypeGuard(DatumType::FLOAT);

		for (size_t i = 0; i < mSize; ++i)
		{
			if (mStorage.Float[i] == value)
			{
				return Iterator(*this, i);
			}
		}

		return end();
	}

	Datum::Iterator Datum::Find(const glm::vec4& value)
	{
		TypeGuard(DatumType::VECTOR);

		for (size_t i = 0; i < mSize; ++i)
		{
			if (mStorage.Vector[i] == value)
			{
				return Iterator(*this, i);
			}
		}

		return end();
	}

	Datum::Iterator Datum::Find(const glm::mat4& value)
	{
		TypeGuard(DatumType::MATRIX);

		for (size_t i = 0; i < mSize; ++i)
		{
			if (mStorage.Matrix[i] == value)
			{
				return Iterator(*this, i);
			}
		}

		return end();
	}

	Datum::Iterator Datum::Find(const std::string& value)
	{
		TypeGuard(DatumType::STRING);

		for (size_t i = 0; i < mSize; ++i)
		{
			if (mStorage.String[i] == value)
			{
				return Iterator(*this, i);
			}
		}

		return end();
	}

	Datum::Iterator Datum::Find(const RTTI* value)
	{
		TypeGuard(DatumType::POINTER);

		for (size_t i = 0; i < mSize; ++i)
		{
			if (mStorage.Pointer[i] == nullptr ? value == nullptr : mStorage.Pointer[i]->Equals(value))
			{
				return Iterator(*this, i);
			}
		}

		return end();
	}

	Datum::Iterator Datum::Find(const Scope& value)
	{
		TypeGuard(DatumType::TABLE);

		for (size_t i = 0; i < mSize; ++i)
		{
			assert(mStorage.Pointer[i] != nullptr);
			if (mStorage.Pointer[i]->Equals(reinterpret_cast<const RTTI*>(&value)))
			{
				return Iterator(*this, i);
			}
		}

		return end();
	}

	Datum::ConstIterator Datum::Find(const std::int32_t& value) const
	{
		return const_cast<Datum*>(this)->Find(value);
	}

	Datum::ConstIterator Datum::Find(const float& value) const
	{
		return const_cast<Datum*>(this)->Find(value);
	}

	Datum::ConstIterator Datum::Find(const glm::vec4& value) const
	{
		return const_cast<Datum*>(this)->Find(value);
	}

	Datum::ConstIterator Datum::Find(const glm::mat4& value) const
	{
		return const_cast<Datum*>(this)->Find(value);
	}

	Datum::ConstIterator Datum::Find(const std::string& value) const
	{
		return const_cast<Datum*>(this)->Find(value);
	}

	Datum::ConstIterator Datum::Find(const RTTI* value) const
	{
		return const_cast<Datum*>(this)->Find(value);
	}

	Datum::ConstIterator Datum::Find(const Scope& value) const
	{
		return const_cast<Datum*>(this)->Find(value);
	}

	bool Datum::Remove(const std::int32_t& value)
	{
		TypeGuard(DatumType::INTEGER);

		return Remove(Find(value));
	}

	bool Datum::Remove(const float& value)
	{
		TypeGuard(DatumType::FLOAT);

		return Remove(Find(value));
	}

	bool Datum::Remove(const glm::vec4& value)
	{
		TypeGuard(DatumType::VECTOR);

		return Remove(Find(value));
	}

	bool Datum::Remove(const glm::mat4& value)
	{
		TypeGuard(DatumType::MATRIX);

		return Remove(Find(value));
	}

	bool Datum::Remove(const std::string& value)
	{
		TypeGuard(DatumType::STRING);

		return Remove(Find(value));
	}

	bool Datum::Remove(const RTTI* value)
	{
		TypeGuard(DatumType::POINTER);

		return Remove(Find(value));
	}

	bool Datum::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::exception("Invalid iterator.");
		}

		TypeGuard(it.Type());

		bool removed = false;
		if (it != end())
		{
			RemoveAt(it.mOffset);
			removed = true;
		}

		return removed;
	}

	void Datum::RemoveAt(size_t index)
	{
		IndexGuard(index);
		ExternalStorageGuard();

		switch (mType)
		{
		case DatumType::INTEGER:
			std::memmove(mStorage.Integer + index, mStorage.Integer + index + 1, sizeof(std::int32_t) * (mSize - (index + 1)));
			break;
		case DatumType::FLOAT:
			std::memmove(mStorage.Float + index, mStorage.Float + index + 1, sizeof(float) * (mSize - (index + 1)));
			break;
		case DatumType::VECTOR:
			std::memmove(mStorage.Vector + index, mStorage.Vector + index + 1, sizeof(glm::vec4) * (mSize - (index + 1)));
			break;
		case DatumType::MATRIX:
			std::memmove(mStorage.Matrix + index, mStorage.Matrix + index + 1, sizeof(glm::mat4) * (mSize - (index + 1)));
			break;
		case DatumType::STRING:
			mStorage.String[index].~basic_string();
			std::memmove(mStorage.String + index, mStorage.String + index + 1, sizeof(std::string) * (mSize - (index + 1)));
			break;
		case DatumType::POINTER:
			std::memmove(mStorage.Pointer + index, mStorage.Pointer + index + 1, sizeof(RTTI*) * (mSize - (index + 1)));
			break;
		case DatumType::TABLE:
			std::memmove(mStorage.Table + index, mStorage.Table + index + 1, sizeof(Scope*) * (mSize - (index + 1)));
			break;
		case DatumType::UNKNOWN:
		default:
			throw std::exception("Not supposed to happen.");
		}

		--mSize;
	}

	void Datum::PopBack()
	{
		return RemoveAt(mSize > 0 ? mSize - 1 : 0);
	}

	void Datum::Resize(size_t size)
	{
		if (mType == DatumType::TABLE)
		{
			throw std::exception("Not allowed.");
		}

		UnknownTypeGuard();
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				Reserve(size);
			}

			for (size_t i = mSize; i < size; ++i)
			{
				auto func = sCreateDefaultFuncs[TypeOffset()];
				assert(func != nullptr);
				std::invoke(func, this, i);
			}
		}
		else if (size < mSize)
		{
			if (mType == DatumType::STRING)
			{
				for (std::string* pointer = mStorage.String; pointer < mStorage.String + mSize; ++pointer)
				{
					pointer->~basic_string();
				}
			}
		}

		mSize = size;
	}

	void Datum::Resize(size_t size, const std::int32_t& value)
	{
		TypeGuard(DatumType::INTEGER);
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				mType = DatumType::INTEGER;
				Reserve(size);
			}

			for (std::int32_t* pointer = mStorage.Integer + mSize; pointer < mStorage.Integer + size; ++pointer)
			{
				new (pointer)std::int32_t(value);
			}
		}

		mSize = size;
	}

	void Datum::Resize(size_t size, const float& value)
	{
		TypeGuard(DatumType::FLOAT);
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				mType = DatumType::FLOAT;
				Reserve(size);
			}

			for (float* pointer = mStorage.Float + mSize; pointer < mStorage.Float + size; ++pointer)
			{
				new (pointer)float(value);
			}
		}

		mSize = size;
	}

	void Datum::Resize(size_t size, const glm::vec4& value)
	{
		TypeGuard(DatumType::VECTOR);
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				mType = DatumType::VECTOR;
				Reserve(size);
			}

			for (glm::vec4* pointer = mStorage.Vector + mSize; pointer < mStorage.Vector + size; ++pointer)
			{
				new (pointer)glm::vec4(value);
			}
		}

		mSize = size;
	}

	void Datum::Resize(size_t size, const glm::mat4& value)
	{
		TypeGuard(DatumType::MATRIX);
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				mType = DatumType::MATRIX;
				Reserve(size);
			}

			for (glm::mat4* pointer = mStorage.Matrix + mSize; pointer < mStorage.Matrix + size; ++pointer)
			{
				new (pointer)glm::mat4(value);
			}
		}

		mSize = size;
	}

	void Datum::Resize(size_t size, const std::string& value)
	{
		TypeGuard(DatumType::STRING);
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				mType = DatumType::STRING;
				Reserve(size);
			}

			for (std::string* pointer = mStorage.String + mSize; pointer < mStorage.String + size; ++pointer)
			{
				new (pointer)std::string(value);
			}
		}
		else if (size < mSize)
		{
			for (std::string* pointer = mStorage.String; pointer < mStorage.String + mSize; ++pointer)
			{
				(*pointer).~basic_string();
			}
		}

		mSize = size;
	}

	void Datum::Resize(size_t size, std::string&& value)
	{
		TypeGuard(DatumType::STRING);
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				mType = DatumType::STRING;
				Reserve(size);
			}

			for (std::string* pointer = mStorage.String + mSize; pointer < mStorage.String + size; ++pointer)
			{
				new (pointer)std::string(std::move(value));
			}
		}
		else if (size < mSize)
		{
			for (std::string* pointer = mStorage.String; pointer < mStorage.String + mSize; ++pointer)
			{
				(*pointer).~basic_string();
			}
		}

		mSize = size;
	}

	void Datum::Resize(size_t size, RTTI* value)
	{
		TypeGuard(DatumType::POINTER);
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				mType = DatumType::POINTER;
				Reserve(size);
			}

			for (RTTI** pointer = mStorage.Pointer + mSize; pointer < mStorage.Pointer + mCapacity; ++pointer)
			{
				new (pointer)RTTI*(value);
			}
		}

		mSize = size;
	}

	void Datum::Resize(size_t size, Scope& value)
	{
		TypeGuard(DatumType::TABLE);
		ExternalStorageGuard();

		if (size > mSize)
		{
			if (size > mCapacity)
			{
				mType = DatumType::TABLE;
				Reserve(size);
			}

			for (Scope** pointer = mStorage.Table + mSize; pointer < mStorage.Table + mCapacity; ++pointer)
			{
				new (pointer)Scope*(&value);
			}
		}

		mSize = size;
	}

	void Datum::Reserve(size_t capacity)
	{
		UnknownTypeGuard();
		ExternalStorageGuard();

		if (capacity > mCapacity)
		{
			mStorage.Wildcard = std::realloc(mStorage.Wildcard, sTypeSizes[TypeOffset()] * capacity);
			mCapacity = capacity;
		}
	}

	void Datum::ShrinkToFit()
	{
		UnknownTypeGuard();
		ExternalStorageGuard();

		if (mCapacity > mSize)
		{
			mStorage.Wildcard = std::realloc(mStorage.Wildcard, sTypeSizes[TypeOffset()] * mSize);
			mCapacity = mSize;
		}
	}

	void Datum::Clear()
	{
		if (mOwnsStorage)
		{
			if (mType == DatumType::STRING)
			{
				for (std::string* pointer = mStorage.String; pointer < mStorage.String + mSize; ++pointer)
				{
					(*pointer).~basic_string();
				}
			}
		}
		else
		{
			mCapacity = 0;
			mStorage.Wildcard = nullptr;
		}

		mSize = 0;
		mOwnsStorage = true;
	}

	Datum::Iterator Datum::begin()
	{
		return Iterator(*this);
	}

	Datum::ConstIterator Datum::begin() const
	{
		return ConstIterator(*this);
	}

	Datum::Iterator Datum::end()
	{
		return Iterator(*this, mSize);
	}

	Datum::ConstIterator Datum::end() const
	{
		return ConstIterator(*this, mSize);
	}

	Datum::ConstIterator Datum::cbegin() const
	{
		return ConstIterator(*this);
	}

	Datum::ConstIterator Datum::cend() const
	{
		return ConstIterator(*this, mSize);
	}

	Datum::DatumType Datum::Type() const
	{
		return mType;
	}

	void Datum::SetType(DatumType type)
	{
		if ((mType != DatumType::UNKNOWN && mType != type) || (mType != DatumType::UNKNOWN && type == DatumType::UNKNOWN))
		{
			throw std::exception("Type already set.");
		}

		mType = type;
	}

	size_t Datum::Size() const
	{
		return mSize;
	}

	size_t Datum::Capacity() const
	{
		return mCapacity;
	}

	bool Datum::OwnsStorage() const
	{
		return mOwnsStorage;
	}

	std::string Datum::ToString(size_t index) const
	{
		IndexGuard(index);

		auto func = sToStringFuncs[TypeOffset()];
		assert(func != nullptr);
		return std::invoke(func, this, index);
	}

	void Datum::SetStorage(void* data, size_t size, DatumType type)
	{
		if (size == 0)
		{
			throw std::exception("Cannot set empty external storage.");
		}

		TypeGuard(type);

		if (mCapacity > 0)
		{
			Destroy();
		}
		else
		{
			mType = type;
		}

		mStorage.Wildcard = data;
		mSize = size;
		mCapacity = size;
		mOwnsStorage = false;
	}

	void Datum::DeepCopy(const Datum& other)
	{
		assert(mType != DatumType::UNKNOWN);

		std::invoke(sDeepCopyFuncs[TypeOffset()], this, other);
		mSize = other.mSize;
	}

	void Datum::FreeMemory()
	{
		if (mOwnsStorage && mType != DatumType::UNKNOWN)
		{
			free(mStorage.Wildcard);
			mStorage.Wildcard = nullptr;
			mCapacity = 0;
		}
	}

	inline void Datum::Destroy()
	{
		Clear();
		FreeMemory();
	}

	inline void Datum::TypeGuard(DatumType type) const
	{
		if (mType != DatumType::UNKNOWN && mType != type)
		{
			throw std::exception("Type error.");
		}
	}

	inline void Datum::UnknownTypeGuard() const
	{
		if (mType == DatumType::UNKNOWN)
		{
			throw std::exception("Type not set.");
		}
	}

	inline void Datum::IndexGuard(size_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range.");
		}
	}

	inline void Datum::ExternalStorageGuard() const
	{
		if (!mOwnsStorage)
		{
			throw std::exception("Can't modify external storage.");
		}
	}

	inline void Datum::PushBackPreCheck(DatumType type)
	{
		if (mCapacity == 0 && mType == DatumType::UNKNOWN)
		{
			mType = type;
			Reserve(1);
		}
		else
		{
			TypeGuard(type);
			ExternalStorageGuard();

			if (mSize == mCapacity)
			{
				Reserve(std::max(mCapacity + 1, mCapacity * 2));
			}
		}
	}

	glm::vec4 Datum::StringToVector(const std::string& string) const
	{
		glm::vec4 vector;
		int result = sscanf_s(string.c_str(), " vec4( %f , %f , %f , %f ) ", &vector[0], &vector[1], &vector[2], &vector[3]);

		if (result != 4)
		{
			throw std::exception("Malformed string.");
		}

		return vector;
	}

	glm::mat4 Datum::StringToMatrix(const std::string& string) const
	{
		glm::mat4 matrix;
		int result = sscanf_s(string.c_str(), " mat4x4( ( %f , %f , %f , %f ) , ( %f , %f , %f , %f ) , ( %f , %f , %f , %f ) , ( %f , %f , %f , %f ) ) ",
			&matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3], &matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3],
			&matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3], &matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3]);

		if (result != 16)
		{
			throw std::exception("Malformed string.");
		}

		return matrix;
	}

	inline size_t Datum::TypeOffset() const
	{
		return static_cast<size_t>(mType);
	}

	bool Datum::ComparePOD(const Datum& other) const
	{
		return !std::memcmp(mStorage.Wildcard, other.mStorage.Wildcard, sTypeSizes[TypeOffset()] * mSize);
	}

	bool Datum::CompareStrings(const Datum& other) const
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			if (mStorage.String[i] != other.mStorage.String[i])
			{
				return false;
			}
		}

		return true;
	}

	bool Datum::ComparePointers(const Datum& other) const
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			const RTTI* lhs = mStorage.Pointer[i];
			const RTTI* rhs = other.mStorage.Pointer[i];

			if (lhs == rhs)
			{
				continue;
			}
			else if (lhs == nullptr)
			{
				if (rhs != nullptr)
				{
					return false;
				}

				continue;
			}
			else if (!lhs->Equals(rhs))
			{
				return false;
			}
		}

		return true;
	}

	void Datum::DeepCopyPOD(const Datum& other)
	{
		auto typeSize = sTypeSizes[TypeOffset()];
		auto minSize = std::min(typeSize * mCapacity, typeSize * other.mSize);
		memcpy_s(mStorage.Wildcard, minSize, other.mStorage.Wildcard, minSize);
	}

	void Datum::DeepCopyStrings(const Datum& other)
	{
		for (size_t i = 0; i < other.mSize; ++i)
		{
			new (mStorage.String + i)std::string(other.mStorage.String[i]);
		}
	}

	void Datum::CopyAssignStrings(const Datum& other)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			mStorage.String[i] = other.mStorage.String[i];
		}
	}

	void Datum::MoveAssignStrings(const Datum& other)
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			mStorage.String[i] = std::move(other.mStorage.String[i]);
		}
	}

	// Iterator

	Datum::Iterator::Iterator(const Datum& owner, size_t offset) :
		mOwner(&owner), mOffset(offset)
	{		
	}

	bool Datum::Iterator::operator==(const Iterator& other) const
	{
		return !operator!=(other);
	}

	bool Datum::Iterator::operator!=(const Iterator& other) const
	{
		return (mOwner != other.mOwner) || (mOffset != other.mOffset);
	}

	Datum::Iterator& Datum::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mOffset < mOwner->Size())
		{
			++mOffset;
		}

		return *this;
	}

	Datum::Iterator Datum::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();

		return temp;
	}

	void* Datum::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mOffset >= mOwner->Size())
		{
			throw std::exception("Out of range iterator.");
		}

		switch (mOwner->mType)
		{
		case DatumType::INTEGER:
			return mOwner->mStorage.Integer + mOffset;
		case DatumType::FLOAT:
			return mOwner->mStorage.Float + mOffset;
		case DatumType::VECTOR:
			return mOwner->mStorage.Vector + mOffset;
		case DatumType::MATRIX:
			return mOwner->mStorage.Matrix + mOffset;
		case DatumType::STRING:
			return mOwner->mStorage.String + mOffset;
		case DatumType::POINTER:
			return mOwner->mStorage.Pointer + mOffset;
		case DatumType::TABLE:
			return mOwner->mStorage.Table + mOffset;
		case DatumType::UNKNOWN:
		default:
			throw std::exception("Type error.");
		}
	}

	Datum::DatumType Datum::Iterator::Type() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		return mOwner->Type();
	}

	// ConstIterator

	Datum::ConstIterator::ConstIterator(const Iterator& other) :
		mOwner(other.mOwner), mOffset(other.mOffset)
	{
	}

	Datum::ConstIterator::ConstIterator(Iterator&& other) :
		mOwner(other.mOwner), mOffset(other.mOffset)
	{
		other.mOwner = nullptr;
		other.mOffset = 0;
	}

	Datum::ConstIterator& Datum::ConstIterator::operator=(const Iterator& other)
	{
		mOwner = other.mOwner;
		mOffset = other.mOffset;

		return *this;
	}

	Datum::ConstIterator& Datum::ConstIterator::operator=(Iterator&& other)
	{
		mOwner = other.mOwner;
		mOffset = other.mOffset;

		other.mOwner = nullptr;
		other.mOffset = 0;

		return *this;
	}

	Datum::ConstIterator::ConstIterator(const Datum& owner, size_t offset) :
		mOwner(&owner), mOffset(offset)
	{
	}

	bool Datum::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !operator!=(other);
	}

	bool Datum::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (mOwner != other.mOwner) || (mOffset != other.mOffset);
	}

	Datum::ConstIterator& Datum::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mOffset < mOwner->Size())
		{
			++mOffset;
		}

		return *this;
	}

	Datum::ConstIterator Datum::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();

		return temp;
	}

	void* Datum::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mOffset >= mOwner->Size())
		{
			throw std::exception("Out of range iterator.");
		}

		switch (mOwner->mType)
		{
		case DatumType::INTEGER:
			return mOwner->mStorage.Integer + mOffset;
		case DatumType::FLOAT:
			return mOwner->mStorage.Float + mOffset;
		case DatumType::VECTOR:
			return mOwner->mStorage.Vector + mOffset;
		case DatumType::MATRIX:
			return mOwner->mStorage.Matrix + mOffset;
		case DatumType::STRING:
			return mOwner->mStorage.String + mOffset;
		case DatumType::POINTER:
			return mOwner->mStorage.Pointer + mOffset;
		case DatumType::TABLE:
			return mOwner->mStorage.Table + mOffset;
		case DatumType::UNKNOWN:
		default:
			throw std::exception("Type error.");
		}
	}

	Datum::DatumType Datum::ConstIterator::Type() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		return mOwner->Type();
	}
}
