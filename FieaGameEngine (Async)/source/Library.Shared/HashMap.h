#pragma once

#include <cstddef>
#include <exception>
#include <initializer_list>
#include <tuple>
#include <cmath>
#include "vector.h"
#include "SList.h"
#include "DefaultHash.h"
#include "DefaultComparator.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Map like container, stores key value pairs where the keys are unique
	/// </summary>
	template 
		<typename TKey, 
		typename TValue, 
		typename THashFunctor = DefaultHash<TKey>, 
		typename TKeyComparator = DefaultComparator<TKey>>
	class HashMap final
	{
	public:
		using PairType = std::pair<const TKey, TValue>;

	private:
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using BucketIterator = typename BucketType::Iterator;
		using BucketConstIterator = typename BucketType::ConstIterator;
		using ChainIterator = typename ChainType::Iterator;
		using ChainConstIterator = typename ChainType::ConstIterator;

	public:
		/// <summary>
		/// Allows to sequentally access the elements of this collection
		/// </summary>
		class Iterator final
		{
			/// <summary>
			/// HashMap needs access to Iterator private members
			/// </summary>
			friend HashMap;

		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name="">Iterator</param>
			Iterator(const Iterator&) = default;

			/// <summary>
			/// Default move constructor
			/// </summary>
			/// <param name="">Iterator</param>
			Iterator(Iterator&&) = default;

			/// <summary>
			/// Default copy assignment operator
			/// </summary>
			/// <param name="">Iterator</param>
			/// <returns>A reference to self</returns>
			Iterator& operator=(const Iterator&) = default;

			/// <summary>
			/// Default move assignment operator
			/// </summary>
			/// <param name="">Iterator</param>
			/// <returns>A reference to self</returns>
			Iterator& operator=(Iterator&&) = default;

			/// <summary>
			/// Default constructor
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Compares two iterators for equality
			/// </summary>
			/// <param name="other">The other iterator</param>
			/// <returns>True if equal, false otherwise</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators for inequality
			/// </summary>
			/// <param name="other">The other iterator</param>
			/// <returns>True if not equal, false otherwise</returns>
			bool operator!=(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators for greater than
			/// </summary>
			/// <param name="other">Iterator</param>
			/// <returns>True if this iterator is greater than the other</returns>
			bool operator>(const Iterator& other) const;

			/// <summary>
			/// Compares two iterators for less than
			/// </summary>
			/// <param name="other">Iterator</param>
			/// <returns>True if this iterator is less than the other</returns>
			bool operator<(const Iterator& other) const;

			/// <summary>
			/// Makes self to point to next element in the collection
			/// </summary>
			/// <returns>Reference to self</returns>
			Iterator& operator++();

			/// <summary>
			/// Makes self to point to next element in the collection
			/// </summary>
			/// <param name="">Dummy param</param>
			/// <returns>Reference to self</returns>
			Iterator operator++(int);

			/// <summary>
			/// Extracts the value this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner</exception>
			/// <returns>A reference to the value this iterator points to</returns>
			PairType& operator*() const;

			/// <summary>
			/// Extracts the value this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner</exception>
			/// <returns>A pointer to the value this iterator points to</returns>
			PairType* operator->() const;

		private:
			/// <summary>
			/// Constructs iterator with owner, bucket and chain iterator
			/// </summary>
			/// <param name="owner">Owner for this iterator</param>
			/// <param name="bucket">Bucket for this iterator</param>
			/// <param name="chainIterator">Chain iterator for this iterator</param>
			explicit Iterator(const HashMap& owner, size_t bucket = 0, const ChainIterator& chainIterator = ChainIterator());

			/// <summary>
			/// Owner of this iterator
			/// </summary>
			HashMap* mOwner = nullptr;
			
			/// <summary>
			/// Bucket of this iterator
			/// </summary>
			size_t mBucket = 0;

			/// <summary>
			/// Chain iterator for this iterator
			/// </summary>
			ChainIterator mChainIterator;
		};

		/// <summary>
		/// Allows to sequentally access the elements of this collection
		/// </summary>
		class ConstIterator final
		{
			/// <summary>
			/// HashMap needs access to Iterator private members
			/// </summary>
			friend HashMap;

		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name="">Iterator</param>
			ConstIterator(const ConstIterator&) = default;

			/// <summary>
			/// Default move constructor
			/// </summary>
			/// <param name="">Iterator</param>
			ConstIterator(ConstIterator&&) = default;

			/// <summary>
			/// Copy constructor for implicit conversion from iterator
			/// </summary>
			/// <param name="other">Iterator</param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// Move constructor for implicit conversion from iterator
			/// </summary>
			/// <param name="other">Iterator</param>
			ConstIterator(Iterator&& other);

			/// <summary>
			/// Default copy assignment operator
			/// </summary>
			/// <param name="">Iterator</param>
			/// <returns>A reference to self</returns>
			ConstIterator& operator=(const ConstIterator&) = default;

			/// <summary>
			/// Default move assignment operator
			/// </summary>
			/// <param name="">Iterator</param>
			/// <returns>A reference to self</returns>
			ConstIterator& operator=(ConstIterator&&) = default;

			/// <summary>
			/// Copy assignment for implicit conversion from iterator
			/// </summary>
			/// <param name="other">Iterator</param>
			/// <returns>Reference to self</returns>
			ConstIterator& operator=(const Iterator& other);

			/// <summary>
			/// Move assignment for implicit conversion from iterator
			/// </summary>
			/// <param name="other">Iterator</param>
			/// <returns>Reference to self</returns>
			ConstIterator& operator=(Iterator&& other);

			/// <summary>
			/// Default constructor
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Compares two iterators for equality
			/// </summary>
			/// <param name="other">The other iterator</param>
			/// <returns>True if equal, false otherwise</returns>
			bool operator==(const ConstIterator& other) const;

			/// <summary>
			/// Compares two iterators for inequality
			/// </summary>
			/// <param name="other">The other iterator</param>
			/// <returns>True if not equal, false otherwise</returns>
			bool operator!=(const ConstIterator& other) const;

			/// <summary>
			/// Compares two iterators for greater than
			/// </summary>
			/// <param name="other">Iterator</param>
			/// <returns>True if this iterator is greater than the other</returns>
			bool operator>(const ConstIterator& other) const;

			/// <summary>
			/// Compares two iterators for less than
			/// </summary>
			/// <param name="other">Iterator</param>
			/// <returns>True if this iterator is less than the other</returns>
			bool operator<(const ConstIterator& other) const;

			/// <summary>
			/// Makes self to point to next element in the collection
			/// </summary>
			/// <returns>Reference to self</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Makes self to point to next element in the collection
			/// </summary>
			/// <param name="">Dummy param</param>
			/// <returns>Reference to self</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Extracts the value this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner</exception>
			/// <returns>A reference to the value this iterator points to</returns>
			const PairType& operator*() const;

			/// <summary>
			/// Extracts the value this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if the iterator has no owner</exception>
			/// <returns>A pointer to the value this iterator points to</returns>
			const PairType* operator->() const;

		private:
			/// <summary>
			/// Constructs iterator with owner, bucket and chain iterator
			/// </summary>
			/// <param name="owner">Owner for this iterator</param>
			/// <param name="bucket">Bucket for this iterator</param>
			/// <param name="chainIterator">Chain iterator for this iterator</param>
			explicit ConstIterator(const HashMap& owner, size_t bucket = 0, const ChainConstIterator& chainIterator = ChainConstIterator());

			/// <summary>
			/// Owner of this iterator
			/// </summary>
			HashMap* mOwner = nullptr;

			/// <summary>
			/// Bucket of this iterator
			/// </summary>
			size_t mBucket = 0;

			/// <summary>
			/// Chain iterator for this iterator
			/// </summary>
			ChainConstIterator mChainIterator;
		};

		/// <summary>
		/// Constructs hashmap with the given number of buckets
		/// </summary>
		/// <param name="bucketCount">Number of buckets</param>
		explicit HashMap(size_t bucketCount = 16);

		/// <summary>
		/// Default copy constructor
		/// </summary>
		/// <param name="">Map</param>
		HashMap(const HashMap&) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">Map</param>
		HashMap(HashMap&& other);
		
		/// <summary>
		/// Initializer list constructor
		/// </summary>
		/// <param name="list">Initializer list</param>
		HashMap(std::initializer_list<PairType> list);

		/// <summary>
		/// Default copy assignment
		/// </summary>
		/// <param name="map"></param>
		/// <returns>Reference to self</returns>
		HashMap& operator=(const HashMap&) = default;

		/// <summary>
		/// Move assignment
		/// </summary>
		/// <param name="other">Map</param>
		/// <returns>Reference to self</returns>
		HashMap& operator=(HashMap&& other);

		/// <summary>
		/// Initializer list assignment
		/// </summary>
		/// <param name="list">Initializer list</param>
		/// <returns>Reference to self</returns>
		HashMap& operator=(std::initializer_list<PairType> list);

		/// <summary>
		/// Default destructor
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Gets the value mapped to the given key, if the key is not present, 
		/// it is added and the value is default constructed
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>Reference to the value mapped to key</returns>
		TValue& operator[](const TKey& key);

		/// <summary>
		/// Gets the value mapped to the given key, if the key is not present, 
		/// it is added and the value is default constructed
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>Reference to the value mapped to key</returns>
		TValue& operator[](TKey&& key);

		/// <summary>
		/// Gets a reference to the mapped value for the given key if found
		/// </summary>
		/// <exception cref="std::exception">Thrown if the key is not present</exception>
		/// <param name="key">Key</param>
		/// <returns>A reference to the mapped value for the given key if found</returns>
		TValue& At(const TKey& key);

		/// <summary>
		/// Gets a reference to the mapped value for the given key if found
		/// </summary>
		/// <exception cref="std::exception">Thrown if the key is not present</exception>
		/// <param name="key">Key</param>
		/// <returns>A reference to the mapped value for the given key if found</returns>
		const TValue& At(const TKey& key) const;

		/// <summary>
		/// Adds a key value pair to the collection, if the given key is
		/// already present then nothing is done
		/// </summary>
		/// <param name="pair">Key value pair</param>
		/// <returns>Pair of iterator to the inserted or already present pair and bool true if inserted, false otherwise</returns>
		std::pair<Iterator, bool> Insert(const PairType& pair);

		/// <summary>
		/// Adds a key value pair to the collection, if the given key is
		/// already present then nothing is done
		/// </summary>
		/// <param name="pair">Key value pair</param>
		/// <returns>Pair of iterator to the inserted or already present pair and bool true if inserted, false otherwise</returns>
		std::pair<Iterator, bool> Insert(PairType&& pair);

		/// <summary>
		/// Constructs in place and adds the value to the collection,
		/// if the key already present then nothing is done
		/// </summary>
		/// <param name="...args">Params to be forwarded to pair's constructor</param>
		/// <returns>Pair of iterator to the inserted or already present pair and bool true if inserted, false otherwise</returns>
		template <typename... Args>		
		std::pair<Iterator, bool> Emplace(Args&&... args);

		/// <summary>
		/// Looks for the given key in the collection
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>Iterator to the value if found, end iterator otherwise</returns>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Looks for the given key in the collection
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>Iterator to the value if found, end iterator otherwise</returns>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Removes the pair with the given key
		/// </summary>
		/// <param name="key">Key</param>
		bool Remove(const TKey& key);

		/// <summary>
		/// Removes the element the given iterator points to
		/// </summary>
		/// <param name="it">Iterator</param>
		/// <returns>True if removed, false otherwise</returns>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes the elements in the given range (inclusive, exclusive)
		/// </summary>
		/// <param name="first">First element in range</param>
		/// <param name="last">Element following last removed element</param>
		void Remove(const Iterator& first, const Iterator& last);

		/// <summary>
		/// Sets the number of buckets to the number needed to accomodate at least count elements 
		/// without exceeding maximum load factor and rehashes the container
		/// </summary>
		/// <param name="count">Number of elements</param>
		void Reserve(size_t count);

		/// <summary>
		/// Sets the number of buckets to bucket count and rehashes the container,
		/// puts all current elements in their new appropiate bucket
		/// </summary>
		/// <param name="bucketCount">Number of buckets</param>
		void Rehash(size_t bucketCount);

		/// <summary>
		/// Checks if the given key is present in the collection
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>True if present, false otherwise</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Checks if the given key is present in the collection
		/// </summary>
		/// <param name="key">Key</param>
		/// <param name="it">Iterator to output if found</param>
		/// <returns>True if present, false otherwise</returns>
		bool ContainsKey(const TKey& key, Iterator& out);

		/// <summary>
		/// Checks if the given key is present in the collection
		/// </summary>
		/// <param name="key">Key</param>
		/// <param name="it">Iterator to output if found</param>
		/// <returns>True if present, false otherwise</returns>
		bool ContainsKey(const TKey& key, ConstIterator& out) const;

		/// <summary>
		/// Removes all key value pairs from the container
		/// </summary>
		void Clear();

		/// <summary>
		/// Gets an iterator to the first element in the collection
		/// </summary>
		/// <returns>Iterator to the first element in the collection</returns>
		Iterator begin();

		/// <summary>
		/// Gets an iterator to the first element in the collection
		/// </summary>
		/// <returns>Iterator to the first element in the collection</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an iterator to the last element in the collection
		/// </summary>
		/// <returns>Iterator to the last element in the collection</returns>
		Iterator end();

		/// <summary>
		/// Gets an iterator to the last element in the collection
		/// </summary>
		/// <returns>Iterator to the last element in the collection</returns>
		ConstIterator end() const;

		/// <summary>
		/// Gets an iterator to the first element in the collection
		/// </summary>
		/// <returns>Iterator to the first element in the collection</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Gets an iterator to the last element in the collection
		/// </summary>
		/// <returns>Iterator to the last element in the collection</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Gets the current load factor
		/// </summary>
		/// <returns>The current load factor</returns>
		float LoadFactor() const;

		/// <summary>
		/// Gets the current max load factor
		/// </summary>
		/// <returns>The current max load factor</returns>
		float MaxLoadFactor() const;

		/// <summary>
		/// Sets the max load factor
		/// </summary>
		/// <param name="maxLoadFactor">The new max load factor</param>
		void MaxLoadFactor(float maxLoadFactor);

		/// <summary>
		/// Gets the size of the collection
		/// </summary>
		/// <returns>Size of the collection</returns>
		size_t Size() const;

		/// <summary>
		/// Gets the number of buckets of the collection
		/// </summary>
		/// <returns>The number of buckets of the collection</returns>
		size_t BucketCount() const;

		/// <summary>
		/// Gets the size of the given bucket
		/// </summary>
		/// <param name="bucket">Bucket</param>
		/// <returns>Size of the bucket</returns>
		size_t BucketSize(size_t bucket) const;

	private:
		/// <summary>
		/// Number of elements in the collection
		/// </summary>
		size_t mSize = 0;

		/// <summary>
		/// Buckets of chains of pairs
		/// </summary>
		BucketType mBuckets;

		/// <summary>
		/// Hash functor
		/// </summary>
		THashFunctor mHashFunctor;

		/// <summary>
		/// Key comparator
		/// </summary>
		TKeyComparator mKeyComparator;

		/// <summary>
		/// Max load factor
		/// </summary>
		float mMaxLoadFactor = 1.0f;

		/// <summary>
		/// Assigns a bucket to the given key
		/// </summary>
		/// <param name="key">Key</param>
		/// <returns>Bucket of the key</returns>
		size_t Hash(const TKey& key) const;

		/// <summary>
		/// Compares two keys
		/// </summary>
		/// <param name="first">First key</param>
		/// <param name="second">Second key</param>
		/// <returns>True if the keys are equal, false otherwise</returns>
		bool KeyEqual(const TKey& first, const TKey& second) const;
	};
}

#include "HashMap.inl"
