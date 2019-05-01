/// \file vector.inl
/// \brief Implementation of Vector class

#include "vector.h"

namespace FieaGameEngine
{
	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::Vector(size_t capacity, std::function<size_t(size_t, size_t)> incrementCallback) :
		mSize(0), mIncrementCallback(incrementCallback)
	{
		Reserve(capacity);
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::Vector(const Vector& other) :
		Vector(other.mCapacity, other.mIncrementCallback)
	{
		mItemComparator = other.mItemComparator;
		DeepCopy(other);
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::Vector(Vector&& other) :
		mSize(other.mSize), mCapacity(other.mCapacity), mBuffer(other.mBuffer), mIncrementCallback(std::move(other.mIncrementCallback)), mItemComparator(std::move(other.mItemComparator))
	{
		other.mSize = 0;
		other.mCapacity = 0;
		other.mBuffer = nullptr;
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::Vector(std::initializer_list<T> list) :
		Vector(list.size())
	{
		for (const auto& data : list)
		{
			EmplaceBack(data);
		}
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::~Vector()
	{
		Clear();
		free(mBuffer);
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>& Vector<T, TItemComparator>::operator=(const Vector<T, TItemComparator>& other)
	{
		if (this != &other)
		{
			if (mSize > 0)
			{
				Clear();
			}

			if (other.mSize > mCapacity)
			{
				Reserve(other.mSize);
			}

			DeepCopy(other);
			mIncrementCallback = other.mIncrementCallback;
			mItemComparator = other.mItemComparator;
		}

		return *this;
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>& Vector<T, TItemComparator>::operator=(Vector&& other)
	{
		if (this != &other)
		{
			if (mSize > 0)
			{
				Clear();
			}

			if (mCapacity > 0)
			{
				free(mBuffer);
			}

			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mBuffer = other.mBuffer;
			mIncrementCallback = std::move(other.mIncrementCallback);
			mItemComparator = std::move(other.mItemComparator);

			other.mSize = 0;
			other.mCapacity = 0;
			other.mBuffer = nullptr;
		}

		return *this;
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>& Vector<T, TItemComparator>::operator=(std::initializer_list<T> list)
	{
		if (mSize > 0)
		{
			Clear();
		}

		if (list.size() > mCapacity)
		{
			Reserve(list.size());
		}

		for (const auto& data : list)
		{
			PushBack(data);
		}

		return *this;
	}

	template <typename T, typename TItemComparator>
	bool Vector<T, TItemComparator>::operator==(const Vector& other)
	{
		if (mSize != other.mSize)
		{
			return false;
		}

		for (size_t i = 0; i < mSize; ++i)
		{
			if (!mItemComparator(mBuffer[i], other[i]))
			{
				return false;
			}
		}

		return true;
	}

	template <typename T, typename TItemComparator>
	bool Vector<T, TItemComparator>::operator!=(const Vector& other)
	{
		return !operator==(other);
	}

	template <typename T, typename TItemComparator>
	inline T& Vector<T, TItemComparator>::operator[](size_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range.");
		}

		return *(mBuffer + index);
	}

	template <typename T, typename TItemComparator>
	inline const T& Vector<T, TItemComparator>::operator[](size_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range.");
		}

		return *(mBuffer + index);
	}

	template <typename T, typename TItemComparator>
	T& Vector<T, TItemComparator>::At(size_t index)
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range.");
		}

		return *(mBuffer + index);
	}

	template <typename T, typename TItemComparator>
	const T& Vector<T, TItemComparator>::At(size_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range.");
		}

		return *(mBuffer + index);
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::Iterator Vector<T, TItemComparator>::PushBack(const T& data)
	{
		return EmplaceBack(data);
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::Iterator Vector<T, TItemComparator>::PushBack(T&& data)
	{
		return EmplaceBack(std::move(data));
	}

	template <typename T, typename TItemComparator>
	inline std::pair<typename Vector<T, TItemComparator>::Iterator, bool> Vector<T, TItemComparator>::PushBackUnique(const T& data)
	{
		Iterator it = Find(data);
		if (it == end())
		{
			return std::pair(EmplaceBack(data), true);
		}

		return std::pair(it, false);
	}

	template <typename T, typename TItemComparator>
	inline std::pair<typename Vector<T, TItemComparator>::Iterator, bool> Vector<T, TItemComparator>::PushBackUnique(T&& data)
	{
		Iterator it = Find(data);
		if (it == end())
		{
			return std::pair(EmplaceBack(std::move(data)), true);
		}

		return std::pair(it, false);
	}

	template <typename T, typename TItemComparator>
	template <typename... Args>
	typename Vector<T, TItemComparator>::Iterator Vector<T, TItemComparator>::EmplaceBack(Args&&... args)
	{
		if (mSize == mCapacity)
		{
			IncrementCapacity();
		}

		new (mBuffer + mSize)T(std::forward<Args>(args)...);
		++mSize;

		return Iterator(*this, mSize - 1);
	}

	template <typename T, typename TItemComparator>
	void Vector<T, TItemComparator>::PopBack()
	{
		if (mSize == 0)
		{
			throw std::exception("This container is empty.");
		}

		(*(mBuffer + mSize - 1)).~T();
		--mSize;
	}

	template <typename T, typename TItemComparator>
	void Vector<T, TItemComparator>::Reserve(size_t capacity)
	{
		if (capacity > mCapacity)
		{
			mBuffer = reinterpret_cast<T*>(realloc(mBuffer, sizeof(T) * capacity));
			mCapacity = capacity;
		}
	}

	template <typename T, typename TItemComparator>
	void Vector<T, TItemComparator>::Resize(size_t size)
	{
		if (size > mSize)
		{
			if (size > mCapacity)
			{
				Reserve(size);
			}

			for (T* item = mBuffer + mSize; item < mBuffer + size; ++item)
			{
				new (item)T();
			}

			mSize = size;
		}
		else if (size < mSize)
		{
			for (T* item = mBuffer + size; item < mBuffer + mSize; ++item)
			{
				(*item).~T();
			}

			mSize = size;
		}
	}

	template <typename T, typename TItemComparator>
	void Vector<T, TItemComparator>::ShrinkToFit()
	{
		if (mCapacity > mSize)
		{
			mBuffer = reinterpret_cast<T*>(realloc(mBuffer, sizeof(T) * mSize));
			mCapacity = mSize;
		}
	}

	template <typename T, typename TItemComparator>
	void Vector<T, TItemComparator>::Clear()
	{
		for (size_t i = 0; i < mSize; ++i)
		{
			(*(mBuffer + i)).~T();
		}

		mSize = 0;
	}

	template <typename T, typename TItemComparator>
	typename Vector<T, TItemComparator>::Iterator Vector<T, TItemComparator>::Find(const T& data)
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (mItemComparator(*it, data))
			{
				return it;
			}
		}

		return it;
	}

	template <typename T, typename TItemComparator>
	typename Vector<T, TItemComparator>::ConstIterator Vector<T, TItemComparator>::Find(const T& data) const
	{
		ConstIterator it = begin();
		for (; it != end(); ++it)
		{
			if (mItemComparator(*it, data))
			{
				return it;
			}
		}

		return it;
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::Remove(const T& data)
	{
		return Remove(Find(data));
	}

	template <typename T, typename TItemComparator>
	bool Vector<T, TItemComparator>::Remove(const Iterator& it)
	{
		Iterator next = it;
		if (it.mOwner == this)
		{
			++next;
		}
		size_t lastSize = mSize;
		Remove(it, next);

		return lastSize > mSize;
	}

	template <typename T, typename TItemComparator>
	void Vector<T, TItemComparator>::Remove(const Iterator& first, const Iterator& last)
	{
		if (first.mOwner != this || last.mOwner != this)
		{
			throw std::exception("The iterators are not associated with this container.");
		}

		if (first > last)
		{
			throw std::exception("Invalid range.");
		}

		Iterator it = first;
		for (; it != last; ++it)
		{
			(*it).~T();
		}

		if (last != end())
		{
			std::memmove(mBuffer + first.mOffset, mBuffer + last.mOffset, sizeof(T) * (mSize - last.mOffset));
		}

		mSize -= last.mOffset - first.mOffset;
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::Iterator Vector<T, TItemComparator>::begin()
	{
		return Iterator(*this);
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::ConstIterator Vector<T, TItemComparator>::begin() const
	{
		return ConstIterator(*this);
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::Iterator Vector<T, TItemComparator>::end()
	{
		return Iterator(*this, mSize);
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::ConstIterator Vector<T, TItemComparator>::end() const
	{
		return ConstIterator(*this, mSize);
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::ConstIterator Vector<T, TItemComparator>::cbegin() const
	{
		return ConstIterator(*this);
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::ConstIterator Vector<T, TItemComparator>::cend() const
	{
		return ConstIterator(*this, mSize);
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename T, typename TItemComparator>
	inline T& Vector<T, TItemComparator>::Front()
	{
		if (mSize == 0)
		{
			throw std::exception("The container is empty.");
		}

		return *mBuffer;
	}

	template <typename T, typename TItemComparator>
	inline const T& Vector<T, TItemComparator>::Front() const
	{
		if (mSize == 0)
		{
			throw std::exception("The container is empty.");
		}

		return *mBuffer;
	}

	template <typename T, typename TItemComparator>
	inline T& Vector<T, TItemComparator>::Back()
	{
		if (mSize == 0)
		{
			throw std::exception("The container is empty.");
		}

		return *(mBuffer + mSize - 1);
	}

	template <typename T, typename TItemComparator>
	inline const T& Vector<T, TItemComparator>::Back() const
	{
		if (mSize == 0)
		{
			throw std::exception("The container is empty.");
		}

		return *(mBuffer + mSize - 1);
	}

	template <typename T, typename TItemComparator>
	inline size_t Vector<T, TItemComparator>::Size() const
	{
		return mSize;
	}

	template <typename T, typename TItemComparator>
	inline size_t Vector<T, TItemComparator>::Capacity() const
	{
		return mCapacity;
	}

	template <typename T, typename TItemComparator>
	inline void Vector<T, TItemComparator>::SetIncrementCallback(const std::function<size_t(size_t, size_t)>& callback)
	{
		mIncrementCallback = callback;
	}

	template <typename T, typename TItemComparator>
	void Vector<T, TItemComparator>::DeepCopy(const Vector<T, TItemComparator>& other)
	{
		for (const auto& data : other)
		{
			PushBack(data);
		}
	}

	template <typename T, typename TItemComparator>
	void Vector<T, TItemComparator>::IncrementCapacity()
	{
		const size_t newCapacity = mIncrementCallback(mSize, mCapacity);

		if (newCapacity <= mCapacity)
		{
			throw std::exception("New capacity must be at least 1 unit greater than current capacity.");
		}

		Reserve(newCapacity);
	}

	// Iterator
	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::Iterator::Iterator(const Vector<T, TItemComparator>& owner, size_t offset) :
		mOwner(const_cast<Vector*>(&owner)), mOffset(offset)
	{
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::Iterator::operator==(const Iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::Iterator::operator!=(const Iterator& other) const
	{
		return (mOwner != other.mOwner) || (mOffset != other.mOffset);
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::Iterator::operator>(const Iterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("The iterators have different owners.");
		}

		return mOffset > other.mOffset;
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::Iterator::operator<(const Iterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("The iterators have different owners.");
		}

		return mOffset < other.mOffset;
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::Iterator::operator>=(const Iterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("The iterators have different owners.");
		}

		return mOffset >= other.mOffset;
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::Iterator::operator<=(const Iterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("The iterators have different owners.");
		}

		return mOffset <= other.mOffset;
	}

	template <typename T, typename TItemComparator>
	typename Vector<T, TItemComparator>::Iterator& Vector<T, TItemComparator>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mOffset < mOwner->mSize)
		{
			++mOffset;
		}

		return *this;
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::Iterator Vector<T, TItemComparator>::Iterator::operator++(int)
	{
		Vector<T, TItemComparator>::Iterator it = *this;
		operator++();

		return it;
	}

	template <typename T, typename TItemComparator>
	T& Vector<T, TItemComparator>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("This iterator is unassociated.");
		}

		if (mOffset >= mOwner->mSize)
		{
			throw std::exception("This iterator is out of range.");
		}

		return *(mOwner->mBuffer + mOffset);
	}

	template <typename T, typename TItemComparator>
	T* Vector<T, TItemComparator>::Iterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("This iterator is unassociated.");
		}

		if (mOffset >= mOwner->mSize)
		{
			throw std::exception("This iterator is out of range.");
		}

		return &(*(mOwner->mBuffer + mOffset));
	}

	// ConstIterator

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::ConstIterator::ConstIterator(const Iterator& other) :
		mOwner(other.mOwner), mOffset(other.mOffset)
	{
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::ConstIterator::ConstIterator(Iterator&& other) :
		mOwner(other.mOwner), mOffset(other.mOffset)
	{
		other.mOwner = nullptr;
		other.mOffset = 0;
	}

	template <typename T, typename TItemComparator>
	typename Vector<T, TItemComparator>::ConstIterator& Vector<T, TItemComparator>::ConstIterator::operator=(const Iterator& other)
	{
		mOwner = other.mOwner;
		mOffset = other.mOffset;

		return *this;
	}

	template <typename T, typename TItemComparator>
	typename Vector<T, TItemComparator>::ConstIterator& Vector<T, TItemComparator>::ConstIterator::operator=(Iterator&& other)
	{
		mOwner = other.mOwner;
		mOffset = other.mOffset;

		other.mOwner = nullptr;
		other.mOffset = 0;

		return *this;
	}

	template <typename T, typename TItemComparator>
	Vector<T, TItemComparator>::ConstIterator::ConstIterator(const Vector<T, TItemComparator>& owner, size_t offset) :
		mOwner(const_cast<Vector*>(&owner)), mOffset(offset)
	{
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !operator!=(other);
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (mOwner != other.mOwner) || (mOffset != other.mOffset);
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::ConstIterator::operator>(const ConstIterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("The iterators have different owners.");
		}

		return mOffset > other.mOffset;
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::ConstIterator::operator<(const ConstIterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("The iterators have different owners.");
		}

		return mOffset < other.mOffset;
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::ConstIterator::operator>=(const ConstIterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("The iterators have different owners.");
		}

		return mOffset >= other.mOffset;
	}

	template <typename T, typename TItemComparator>
	inline bool Vector<T, TItemComparator>::ConstIterator::operator<=(const ConstIterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("The iterators have different owners.");
		}

		return mOffset <= other.mOffset;
	}

	template <typename T, typename TItemComparator>
	typename Vector<T, TItemComparator>::ConstIterator& Vector<T, TItemComparator>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mOffset < mOwner->mSize)
		{
			++mOffset;
		}

		return *this;
	}

	template <typename T, typename TItemComparator>
	inline typename Vector<T, TItemComparator>::ConstIterator Vector<T, TItemComparator>::ConstIterator::operator++(int)
	{
		Vector<T, TItemComparator>::ConstIterator it = *this;
		operator++();

		return it;
	}

	template <typename T, typename TItemComparator>
	const T& Vector<T, TItemComparator>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("This iterator is unassociated.");
		}

		if (mOffset >= mOwner->mSize)
		{
			throw std::exception("This iterator is out of range.");
		}

		return *(mOwner->mBuffer + mOffset);
	}

	template <typename T, typename TItemComparator>
	const T* Vector<T, TItemComparator>::ConstIterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("This iterator is unassociated.");
		}

		if (mOffset >= mOwner->mSize)
		{
			throw std::exception("This iterator is out of range.");
		}

		return &(*(mOwner->mBuffer + mOffset));
	}
}
