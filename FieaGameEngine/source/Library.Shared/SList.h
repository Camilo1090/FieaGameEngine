#pragma once

/// \file SList.h
/// \brief Contains the definition for class SList

#include <cstddef>
#include <exception>
#include <initializer_list>

/// <summary>
/// Contains all the classes and structures related to the Game Engine library
/// </summary>
namespace FieaGameEngine
{
	/// <summary>
	/// Collection of singly linked elements, where every element has a pointer to the next one
	/// </summary>
	template <typename T>
	class SList final
	{
	private:
		/// <summary>
		/// Represents an element of the list
		/// </summary>
		struct Node final
		{
			/// <summary>
			/// Constructor
			/// Requires the value that this node will contain
			/// </summary>
			/// <param name="data">Required value of the data this node will contain</param>
			/// <param name="next">Optional pointer to the next node in the list</param>
			explicit Node(const T& data, Node* next = nullptr);

			/// <summary>
			/// Constructor
			/// Requires the value that this node will contain
			/// </summary>
			/// <param name="data">Required value of the data this node will contain</param>
			/// <param name="next">Optional pointer to the next node in the list</param>
			explicit Node(T&& data, Node* next = nullptr);

			/// <summary>
			/// Constructor
			/// Requires the value that this node will contain
			/// </summary>
			/// <param name="data">Required value of the data this node will contain</param>
			/// <param name="next">Optional pointer to the next node in the list</param>
			template <typename... Args>
			explicit Node(Node* next, Args&&... args);

			Node(const Node&) = delete;
			Node(Node&&) = delete;
			Node& operator=(const Node&) = delete;
			Node& operator=(Node&&) = delete;

			/// <summary>
			/// Default destructor
			/// </summary>
			~Node() = default;

			/// <summary>
			/// Sets the value that this node contains
			/// </summary>
			/// <param name="data">The new value of the node</param>
			void SetData(const T& data);

			/// <summary>
			/// The actual piece of data the user stores
			/// </summary>
			T Data;

			/// <summary>
			/// Pointer to the next node in the list
			/// </summary>
			Node* Next;
		};

	public:
		/// <summary>
		/// Represents a position inside the list
		/// Can be used to sequentally access nodes in the list
		/// </summary>
		class Iterator final
		{
			/// <summary>
			/// SList needs access to the private constructor to create Iterators
			/// </summary>
			friend SList;

		public:
			/// <summary>
			/// Default constructor, creates an iterator with no owner and no node
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Default copy constructor, no deep copy required
			/// </summary>
			/// <param name="">Reference to the iterator to be copied</param>
			Iterator(const Iterator&) = default;

			/// <summary>
			/// Default move constructor, no deep copy required
			/// </summary>
			/// <param name="">Reference to the iterator to be moved</param>
			Iterator(Iterator&&) = default;

			/// <summary>
			/// Default copy assignment operator, no deep copy required
			/// </summary>
			/// <param name="">Reference to the iterator to be copied</param>
			/// <returns>A reference to this iterator</returns>
			Iterator& operator=(const Iterator&) = default;

			/// <summary>
			/// Default move assignment operator
			/// </summary>
			/// <param name="">Reference to the iterator to be moved</param>
			/// <returns>A reference to this iterator</returns>
			Iterator& operator=(Iterator&&) = default;

			/// <summary>
			/// Default destructor, no need to free any memory
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Checks this iterator for equality against other iterator
			/// </summary>
			/// <param name="rhs">The other iterator to be compared against</param>
			/// <returns>True if both iterators have the same owner AND point to the same element, false otherwise</returns>
			bool operator==(const Iterator& rhs) const;

			/// <summary>
			/// Checks this iterator for inequality against other iterator
			/// </summary>
			/// <param name="rhs">The other iterator to be compared against</param>
			/// <returns>True if the iterators have different owner OR point to different elements, false otherwise</returns>
			bool operator!=(const Iterator& rhs) const;

			/// <summary>
			/// TODO
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			bool operator>(const Iterator& other) const;

			/// <summary>
			/// TODO
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			bool operator<(const Iterator& other) const;

			/// <summary>
			/// Overload for the pre-increment operator
			/// Makes this iterator point to the next element in the list
			/// </summary>
			/// <exception cref="std::exception">Thrwon if this list has no owner</exception>
			/// <returns>A reference to this iterator</returns>
			Iterator& operator++();

