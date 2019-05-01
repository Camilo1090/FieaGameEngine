#include "HashMap.h"

namespace FieaGameEngine
{
	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>::HashMap(size_t bucketCount) :
		mBuckets(bucketCount)
	{
		mBuckets.Resize(bucketCount);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>::HashMap(HashMap&& other) :
		mSize(other.mSize), mBuckets(std::move(other.mBuckets)), mHashFunctor(std::move(other.mHashFunctor)), mKeyComparator(std::move(other.mKeyComparator))
	{
		other.mSize = 0;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>::HashMap(std::initializer_list<PairType> list) :
		HashMap(list.size() * 2)
	{
		for (auto& pair : list)
		{
			Emplace(std::move(pair));
		}
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::operator=(HashMap&& other)
	{
		if (this != &other)
		{
			if (mSize > 0)
			{
				Clear();
			}

			mSize = other.mSize;
			mBuckets = std::move(other.mBuckets);
			mHashFunctor = std::move(other.mHashFunctor);
			mKeyComparator = std::move(other.mKeyComparator);

			other.mSize = 0;
		}

		return *this;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::operator=(std::initializer_list<PairType> list)
	{
		if (mSize > 0)
		{
			Clear();
		}

		for (auto& pair : list)
		{
			Emplace(std::move(pair));
		}

		return *this;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	TValue& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::operator[](const TKey& key)
	{
		auto[it, inserted] = Emplace(key, TValue());

		return it->second;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	TValue& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::operator[](TKey&& key)
	{
		auto[it, inserted] = Emplace(std::move(key), TValue());

		return it->second;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	TValue& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::At(const TKey& key)
	{
		Iterator it = Find(key);

		if (it == end())
		{
			throw std::exception("Key not found.");
		}

		return it->second;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	const TValue& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::At(const TKey& key) const
	{
		ConstIterator it = Find(key);

		if (it == end())
		{
			throw std::exception("Key not found.");
		}

		return it->second;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	std::pair<typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator, bool> HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Insert(const PairType& pair)
	{
		return Emplace(pair);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	std::pair<typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator, bool> HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Insert(PairType&& pair)
	{
		return Emplace(std::move(pair));
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	template <typename... Args>
	std::pair<typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator, bool> HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Emplace(Args&&... args)
	{
		PairType pair(std::forward<Args>(args)...);
		Iterator it = Find(pair.first);

		bool inserted = false;
		if (it == end())
		{
			const size_t bucket = Hash(pair.first);
			ChainIterator chainIterator = mBuckets[bucket].PushBack(std::move(pair));
			++mSize;
			it = Iterator(*this, bucket, chainIterator);
			inserted = true;
		}

		return std::pair(it, inserted);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Find(const TKey& key)
	{
		const size_t bucket = Hash(key);

		for (ChainIterator it = mBuckets[bucket].begin(); it != mBuckets[bucket].end(); ++it)
		{
			if (KeyEqual(key, it->first))
			{
				return Iterator(*this, bucket, it);
			}
		}

		return end();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Find(const TKey& key) const
	{
		const size_t bucket = Hash(key);

		for (ChainConstIterator it = mBuckets[bucket].begin(); it != mBuckets[bucket].end(); ++it)
		{
			if (KeyEqual(key, it->first))
			{
				return ConstIterator(*this, bucket, it);
			}
		}

		return end();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Remove(const TKey& key)
	{
		Iterator it = Find(key);
		return Remove(it);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Remove(const Iterator& it)
	{
		if (it.mOwner == this && it != end())
		{
			mBuckets[it.mBucket].Remove(it.mChainIterator);
			--mSize;
			return true;
		}

		return false;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	void HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Remove(const Iterator& first, const Iterator& last)
	{
		if (first.mOwner != this || last.mOwner != this)
		{
			throw std::exception("Iterators are not associated with this container.");
		}

		if (last < first)
		{
			throw std::exception("Invalid range.");
		}

		size_t bucket = first.mBucket;
		ChainIterator chainIt = first.mChainIterator;
		while (bucket < last.mBucket)
		{
			mSize -= mBuckets[bucket].Remove(chainIt, mBuckets[bucket].end());

			++bucket;
			if (bucket < BucketCount())
			{
				chainIt = mBuckets[bucket].begin();
			}
		}

		if (bucket < BucketCount())
		{
			mSize -= mBuckets[bucket].Remove(chainIt, last.mChainIterator);
		}
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	void HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Reserve(size_t count)
	{
		Rehash(static_cast<size_t>(std::ceil(count / mMaxLoadFactor)));
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	void HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Rehash(size_t bucketCount)
	{
		BucketType newTable;
		newTable.Resize(bucketCount);

		for (auto& pair : *this)
		{
			const size_t bucket = mHashFunctor(pair.first) % bucketCount;
			newTable[bucket].EmplaceBack(std::move(pair));
		}

		mBuckets = std::move(newTable);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ContainsKey(const TKey& key) const
	{
		return Find(key) != end();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ContainsKey(const TKey& key, Iterator& out)
	{
		out = Find(key);

		return out != end();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ContainsKey(const TKey& key, ConstIterator& out) const
	{
		out = Find(key);

		return out != end();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	void HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Clear()
	{
		for (auto& chain : mBuckets)
		{
			chain.Clear();
		}

		mSize = 0;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::begin()
	{
		for (size_t i = 0; i < mBuckets.Size(); ++i)
		{
			if (!mBuckets[i].IsEmpty())
			{
				return Iterator(*this, i, mBuckets[i].begin());
			}
		}

		return end();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::begin() const
	{
		for (size_t i = 0; i < mBuckets.Size(); ++i)
		{
			if (!mBuckets[i].IsEmpty())
			{
				return ConstIterator(*this, i, mBuckets[i].begin());
			}
		}

		return end();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::end()
	{
		return Iterator(*this, BucketCount());
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::end() const
	{
		return ConstIterator(*this, BucketCount());
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::cbegin() const
	{
		for (size_t i = 0; i < mBuckets.Size(); ++i)
		{
			if (!mBuckets[i].IsEmpty())
			{
				return ConstIterator(*this, i, mBuckets[i].begin());
			}
		}

		return ConstIterator(*this, BucketCount());
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::cend() const
	{
		return ConstIterator(*this, BucketCount());
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline float FieaGameEngine::HashMap<TKey, TValue, THashFunctor, TKeyComparator>::LoadFactor() const
	{
		return static_cast<float>(mSize) / BucketCount();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline float HashMap<TKey, TValue, THashFunctor, TKeyComparator>::MaxLoadFactor() const
	{
		return mMaxLoadFactor;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline void HashMap<TKey, TValue, THashFunctor, TKeyComparator>::MaxLoadFactor(float maxLoadFactor)
	{
		mMaxLoadFactor = maxLoadFactor;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline size_t HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Size() const
	{
		return mSize;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline size_t FieaGameEngine::HashMap<TKey, TValue, THashFunctor, TKeyComparator>::BucketCount() const
	{
		return mBuckets.Size();
	}

	template<typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline size_t HashMap<TKey, TValue, THashFunctor, TKeyComparator>::BucketSize(size_t bucket) const
	{
		return mBuckets[bucket].Size();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline size_t HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Hash(const TKey& key) const
	{
		return mHashFunctor(key) % BucketCount();
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::KeyEqual(const TKey& first, const TKey& second) const
	{
		return mKeyComparator(first, second);
	}

	// Iterator

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::Iterator(const HashMap& owner, size_t bucket, const ChainIterator& chainIterator) :
		mOwner(const_cast<HashMap*>(&owner)), mBucket(bucket), mChainIterator(chainIterator)
	{
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::operator==(const Iterator& other) const
	{
		return !operator!=(other);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::operator!=(const Iterator& other) const
	{
		return (mOwner != other.mOwner) || (mBucket != other.mBucket) || (mChainIterator != other.mChainIterator);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::operator>(const Iterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("Iterators have different owners.");
		}

		return (mBucket > other.mBucket) || (mBucket == other.mBucket && mChainIterator > other.mChainIterator);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::operator<(const Iterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("Iterators have different owners.");
		}

		return (mBucket < other.mBucket) || (mBucket == other.mBucket && mChainIterator < other.mChainIterator);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		++mChainIterator;
		if (mChainIterator == mOwner->mBuckets[mBucket].end())
		{
			if (mBucket < mOwner->BucketCount())
			{
				++mBucket;
				for (; mBucket < mOwner->BucketCount(); ++mBucket)
				{
					if (!mOwner->mBuckets[mBucket].IsEmpty())
					{
						mChainIterator = mOwner->mBuckets[mBucket].begin();
						return *this;
					}
				}

				if (mBucket == mOwner->BucketCount())
				{
					mChainIterator = ChainIterator();
				}
			}
		}

		return *this;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();

		return temp;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::PairType& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		return *mChainIterator;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::PairType* HashMap<TKey, TValue, THashFunctor, TKeyComparator>::Iterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		return &(*mChainIterator);
	}

	// ConstIterator

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::ConstIterator(const HashMap& owner, size_t bucket, const ChainConstIterator& chainConstIterator) :
		mOwner(const_cast<HashMap*>(&owner)), mBucket(bucket), mChainIterator(chainConstIterator)
	{
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::ConstIterator(const Iterator& other) :
		mOwner(other.mOwner), mBucket(other.mBucket), mChainIterator(other.mChainIterator)
	{
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::ConstIterator(Iterator&& other) :
		mOwner(other.mOwner), mBucket(other.mBucket), mChainIterator(std::move(other.mChainIterator))
	{
		other.mOwner = nullptr;
		other.mBucket = 0;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator=(const Iterator& other)
	{
		mOwner = other.mOwner;
		mBucket = other.mBucket;
		mChainIterator = other.mChainIterator;

		other.mOwner = nullptr;
		other.mBucket = 0;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator=(Iterator&& other)
	{
		mOwner = other.mOwner;
		mBucket = other.mBucket;
		mChainIterator = std::move(other.mChainIterator);

		other.mOwner = nullptr;
		other.mBucket = 0;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator==(const ConstIterator& other) const
	{
		return !operator!=(other);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	inline bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator!=(const ConstIterator& other) const
	{
		return (mOwner != other.mOwner) || (mBucket != other.mBucket) || (mChainIterator != other.mChainIterator);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator>(const ConstIterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("Iterators have different owners.");
		}

		return (mBucket > other.mBucket) || (mBucket == other.mBucket && mChainIterator > other.mChainIterator);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	bool HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator<(const ConstIterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("Iterators have different owners.");
		}

		return (mBucket < other.mBucket) || (mBucket == other.mBucket && mChainIterator < other.mChainIterator);
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		++mChainIterator;
		if (mChainIterator == mOwner->mBuckets[mBucket].end())
		{
			if (mBucket < mOwner->BucketCount())
			{
				++mBucket;
				for (; mBucket < mOwner->BucketCount(); ++mBucket)
				{
					if (!mOwner->mBuckets[mBucket].IsEmpty())
					{
						mChainIterator = mOwner->mBuckets[mBucket].begin();
						return *this;
					}
				}

				if (mBucket == mOwner->BucketCount())
				{
					mChainIterator = ChainConstIterator();
				}
			}
		}

		return *this;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();

		return temp;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	const typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::PairType& HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		return *mChainIterator;
	}

	template <typename TKey, typename TValue, typename THashFunctor, typename TKeyComparator>
	const typename HashMap<TKey, TValue, THashFunctor, TKeyComparator>::PairType* HashMap<TKey, TValue, THashFunctor, TKeyComparator>::ConstIterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		return &(*mChainIterator);
	}
}
