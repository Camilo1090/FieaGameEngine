/// \file SList.inl
/// \brief Contains the implementation for every method of class SList

#include "SList.h"

namespace FieaGameEngine
{
#pragma region SList
	template <typename T>
	SList<T>::SList(const SList& other)
	{
		DeepCopy(other);
	}

	template <typename T>
	SList<T>::SList(SList&& other) :
		mSize(other.mSize), mFront(other.mFront), mBack(other.mBack)
	{
		other.mSize = 0;
		other.mFront = nullptr;
		other.mBack = nullptr;
	}

	template <typename T>
	SList<T>::SList(std::initializer_list<T> list)
	{
		for (const auto& item : list)
		{
			PushBack(item);
		}
	}

	template <typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <typename T>
	SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			if (mSize > 0)
			{
				Clear();
			}

			DeepCopy(other);
		}

		return *this;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(SList&& other)
	{
		if (this != &other)
		{
			if (mSize > 0)
			{
				Clear();
			}

			mSize = other.mSize;
			mFront = other.mFront;
			mBack = other.mBack;

			other.mSize = 0;
			other.mFront = nullptr;
			other.mBack = nullptr;
		}

		return *this;
	}

	template <typename T>
	SList<T>& SList<T>::operator=(std::initializer_list<T> other)
	{
		if (mSize > 0)
		{
			Clear();
		}

		for (const auto& value : other)
		{
			PushBack(value);
		}

		return *this;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& item)
	{
		return EmplaceFront(item);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushFront(T&& item)
	{
		return EmplaceFront(std::move(item));
	}

	template <typename T>
	template <typename ...Args>
	typename SList<T>::Iterator SList<T>::EmplaceFront(Args&&... args)
	{
		mFront = new Node(mFront, std::forward<Args>(args)...);

		if (mBack == nullptr)
		{
			mBack = mFront;
		}

		++mSize;

		return begin();
	}

	template <typename T>
	void SList<T>::PopFront()
	{
		if (mSize == 0)
		{
			throw std::exception("List is empty.");
		}

		Node* node = mFront->Next;
		delete mFront;
		mFront = node;

		if (mSize == 1)
		{
			mBack = nullptr;
		}

		--mSize;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& item)
	{
		return EmplaceBack(item);
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::PushBack(T&& item)
	{
		return EmplaceBack(std::move(item));
	}

	template <typename T>
	template <typename... Args>
	typename SList<T>::Iterator SList<T>::EmplaceBack(Args&&... args)
	{
		Node* node = new Node(nullptr, std::forward<Args>(args)...);

		if (mSize == 0)
		{
			mFront = node;
		}
		else
		{
			mBack->Next = node;
		}

		mBack = node;
		++mSize;

		return Iterator(*this, mBack);
	}

	template <typename T>
	void SList<T>::PopBack()
	{
		if (mSize == 0)
		{
			throw std::exception("List is empty.");
		}

		if (mSize == 1)
		{
			delete mBack;
			mBack = nullptr;
			mFront = nullptr;
		}
		else
		{
			// Need to find the node before the back
			Node* currentNode = mFront;
			while (currentNode->Next != mBack)
			{
				currentNode = currentNode->Next;
			}

			delete mBack;
			mBack = currentNode;
			currentNode->Next = nullptr;
		}

		--mSize;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& it, const T& data)
	{
		if (this != it.mOwner || it.mNode == nullptr)
		{
			throw std::exception("Invalid iterator.");
		}

		Node* node = new Node(data, it.mNode->Next);
		it.mNode->Next = node;

		if (mBack == it.mNode)
		{
			mBack = node;
		}

		mSize++;

		return Iterator(*this, node);
	}

	template <typename T>
	bool SList<T>::Find(const T& data, Iterator& out)
	{
		for (Iterator it = begin(); it != end(); ++it)
		{
			if (*it == data)
			{
				out = it;
				return true;
			}
		}

		out = end();
		return false;
	}

	template <typename T>
	bool SList<T>::Find(const T& data, ConstIterator& out) const
	{
		for (ConstIterator it = begin(); it != end(); ++it)
		{
			if (*it == data)
			{
				out = it;
				return true;
			}
		}

		out = end();
		return false;
	}

	template <typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& data)
	{
		Iterator it;
		Find(data, it);

		return it;
	}

	template <typename T>
	typename SList<T>::ConstIterator SList<T>::Find(const T& data) const
	{
		ConstIterator it;
		Find(data, it);

		return it;
	}

	template <typename T>
	bool SList<T>::Remove(const T& data)
	{
		Iterator it;
		const bool found = Find(data, it);

		if (found)
		{
			return Remove(it);
		}

		return false;
	}

	template <typename T>
	bool SList<T>::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::exception("Invalid iterator. Owned by another container.");
		}

		bool found = false;
		if (it != end())
		{
			if (it.mNode == mBack)
			{
				PopBack();
			}
			else
			{
				Node* nodeToDelete = it.mNode->Next;
				it.mNode->Data.~T();
				new (&(it.mNode->Data))T(std::move(nodeToDelete->Data));
				it.mNode->Next = nodeToDelete->Next;

				if (nodeToDelete == mBack)
				{
					mBack = it.mNode;
				}

				delete nodeToDelete;
				--mSize;
			}

			found = true;
		}

		return found;
	}

	template <typename T>
	size_t SList<T>::Remove(const Iterator& first, const Iterator& last)
	{
		if (first.mOwner != this || last.mOwner != this)
		{
			throw std::exception("Iterators are not associated with this container.");
		}

		if (last < first)
		{
			throw std::exception("Invalid range.");
		}

		size_t lastSize = mSize;

		if (first != last)
		{
			Iterator it = first;
			++it;
			while (it < last)
			{
				delete (it++).mNode;
				--mSize;
			}

			if (last != end())
			{
				Node* nodeToDelete = last.mNode;
				first.mNode->Data.~T();
				new (&(first.mNode->Data))T(std::move(nodeToDelete->Data));
				first.mNode->Next = nodeToDelete->Next;

				if (nodeToDelete == mBack)
				{
					mBack = first.mNode;
				}

				delete nodeToDelete;
				--mSize;
			}
			else
			{
				if (mSize == 1)
				{
					delete first.mNode;
					--mSize;
					mFront = nullptr;
					mBack = nullptr;
				}
				else
				{
					Node* currentNode = mFront;
					while (currentNode->Next != first.mNode)
					{
						currentNode = currentNode->Next;
					}

					delete first.mNode;
					--mSize;
					currentNode->Next = nullptr;
					mBack = currentNode;
				}
			}
		}

		return lastSize - mSize;
	}

	template <typename T>
	void SList<T>::Clear()
	{
		while (mSize > 0)
		{
			PopFront();
		}
	}

	template <typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <typename T>
	inline T& SList<T>::Front()
	{
		if (mSize == 0)
		{
			throw std::exception("List is empty.");
		}

		return mFront->Data;
	}

	template <typename T>
	inline const T& SList<T>::Front() const
	{
		return const_cast<SList*>(this)->Front();
	}

	template <typename T>
	inline T& SList<T>::Back()
	{
		if (mSize == 0)
		{
			throw std::exception("List is empty.");
		}

		return mBack->Data;
	}

	template <typename T>
	inline const T& SList<T>::Back() const
	{
		return const_cast<SList*>(this)->Back();
	}

	template <typename T>
	inline size_t SList<T>::Size() const
	{
		return mSize;
	}

	template <typename T>
	void SList<T>::DeepCopy(const SList<T>& rhs)
	{
		for (const auto& data : rhs)
		{
			PushBack(data);
		}
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, mFront);
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator(*this, mFront);
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this);
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator(*this);
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(*this, mFront);
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(*this);
	}