			/// <summary>
			/// Overload for the post-increment operator
			/// Makes this iterator point to the next element in the list
			/// </summary>
			/// <exception cref="std::exception">Thrwon if this list has no owner</exception>
			/// <param name="">Dummy param to disambiguate from overloaded methods</param>
			/// <returns>A copy to this iterator made before the increment</returns>
			Iterator operator++(int);

			/// <summary>
			/// Overload for the dereference operator
			/// Gets the element this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if this iterator is out of range</exception>
			/// <returns>A reference to the element this iterator points to</returns>
			T& operator*() const;

			/// <summary>
			/// TODO
			/// </summary>
			/// <returns></returns>
			T* operator->() const;

		private:
			/// <summary>
			/// Creates an iterator with the given list as owner and optionally with the given node as element
			/// </summary>
			/// <param name="owner">Reference to the list that owns this iterator</param>
			/// <param name="node">Pointer to the node this iterator should point to</param>
			explicit Iterator(const SList& owner, Node* node = nullptr);

			/// <summary>
			/// Pointer to the owner list of this iterator
			/// </summary>
			const SList* mOwner = nullptr;

			/// <summary>
			/// Pointer to the node this iterator is currently pointing to
			/// </summary>
			Node* mNode = nullptr;
		};

		/// <summary>
		/// TODO
		/// </summary>
		class ConstIterator final
		{
			/// <summary>
			/// SList needs access to the private constructor to create Iterators
			/// </summary>
			friend class SList;

		public:
			/// <summary>
			/// Default constructor, creates an iterator with no owner and no node
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Default copy constructor, no deep copy required
			/// </summary>
			/// <param name="">Reference to the iterator to be copied</param>
			ConstIterator(const ConstIterator&) = default;

			/// <summary>
			/// TODO
			/// </summary>
			/// <param name="other"></param>
			ConstIterator(const Iterator& other);

			/// <summary>
			/// TODO
			/// </summary>
			/// <param name="other"></param>
			ConstIterator(Iterator&& other);

			/// <summary>
			/// Default copy assignment operator, no deep copy required
			/// </summary>
			/// <param name="">Reference to the iterator to be copied</param>
			/// <returns>A reference to this iterator</returns>
			ConstIterator& operator=(const ConstIterator&) = default;

			/// <summary>
			/// Default assignment operator, no deep copy required
			/// </summary>
			/// <param name="">Reference to the iterator to be copied</param>
			/// <returns>A reference to this iterator</returns>
			ConstIterator& operator=(const Iterator& other);

			/// <summary>
			/// TODO
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			ConstIterator& operator=(Iterator&& other);

			/// <summary>
			/// Default destructor, no need to free any memory
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Checks this iterator for equality against other iterator
			/// </summary>
			/// <param name="rhs">The other iterator to be compared against</param>
			/// <returns>True if both iterators have the same owner AND point to the same element, false otherwise</returns>
			bool operator==(const ConstIterator& rhs) const;

			/// <summary>
			/// Checks this iterator for inequality against other iterator
			/// </summary>
			/// <param name="rhs">The other iterator to be compared against</param>
			/// <returns>True if the iterators have different owner OR point to different elements, false otherwise</returns>
			bool operator!=(const ConstIterator& rhs) const;

			/// <summary>
			/// TODO
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			bool operator>(const ConstIterator& other) const;

			/// <summary>
			/// TODO
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			bool operator<(const ConstIterator& other) const;

			/// <summary>
			/// Overload for the pre-increment operator
			/// Makes this iterator point to the next element in the list
			/// </summary>
			/// <returns>A reference to this iterator</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Overload for the post-increment operator
			/// Makes this iterator point to the next element in the list
			/// </summary>
			/// <param name="">Dummy param to disambiguate from overloaded methods</param>
			/// <returns>A copy to this iterator made before the increment</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Overload for the dereference operator
			/// Gets the element this iterator points to
			/// </summary>
			/// <exception cref="std::exception">Thrown if this iterator is out of range</exception>
			/// <returns>A constant reference to the element this iterator points to</returns>
			const T& operator*() const;

			/// <summary>
			/// TODO
			/// </summary>
			/// <returns></returns>
			const T* operator->() const;

