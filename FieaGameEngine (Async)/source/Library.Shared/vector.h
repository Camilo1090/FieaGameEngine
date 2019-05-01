#pragma once

/// \file vector.h
/// \brief Definition of Vector class

#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <exception>
#include <functional>
#include <initializer_list>
#include "DefaultComparator.h"
#include "Utility.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Dynamic container of contiguously stored elements
	/// </summary>
	template <typename T, typename TItemComparator = DefaultComparator<T>>
	class Vector final
	{
	public:
		using value_type = T;
		void push_back(const value_type& value) { EmplaceBack(value); }
		void push_back(value_type&& value) { EmplaceBack(std::move(value)); }

	public:
		/// <summary>
		/// Represents a position in the vector
		/// Can be used to sequentally access elements in the vector
		/// </summary>
		class Iterator final
		{
			/// <summary>
			/// Vector needs to create iterators using its private constructor
			/// </summary>
			friend Vector;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

		public:
			/// <summary>
			/// Creates an iterator with no owner and a meaningless offset
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Shallow copies the given iterator into this iterator
			/// </summary>
			/// <param name="">The iterator to be copied</param>
			Iterator(const Iterator&) = default;

			/// <summary>
			/// Default move constructor
			/// </summary>
			/// <param name="">Iterator</param>
			Iterator(Iterator&&) = default;

			/// <summary>
			/// Shallow copies the given iterator into this iterator
			/// </summary>
			/// <param name="">The iterator to be copied</param>
			/// <returns>A reference to this iterator</returns>
			Iterator& operator=(const Iterator&) = default;

			/// <summary>
			/// Default move assignment
			/// </summary>
			/// <param name="">Iterator</param>
			/// <returns>Reference to self</returns>
			Iterator& operator=(Iterator&&) = default;

			/// <summary>
			/// Destructs the iterator
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Compares two iterators for equality
			/// </summary>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if both iterators have the same owner and offset, false otherwise</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Compares two iterator for inequality
			/// </summary>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if the iterators have different owners or offsets, false otherwise</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterators have different owners</exception>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if this iterator has an offset greater than the other, false otherwise</returns>
			bool operator>(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterators have different owners</exception>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if this iterator has an offset less than the other, false otherwise</returns>
			bool operator<(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterators have different owners</exception>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if this iterator has an offset greater or equal to the other, false otherwise</returns>
			bool operator>=(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterators have different owners</exception>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if this iterator has an offset less or equal to the other, false otherwise</returns>
			bool operator<=(const Iterator& other) const;

			/// <summary>
			/// Makes this iterator point to the next element in the vector
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner</exception>
			/// <returns>A reference to this iterator</returns>
			Iterator& operator++();

			/// <summary>
			/// Makes this iterator point to the next element in the vector
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner</exception>
			/// <param name="">Dummy parameter to differentiate functions</param>
			/// <returns>A copy to this iterator before the increment</returns>
			Iterator operator++(int);

			/// <summary>
			/// Gets the value this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner or is out of range</exception>
			/// <returns>A reference to the value this iterator points to</returns>
			T& operator*() const;

			/// <summary>
			/// Gets the value this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner or is out of range</exception>
			/// <returns>A pointer to the value this iterator points to</returns>
			T* operator->() const;

		private:
			/// <summary>
			/// Creates an iterator with an owner and an offset
			/// </summary>
			/// <param name="owner">The owner of this iterator</param>
			/// <param name="offset">The index of the value this iterator points to</param>
			explicit Iterator(const Vector& owner, size_t offset = 0);

			/// <summary>
			/// The owner of this iterator
			/// </summary>
			Vector* mOwner = nullptr;

			/// <summary>
			/// The index of the value this iterator points to
			/// </summary>
			size_t mOffset = 0;
		};

		class ConstIterator final
		{
			/// <summary>
			/// Vector needs to create iterators using its private constructor
			/// </summary>
			friend class Vector;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			using iterator_category = std::forward_iterator_tag;

		public:
			/// <summary>
			/// Creates an iterator with no owner and a meaningless offset
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Shallow copies the given iterator into this iterator
			/// </summary>
			/// <param name="">The iterator to be copied</param>
			ConstIterator(const ConstIterator&) = default;

			/// <summary>
			/// Creates a copy of the given iterator as a const iterator
			/// </summary>
			/// <param name="other">The iterator to be copied</param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// Implicit conversion move constructor
			/// </summary>
			/// <param name="other">Iterator</param>
			ConstIterator(Iterator&& other);

			/// <summary>
			/// Shallow copies the given iterator into this iterator
			/// </summary>
			/// <param name="">The iterator to be copied</param>
			/// <returns>A reference to this iterator</returns>
			ConstIterator& operator=(const ConstIterator&) = default;

			/// <summary>
			/// Shallow copies the given iterator into this iterator
			/// </summary>
			/// <param name="">The iterator to be copied</param>
			/// <returns>A reference to this iterator</returns>
			ConstIterator& operator=(const Iterator& other);

			/// <summary>
			/// Implicit conversion move assignment from iterator
			/// </summary>
			/// <param name="other">iterator</param>
			/// <returns>reference to self</returns>
			ConstIterator& operator=(Iterator&& other);

			/// <summary>
			/// Destructs the iterator
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Compares two iterators for equality
			/// </summary>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if both iterators have the same owner and offset, false otherwise</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Compares two iterator for inequality
			/// </summary>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if the iterators have different owners or offsets, false otherwise</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Compares two iterators
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterators have different owners</exception>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if this iterator has an offset greater than the other, false otherwise</returns>
			bool operator>(const ConstIterator& other) const;

			/// <summary>
			/// Compares two iterators
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterators have different owners</exception>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if this iterator has an offset less than the other, false otherwise</returns>
			bool operator<(const ConstIterator& other) const;

			/// <summary>
			/// Compares two iterators
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterators have different owners</exception>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if this iterator has an offset greater or equal to the other, false otherwise</returns>
			bool operator>=(const ConstIterator& other) const;

			/// <summary>
			/// Compares two iterators
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterators have different owners</exception>
			/// <param name="other">The other iterator to compare against</param>
			/// <returns>True if this iterator has an offset less or equal to the other, false otherwise</returns>
			bool operator<=(const ConstIterator& other) const;

			/// <summary>
			/// Makes this iterator point to the next element in the vector
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner</exception>
			/// <returns>A reference to this iterator</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Makes this iterator point to the next element in the vector
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner</exception>
			/// <param name="">Dummy parameter to differentiate functions</param>
			/// <returns>A copy to this iterator before the increment</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Gets the value this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner or is out of range</exception>
			/// <returns>A constant reference to the value this iterator points to</returns>
			const T& operator*() const;

			/// <summary>
			/// Gets the value this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner or is out of range</exception>
			/// <returns>A pointer to the value this iterator points to</returns>
			const T* operator->() const;

		private:
			/// <summary>
			/// Creates an iterator with an owner and an offset
			/// </summary>
			/// <param name="owner">The owner of this iterator</param>
			/// <param name="offset">The index of the value this iterator points to</param>
			explicit ConstIterator(const Vector& owner, size_t offset = 0);

			/// <summary>
			/// The owner of this iterator
			/// </summary>
			Vector* mOwner = nullptr;

			/// <summary>
			/// The index of the value this iterator points to
			/// </summary>
			size_t mOffset = 0;
		};

	public:
		/// <summary>
		/// Creates a vector with the given capacity and increment strategy
		/// </summary>
		/// <param name="capacity">The new capacity</param>
		/// <param name="incrementCallback">The increment strategy to use for this vector</param>
		explicit Vector(size_t capacity = 0, std::function<size_t(size_t, size_t)> incrementCallback = DefaultIncrement());

		/// <summary>
		/// Deep copies the given vector into this
		/// </summary>
		/// <param name="other">The vector to be copied</param>
		Vector(const Vector& other);

		/// <summary>
		/// Moves the given vector into this
		/// </summary>
		/// <param name="other">The vector to be moved</param>
		Vector(Vector&& other);

		/// <summary>
		/// Creates a vector with the contents of the given list
		/// </summary>
		/// <param name="list">The list with the elements to copy</param>
		Vector(std::initializer_list<T> list);

		/// <summary>
		/// Destructs the vector
		/// </summary>
		~Vector();

		/// <summary>
		/// Copies the given vector into this
		/// </summary>
		/// <param name="other">The vector to be copied</param>
		/// <returns>A reference to this vector</returns>
		Vector& operator=(const Vector& other);

		/// <summary>
		/// Moves the given vector into this
		/// </summary>
		/// <param name="other">The vector to be moved</param>
		/// <returns>A reference to this vector</returns>
		Vector& operator=(Vector&& other);

		/// <summary>
		/// Copies the elements of the given list into this
		/// </summary>
		/// <param name="list">The list with the elements</param>
		/// <returns>A reference to this iterator</returns>
		Vector& operator=(std::initializer_list<T> list);

		bool operator==(const Vector& other);
		bool operator!=(const Vector& other);

		/// <summary>
		/// Gets the value at the given index
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given index is out of range</exception>
		/// <param name="index">The index of the value to get</param>
		/// <returns>A reference to the value</returns>
		T& operator[](size_t index);

		/// <summary>
		/// Gets the value at the given index
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given index is out of range</exception>
		/// <param name="index">The index of the value to get</param>
		/// <returns>A constant reference to the value</returns>
		const T& operator[](size_t index) const;

		/// <summary>
		/// Gets the value at the given index
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given index is out of range</exception>
		/// <param name="index">The index of the value to get</param>
		/// <returns>A reference to the value</returns>
		T& At(size_t index);

		/// <summary>
		/// Gets the value at the given index
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given index is out of range</exception>
		/// <param name="index">The index of the value to get</param>
		/// <returns>A constant reference to the value</returns>
		const T& At(size_t index) const;

		/// <summary>
		/// Appends the given element to the end of the vector
		/// </summary>
		/// <exception cref="std::exception">Thrown if the increment strategy fails to provide a valid new capacity (strictly greater than current capacity)</exception>
		/// <param name="data">The element to be added</param>
		/// <returns>Iterator to the added element</returns>
		Iterator PushBack(const T& data);

		/// <summary>
		/// Appends the given element to the end of the vector
		/// </summary>
		/// <exception cref="std::exception">Thrown if the increment strategy fails to provide a valid new capacity (strictly greater than current capacity)</exception>
		/// <param name="data">The element to be added</param>
		/// <returns>Iterator to the added element</returns>
		Iterator PushBack(T&& data);

		std::pair<Iterator, bool> PushBackUnique(const T& data);
		std::pair<Iterator, bool> PushBackUnique(T&& data);

		/// <summary>
		/// Adds an element to the end of the list
		/// </summary>
		/// <param name="...args">arguments for the constructor of the stored type</param>
		/// <returns>Iterator pointing to the added element</returns>
		template <typename... Args>		
		Iterator EmplaceBack(Args&&... args);

		/// <summary>
		/// Removes the last element of the vector
		/// <exception cref="std::exception">Thrown if the vector is empty</exception>
		/// </summary>
		void PopBack();

		/// <summary>
		/// Increases the capacity of the vector to match the given capacity.
		/// Only increases capacity, given capacity less than or equal
		/// to the current capacity is a no-op
		/// </summary>
		/// <param name="capacity">The new capacity of the vector</param>
		void Reserve(size_t capacity);

		/// <summary>
		/// TODO
		/// </summary>
		/// <param name="size">TODO</param>
		void Resize(size_t size);

		/// <summary>
		/// Releases excess capacity
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Destructs all the elements in the vector
		/// </summary>
		void Clear();

		/// <summary>
		/// Looks for the given value in the vector
		/// </summary>
		/// <param name="data">The value to look for</param>
		/// <returns>An iterator pointing to the element if found, end otherwise</returns>
		Iterator Find(const T& data);

		/// <summary>
		/// Looks for the given value in the vector
		/// </summary>
		/// <param name="data">The value to look for</param>
		/// <returns>A constiterator pointing to the element if found, end otherwise</returns>
		ConstIterator Find(const T& data) const;

		/// <summary>
		/// Removes the first occurrence of the given element
		/// </summary>
		/// <param name="it">The element to be removed</param>
		bool Remove(const T& data);

		/// <summary>
		/// Removes the element the given iterator points to
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given iterator is not owned by this vector</exception>
		/// <param name="it">The iterator pointing to the element to be removed</param>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes the range of elements the given iterator range points to
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given iterators are not owned by this vector or if the range is invalid</exception>
		/// <param name="first">The iterator pointing to the first element to be removed</param>
		/// <param name="last">The iterator pointing to the element following the last one to remove</param>
		void Remove(const Iterator& first, const Iterator& last);

		/// <summary>
		/// Gets an iterator to the first element in the vector
		/// </summary>
		/// <returns>An iterator to the first element in the vector</returns>
		Iterator begin();

		/// <summary>
		/// Gets an iterator to the first element in the vector
		/// </summary>
		/// <returns>An iterator to the first element in the vector</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an iterator pointing past the last element in the vector
		/// </summary>
		/// <returns>An iterator pointing past the last element in the vector</returns>
		Iterator end();

		/// <summary>
		/// Gets an iterator pointing past the last element in the vector
		/// </summary>
		/// <returns>An iterator pointing past the last element in the vector</returns>
		ConstIterator end() const;

		/// <summary>
		/// Gets an iterator to the first element in the vector
		/// </summary>
		/// <returns>An iterator to the first element in the vector</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an iterator pointing past the last element in the vector
		/// </summary>
		/// <returns>An iterator pointing past the last element in the vector</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Gets whether or not this vector is empty
		/// </summary>
		/// <returns>True is the vector is empty, false otherwise</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Gets the first value in the vector
		/// </summary>
		/// <exception cref="std::exception">Thrown if the vector is empty</exception>
		/// <returns>A reference to the first value in the vector</returns>
		T& Front();

		/// <summary>
		/// Gets the first value in the vector
		/// </summary>
		/// <exception cref="std::exception">Thrown if the vector is empty</exception>
		/// <returns>A constant reference to the first value in the vector</returns>
		const T& Front() const;

		/// <summary>
		/// Gets the last value in the vector
		/// </summary>
		/// <exception cref="std::exception">Thrown if the vector is empty</exception>
		/// <returns>A reference to the last value in the vector</returns>
		T& Back();

		/// <summary>
		/// Gets the last value in the vector
		/// </summary>
		/// <exception cref="std::exception">Thrown if the vector is empty</exception>
		/// <returns>A constant reference to the last value in the vector</returns>
		const T& Back() const;

		/// <summary>
		/// Gets the current size of the vector
		/// </summary>
		/// <returns>The current size of the vector</returns>
		size_t Size() const;

		/// <summary>
		/// Gets the current capacity of the vector
		/// </summary>
		/// <returns>The current capacity of the vector</returns>
		size_t Capacity() const;

		/// <summary>
		/// Sets the capacity increment strategy callback for this vector
		/// </summary>
		/// <param name="callback">The new strategy to be used in this vector</param>
		void SetIncrementCallback(const std::function<size_t(size_t, size_t)>& callback);

	private:
		/// <summary>
		/// The current number of elements in the vector
		/// </summary>
		size_t mSize = 0;

		/// <summary>
		/// The current total number of elements this vector can hold
		/// </summary>
		size_t mCapacity = 0;

		/// <summary>
		/// Pointer to the buffer of elements
		/// </summary>
		T* mBuffer = nullptr;

		/// <summary>
		/// The current capacity increment strategy
		/// </summary>
		std::function<size_t(size_t, size_t)> mIncrementCallback = std::function(DefaultIncrement());

		TItemComparator mItemComparator;

		/// <summary>
		/// Deep copies the given vector into this
		/// </summary>
		/// <param name="other">Reference to vector to be copied</param>
		void DeepCopy(const Vector& other);

		/// <summary>
		/// TODO
		/// </summary>
		void IncrementCapacity();
	};
}

#include "vector.inl"