#pragma endregion SList
#pragma region Iterator
	template <typename T>
	SList<T>::Iterator::Iterator(const SList<T>& owner, Node* node) :
		mOwner(&owner), mNode(node)
	{
	}

	template <typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template <typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return (mOwner != rhs.mOwner) || (mNode != rhs.mNode);
	}

	template <typename T>
	bool SList<T>::Iterator::operator>(const Iterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("Iterators have different owners.");
		}

		if (mNode == nullptr)
		{
			return true;
		}

		if (other.mNode == nullptr)
		{
			return false;
		}

		Node* node = other.mNode;
		while (node != nullptr)
		{
			if (node == mNode)
			{
				return true;
			}

			node = node->Next;
		}

		return false;
	}

	template <typename T>
	bool SList<T>::Iterator::operator<(const Iterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("Iterators have different owners.");
		}

		if (mNode == nullptr)
		{
			return false;
		}

		if (other.mNode == nullptr)
		{
			return true;
		}

		Node* node = mNode->Next;
		while (node != nullptr)
		{
			if (node == other.mNode)
			{
				return true;
			}

			node = node->Next;
		}

		return false;
	}

	template <typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mNode != nullptr)
		{
			mNode = mNode->Next;
		}

		return *this;
	}

	template <typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();

		return temp;
	}

	template<typename T>
	inline typename T& SList<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mNode == nullptr)
		{
			throw std::exception("Can't dereference out of range iterator.");
		}

		return mNode->Data;
	}

	template<typename T>
	inline typename T* SList<T>::Iterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mNode == nullptr)
		{
			throw std::exception("Can't dereference out of range iterator.");
		}

		return &(mNode->Data);
	}