		private:
			/// <summary>
			/// Creates an iterator with the given list as owner and optionally with the given node as element
			/// </summary>
			/// <param name="owner">Reference to the list that owns this iterator</param>
			/// <param name="node">Pointer to the node this iterator should point to</param>
			explicit ConstIterator(const SList& owner, Node* node = nullptr);

			/// <summary>
			/// Pointer to the owner list of this iterator
			/// </summary>
			const SList* mOwner = nullptr;

			/// <summary>
			/// Pointer to the node this iterator is currently pointing to
			/// </summary>
			Node* mNode = nullptr;
		};

		/// <summary>
		/// Constructs a SList with size = 0 and both front and back = nullptr
		/// </summary>
		SList() = default;

		/// <summary>
		/// Copy constructor, deep-copies input list into this list
		/// </summary>
		/// <param name="other">List to be copied</param>
		SList(const SList& other);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">List to be moved</param>
		SList(SList&& other);

		/// <summary>
		/// Creates an SList with the items given by the input std::initializer_list in the same order
		/// </summary>
		/// <param name="list">List with the items to be added</param>
		SList(std::initializer_list<T> list);

		/// <summary>
		/// Destructor, frees all dynamically allocated memory
		/// </summary>
		~SList();

		/// <summary>
		/// Copy assignment operator, deep-copies input list into this list
		/// </summary>
		/// <param name="other">List to be copied</param>
		/// <returns>Reference to this list</returns>
		SList& operator=(const SList& other);

		/// <summary>
		/// Move assignment operator, deep-copies input list into this list
		/// </summary>
		/// <param name="other">List to be copied</param>
		/// <returns>Reference to this list</returns>
		SList& operator=(SList&& other);

		/// <summary>
		/// Assignment operator with initializer list, replaces the contents of this list with the items from the input list
		/// </summary>
		/// <param name="other">List with the items to be added</param>
		/// <returns>A reference to this list</returns>
		SList& operator=(std::initializer_list<T> other);

		/// <summary>
		/// Adds an item to the front of the list, making it the new first element
		/// </summary>
		/// <param name="item">Element to be added</param>
		/// <returns>An iterator that points to the added element</returns>
		Iterator PushFront(const T& item);

		/// <summary>
		/// Adds an item to the front of the list, making it the new first element
		/// </summary>
		/// <param name="item">Element to be added</param>
		/// <returns>An iterator that points to the added element</returns>
		Iterator PushFront(T&& item);

		template <typename... Args>
		Iterator EmplaceFront(Args&&... args);

		/// <summary>
		/// Removes the first item in the list
		/// </summary>
		/// <exception cref="std::exception">Thrown if the list is empty</exception>
		void PopFront();

		/// <summary>
		/// Adds an item to the back of the list, making it the new last element
		/// </summary>
		/// <param name="item">Element to be added</param>
		/// <returns>An iterator that points to the added element</returns>
		Iterator PushBack(const T& item);

		/// <summary>
		/// Adds an item to the back of the list, making it the new last element
		/// </summary>
		/// <param name="item">Element to be added</param>
		/// <returns>An iterator that points to the added element</returns>
		Iterator PushBack(T&& item);

		template <typename... Args>
		Iterator EmplaceBack(Args&&... args);

		/// <summary>
		/// Removes the last element of the list
		/// </summary>
		/// <exception cref="std::exception">Thrown if the list is empty</exception>
		void PopBack();

		/// <summary>
		/// Adds a new element to the list after the element the input iterator points to
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given iterator is from another list or if it's the end iterator</exception>
		/// <param name="it">Reference to the iterator that points to the item that will be before the new element</param>
		/// <param name="data">Reference to the element to be added to the list</param>
		/// <returns>An iterator pointing to the element inserted</returns>
		Iterator InsertAfter(const Iterator& it, const T& data);

		/// <summary>
		/// Looks for a match for the given item (using operator==) sequentially from first element to last
		/// </summary>
		/// <param name="data">Reference to the element that is being looked for</param>
		/// <param name="out">Reference to the iterator instance that shall point to the element if found</param>
		/// <returns>True if the element was found, false otherwise</returns>
		bool Find(const T& data, Iterator& out);

