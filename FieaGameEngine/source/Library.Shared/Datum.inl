#include "Datum.h"
#pragma warning(push)
#pragma warning(disable: 4201)
#include <glm/glm.hpp>
#pragma warning(pop)

namespace FieaGameEngine
{
	template <typename T>
	T& Datum::Get(size_t index)
	{
		static_assert(false, "Unsupported type.");
	}

	template <typename T>
	inline const T& Datum::Get(size_t index) const
	{
		return const_cast<Datum*>(this)->Get<T>(index);
	}

	template <>
	inline std::int32_t& Datum::Get<std::int32_t>(size_t index)
	{
		TypeGuard(DatumType::INTEGER);
		IndexGuard(index);

		return mStorage.Integer[index];
	}

	template <>
	inline float& Datum::Get<float>(size_t index)
	{
		TypeGuard(DatumType::FLOAT);
		IndexGuard(index);

		return mStorage.Float[index];
	}

	template <>
	inline glm::vec4& Datum::Get<glm::vec4>(size_t index)
	{
		TypeGuard(DatumType::VECTOR);
		IndexGuard(index);

		return mStorage.Vector[index];
	}

	template <>
	inline glm::mat4x4& Datum::Get<glm::mat4x4>(size_t index)
	{
		TypeGuard(DatumType::MATRIX);
		IndexGuard(index);

		return mStorage.Matrix[index];
	}

	template <>
	inline std::string& Datum::Get<std::string>(size_t index)
	{
		TypeGuard(DatumType::STRING);
		IndexGuard(index);

		return mStorage.String[index];
	}

	template <>
	inline RTTI*& Datum::Get<RTTI*>(size_t index)
	{
		TypeGuard(DatumType::POINTER);
		IndexGuard(index);

		return mStorage.Pointer[index];
	}

	template <>
	inline Scope*& Datum::Get<Scope*>(size_t index)
	{
		TypeGuard(DatumType::TABLE);
		IndexGuard(index);

		return mStorage.Table[index];
	}

	template <typename T>
	inline T& Datum::Front()
	{
		return Get<T>();
	}

	template <typename T>
	inline const T& Datum::Front() const
	{
		return const_cast<Datum*>(this)->Front<T>();
	}

	template <typename T>
	inline T& Datum::Back()
	{
		return Get<T>(mSize > 0 ? mSize - 1 : 0);
	}
	
	template <typename T>
	inline const T& Datum::Back() const
	{
		return const_cast<Datum*>(this)->Back<T>();
	}

	template <typename T>
	inline void Datum::CopyInitializerList(std::initializer_list<T> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template <typename T>
	inline void Datum::CreateDefault(size_t index)
	{
		static_assert(false, "Unsupported type.");
	}

	template <typename T>
	inline std::string Datum::ToString(size_t index) const
	{
		static_assert(false, "Unsupported type.");
	}

	template <typename T>
	void SetFromString(const std::string& string, size_t index)
	{
		static_assert(false, "Unsupported type.");
	}

	template <typename T>
	inline T& Datum::Iterator::Get() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Invalid iterator.");
		}

		return const_cast<Datum*>(mOwner)->Get<T>(mOffset);
	}

	template <typename T>
	inline const T& Datum::ConstIterator::Get() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Invalid iterator.");
		}

		return const_cast<Datum*>(mOwner)->Get<T>(mOffset);
	}
}