#pragma endregion Iterator
#pragma region ConstIterator
	template <typename T>
	SList<T>::ConstIterator::ConstIterator(const SList<T>& owner, Node* node) :
		mOwner(&owner), mNode(node)
	{
	}

	template <typename T>
	SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		mOwner(other.mOwner), mNode(other.mNode)
	{
	}

	template <typename T>
	SList<T>::ConstIterator::ConstIterator(Iterator&& other) :
		mOwner(other.mOwner), mNode(other.mNode)
	{
		other.mOwner = nullptr;
		other.mNode = nullptr;
	}

	template <typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator=(const Iterator& other)
	{
		mOwner = other.mOwner;
		mNode = other.mNode;

		return *this;
	}

	template <typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator=(Iterator&& other)
	{
		mOwner = other.mOwner;
		mNode = other.mNode;

		other.mOwner = nullptr;
		other.mNode = nullptr;

		return *this;
	}

	template <typename T>
	inline typename bool SList<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template <typename T>
	inline typename bool SList<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return (mOwner != rhs.mOwner) || (mNode != rhs.mNode);
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator>(const ConstIterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("ConstIterators have different owners.");
		}

		if (mNode == nullptr)
		{
			return true;
		}

		if (other.mNode == nullptr)
		{
			return false;
		}

		Node* node = other.mNode;
		while (node != nullptr)
		{
			if (node == mNode)
			{
				return true;
			}

			node = node->Next;
		}

		return false;
	}

	template <typename T>
	bool SList<T>::ConstIterator::operator<(const ConstIterator& other) const
	{
		if (mOwner != other.mOwner)
		{
			throw std::exception("ConstIterators have different owners.");
		}

		if (mNode == nullptr)
		{
			return false;
		}

		if (other.mNode == nullptr)
		{
			return true;
		}

		Node* node = mNode->Next;
		while (node != nullptr)
		{
			if (node == other.mNode)
			{
				return true;
			}

			node = node->Next;
		}

		return false;
	}

	template <typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mNode != nullptr)
		{
			mNode = mNode->Next;
		}

		return *this;
	}

	template <typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();

		return temp;
	}

	template <typename T>
	typename const T& SList<T>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mNode == nullptr)
		{
			throw std::exception("Can't dereference out of range iterator.");
		}

		return mNode->Data;
	}

	template <typename T>
	typename const T* SList<T>::ConstIterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Unassociated iterator.");
		}

		if (mNode == nullptr)
		{
			throw std::exception("Can't dereference out of range iterator.");
		}

		return &(mNode->Data);
	}
#pragma endregion ConstIterator
#pragma region Node
	template <typename T>
	SList<T>::Node::Node(const T& data, Node* next) :
		Node(next, data)
	{
	}

	template <typename T>
	SList<T>::Node::Node(T&& data, Node* next) :
		Node(next, std::move(data))
	{
	}

	template <typename T>
	template <typename... Args>
	SList<T>::Node::Node(Node* next, Args&&... args) :
		Data(std::forward<Args>(args)...), Next(next)
	{
	}

	template <typename T>
	inline void SList<T>::Node::SetData(const T& data)
	{
		Data = data;
	}
#pragma endregion Node
}