		/// <summary>
		/// Looks for a match for the given item (using operator==) sequentially from first element to last
		/// </summary>
		/// <param name="data">Reference to the element that is being looked for</param>
		/// <param name="out">Reference to the iterator instance that shall point to the element if found</param>
		/// <returns>True if the element was found, false otherwise</returns>
		bool Find(const T& data, ConstIterator& out) const;

		/// <summary>
		/// Looks for a match for the given item (using operator==) sequentially from first element to last
		/// </summary>
		/// <param name="data">Reference to the element that is being looked for</param>
		/// <param name="out">Reference to the iterator instance that shall point to the element if found</param>
		/// <returns>True if the element was found, false otherwise</returns>
		Iterator Find(const T& data);

		/// <summary>
		/// Looks for a match for the given item (using operator==) sequentially from first element to last
		/// </summary>
		/// <param name="data">Reference to the element that is being looked for</param>
		/// <param name="out">Reference to the iterator instance that shall point to the element if found</param>
		/// <returns>True if the element was found, false otherwise</returns>
		ConstIterator Find(const T& data) const;

		/// <summary>
		/// Removes the first ocurrence of the given element from the list
		/// </summary>
		/// <param name="data">Reference to the element to be removed</param>
		/// <returns>True if the element was removed, false otherwise</returns>
		bool Remove(const T& data);

		/// <summary>
		/// Removes the element the given iterator points to
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given iterator is owned by another container</exception>
		/// <param name="it">Reference to the iterator that points to the element to be removed</param>
		/// <returns>True if the element was removed, false otherwise</returns>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes the element the given iterator points to
		/// </summary>
		/// <exception cref="std::exception">Thrown if the given iterator is owned by another container</exception>
		/// <param name="it">Reference to the iterator that points to the element to be removed</param>
		/// <returns>Number of removed elements</returns>
		size_t Remove(const Iterator& first, const Iterator& last);

		/// <summary>
		/// Removes all elements from the list
		/// </summary>
		void Clear();

		/// <summary>
		/// Tells whether or not the list is empty
		/// </summary>
		/// <returns>True if the list is empty, False otherwise</returns>
		bool IsEmpty() const;
		
		/// <summary>
		/// Gets a reference to the first element of the list
		/// </summary>
		/// <exception cref="std::exception">Thrown if the list is empty</exception>
		/// <returns>A reference to the first element of the list</returns>
		T& Front();

		/// <summary>
		/// Gets a constant reference to the first element of the list
		/// </summary>
		/// <exception cref="std::exception">Thrown if the list is empty</exception>
		/// <returns>A constant reference to the first element of the list</returns>
		const T& Front() const;

		/// <summary>
		/// Gets the last element of the list
		/// </summary>
		/// <exception cref="std::exception">Thrown if the list is empty</exception>
		/// <returns>A reference to the last element of the list</returns>
		T& Back();

		/// <summary>
		/// Gets a constant reference to the last element of the list
		/// </summary>
		/// <exception cref="std::exception">Thrown if the list is empty</exception>
		/// <returns>A constant reference to the last element of the list</returns>
		const T& Back() const;

		/// <summary>
		/// Gets the current size of the list
		/// </summary>
		/// <returns>The current size of the list</returns>
		size_t Size() const;

		/// <summary>
		/// Gets an iterator that points to the first element in the list
		/// </summary>
		/// <returns>An iterator that points to the first element in the list</returns>
		Iterator begin();

		/// <summary>
		/// Gets an iterator that points to the first element in the list
		/// </summary>
		/// <returns>A constant iterator that points to the first element in the list</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Gets an iterator that points past the last element in the list
		/// </summary>
		/// <returns>An iterator that points past the last element in the list</returns>
		Iterator end();

		/// <summary>
		/// Gets an iterator that points past the last element in the list
		/// </summary>
		/// <returns>A constant iterator that points past the last element in the list</returns>
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

	private:
		/// <summary>
		/// Pointer to the first element of the list
		/// </summary>
		Node* mFront = nullptr;

		/// <summary>
		/// Pointer to the last element of the list
		/// </summary>
		Node* mBack = nullptr;

		/// <summary>
		/// Number of elements in the collection
		/// </summary>
		size_t mSize = 0;

		/// <summary>
		/// Performs a deep copy of the input list into this list
		/// </summary>
		/// <param name="other">The list to be copied</param>
		void DeepCopy(const SList& other);
	};
}

#include "SList.inl"
