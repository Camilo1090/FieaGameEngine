#pragma once

#include <glm/fwd.hpp>
#include <cstddef>
#include <cstdint>

namespace FieaGameEngine
{
	class RTTI;
	class Scope;

	/// <summary>
	/// Core class for representing a piece of data in the engine
	/// </summary>
	class Datum final
	{
		/// <summary>
		/// Scope needs to create datums of type table
		/// </summary>
		friend Scope;

		/// <summary>
		/// Attributed needs to set external storage as void*
		/// </summary>
		friend class Attributed;

	public:
		static void CopyValues(const Datum& source, Datum& destination);
		static void MoveValues(const Datum& source, Datum& destination);

	public:
		/// <summary>
		/// All possible flavors of datum
		/// </summary>
		enum class DatumType
		{
			UNKNOWN,
			INTEGER,
			FLOAT,
			VECTOR,
			MATRIX,
			STRING,
			POINTER,
			TABLE,

			FIRST = UNKNOWN,
			LAST = TABLE
		};

		/// <summary>
		/// Sequentally access the values in a datum
		/// </summary>
		class Iterator final
		{
			/// <summary>
			/// Datum needs to construct iterators
			/// </summary>
			friend Datum;

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
			/// <returns>A pointer to the value this iterator points to</returns>
			void* operator*() const;

			template <typename T>
			T& Get() const;

			/// <summary>
			/// Gets the type of the owner
			/// </summary>
			/// <returns>The type of the owner</returns>
			DatumType Type() const;

		private:
			/// <summary>
			/// Creates an iterator with an owner and an offset
			/// </summary>
			/// <param name="owner">The owner of this iterator</param>
			/// <param name="offset">The index of the value this iterator points to</param>
			explicit Iterator(const Datum& owner, size_t offset = 0);

			/// <summary>
			/// The owner of this iterator
			/// </summary>
			const Datum* mOwner = nullptr;

			/// <summary>
			/// The index of the value this iterator points to
			/// </summary>
			size_t mOffset = 0;
		};

		class ConstIterator final
		{
			/// <summary>
			/// Datum needs to construct iterators
			/// </summary>
			friend Datum;

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
			/// Implicit conversion copy constructor
			/// </summary>
			/// <param name="other">Iterator</param>
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
			/// Implicit conversion copy assignment
			/// </summary>
			/// <param name="other">Iterator</param>
			/// <returns>Reference to self</returns>
			ConstIterator& operator=(const Iterator& other);

			/// <summary>
			/// Implicit conversion move assignment
			/// </summary>
			/// <param name="other">Iterator</param>
			/// <returns>Reference to self</returns>
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
			/// <returns>A pointer to the value this iterator points to</returns>
			void* operator*() const;

			template <typename T>
			const T& Get() const;

			/// <summary>
			/// Gets the type of the datum this iterator belongs to
			/// </summary>
			/// <returns>The type of the owner</returns>
			DatumType Type() const;

		private:
			/// <summary>
			/// Creates an iterator with an owner and an offset
			/// </summary>
			/// <param name="owner">The owner of this iterator</param>
			/// <param name="offset">The index of the value this iterator points to</param>
			explicit ConstIterator(const Datum& owner, size_t offset = 0);

			/// <summary>
			/// The owner of this iterator
			/// </summary>
			const Datum* mOwner = nullptr;

			/// <summary>
			/// The index of the value this iterator points to
			/// </summary>
			size_t mOffset = 0;
		};
		
		/// <summary>
		/// Construct a datum with an optional type and starting capacity
		/// </summary>
		/// <param name="type">datum type</param>
		/// <param name="capacity">datum capacity</param>
		explicit Datum(DatumType type = DatumType::UNKNOWN, size_t capacity = 0);

		/// <summary>
		/// copy constructor
		/// </summary>
		/// <param name="other">datum</param>
		Datum(const Datum& other);

		/// <summary>
		/// move constructor
		/// </summary>
		/// <param name="other">datum</param>
		Datum(Datum&& other);
		
		/// <summary>
		/// copy assignment
		/// </summary>
		/// <param name="other">datum</param>
		/// <returns>reference to self</returns>
		Datum& operator=(const Datum& other);

		/// <summary>
		/// move assignment
		/// </summary>
		/// <param name="other">datum</param>
		/// <returns>reference to self</returns>
		Datum& operator=(Datum&& other);

		/// <summary>
		/// default destructor
		/// </summary>
		~Datum();

		/// <summary>
		/// integer scalar constructor
		/// </summary>
		/// <param name="value">integer scalar</param>
		Datum(const std::int32_t& value);

		/// <summary>
		/// float scalar constructor
		/// </summary>
		/// <param name="value">float scalar</param>
		Datum(const float& value);

		/// <summary>
		/// vector scalar constructor
		/// </summary>
		/// <param name="value">vector scalar</param>
		Datum(const glm::vec4& value);

		/// <summary>
		/// matrix scalar constructor
		/// </summary>
		/// <param name="value">matrix scalar</param>
		Datum(const glm::mat4& value);

		/// <summary>
		/// string scalar constructor
		/// </summary>
		/// <param name="value">string scalar</param>
		Datum(const std::string& value);

		/// <summary>
		/// string scalar move constructor
		/// </summary>
		/// <param name="value">string rvalue</param>
		Datum(std::string&& value);

		/// <summary>
		/// RTTI pointer scalar constructor
		/// </summary>
		/// <param name="value">RTTI pointer scalar</param>
		Datum(RTTI* value);

		/// <summary>
		/// integer list constructor
		/// </summary>
		/// <param name="list">integer list</param>
		Datum(std::initializer_list<std::int32_t> list);

		/// <summary>
		/// float list constructor
		/// </summary>
		/// <param name="list">float list</param>
		Datum(std::initializer_list<float> list);

		/// <summary>
		/// vector list constructor
		/// </summary>
		/// <param name="list">vector list</param>
		Datum(std::initializer_list<glm::vec4> list);

		/// <summary>
		/// matrix list constructor
		/// </summary>
		/// <param name="list">matrix list</param>
		Datum(std::initializer_list<glm::mat4> list);

		/// <summary>
		/// string list constructor
		/// </summary>
		/// <param name="list">string list</param>
		Datum(std::initializer_list<std::string> list);

		/// <summary>
		/// RTTI pointer list constructor
		/// </summary>
		/// <param name="list">RTTI pointer list</param>
		Datum(std::initializer_list<RTTI*> list);

		/// <summary>
		/// integer scalar assignment
		/// </summary>
		/// <param name="value">integer scalar</param>
		/// <returns>reference to self</returns>
		Datum& operator=(const std::int32_t& value);

		/// <summary>
		/// float scalar assignment
		/// </summary>
		/// <param name="value">float scalar</param>
		/// <returns>reference to self</returns>
		Datum& operator=(const float& value);

		/// <summary>
		/// vector scalar assignment
		/// </summary>
		/// <param name="value">vector scalar</param>
		/// <returns>reference to self</returns>
		Datum& operator=(const glm::vec4& value);

		/// <summary>
		/// matrix scalar assignment
		/// </summary>
		/// <param name="value">matrix scalar</param>
		/// <returns>reference to self</returns>
		Datum& operator=(const glm::mat4& value);

		/// <summary>
		/// string scalar assignment
		/// </summary>
		/// <param name="value">string scalar</param>
		/// <returns>reference to self</returns>
		Datum& operator=(const std::string& value);

		/// <summary>
		/// string scalar move assignment
		/// </summary>
		/// <param name="value">string rvalue</param>
		/// <returns>reference to self</returns>
		Datum& operator=(std::string&& value);

		/// <summary>
		/// RTTI pointer scalar assignment
		/// </summary>
		/// <param name="value">RTTI pointer scalar</param>
		/// <returns>reference to self</returns>
		Datum& operator=(RTTI* value);

		/// <summary>
		/// integer list assignment
		/// </summary>
		/// <param name="list">integer list</param>
		/// <returns>reference to self</returns>
		Datum& operator=(std::initializer_list<std::int32_t> list);

		/// <summary>
		/// float list assignment
		/// </summary>
		/// <param name="list">float list</param>
		/// <returns>reference to self</returns>
		Datum& operator=(std::initializer_list<float> list);

		/// <summary>
		/// vector list assignment
		/// </summary>
		/// <param name="list">vector list</param>
		/// <returns>reference to self</returns>
		Datum& operator=(std::initializer_list<glm::vec4> list);

		/// <summary>
		/// matrix list assignment
		/// </summary>
		/// <param name="list">matrix list</param>
		/// <returns>reference to self</returns>
		Datum& operator=(std::initializer_list<glm::mat4> list);

		/// <summary>
		/// string list assignment
		/// </summary>
		/// <param name="list">string list</param>
		/// <returns>reference to self</returns>
		Datum& operator=(std::initializer_list<std::string> list);

		/// <summary>
		/// RTTI pointer list assignment
		/// </summary>
		/// <param name="list">RTTI pointer list</param>
		/// <returns>reference to self</returns>
		Datum& operator=(std::initializer_list<RTTI*> list);

		/// <summary>
		/// compares two datums for equality
		/// </summary>
		/// <param name="other">datum</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const Datum& other) const;

		/// <summary>
		/// compares two datums for inequality
		/// </summary>
		/// <param name="other">datum</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const Datum& other) const;

		/// <summary>
		/// integer scalar equality
		/// </summary>
		/// <param name="other">integer</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const std::int32_t& other) const;

		/// <summary>
		/// integer scalar inequality
		/// </summary>
		/// <param name="other">integer</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const std::int32_t& other) const;

		/// <summary>
		/// float scalar equality
		/// </summary>
		/// <param name="other">float</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const float& other) const;

		/// <summary>
		/// float scalar inequality
		/// </summary>
		/// <param name="other">float</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const float& other) const;

		/// <summary>
		/// vector scalar equality
		/// </summary>
		/// <param name="other">vector</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const glm::vec4& other) const;

		/// <summary>
		/// vector scalar inequality
		/// </summary>
		/// <param name="other">vector</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const glm::vec4& other) const;

		/// <summary>
		/// matrix scalar equality
		/// </summary>
		/// <param name="other">matrix</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const glm::mat4& other) const;

		/// <summary>
		/// matrix scalar inequality
		/// </summary>
		/// <param name="other">matrix</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const glm::mat4& other) const;

		/// <summary>
		/// string scalar equality
		/// </summary>
		/// <param name="other">string</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const std::string& other) const;

		/// <summary>
		/// string scalar inequality
		/// </summary>
		/// <param name="other">string</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const std::string& other) const;

		/// <summary>
		/// RTTI pointer scalar equality
		/// </summary>
		/// <param name="other">RTTI pointer</param>
		/// <returns>true if equal, false otherwise</returns>
		bool operator==(const RTTI* other) const;

		/// <summary>
		/// RTTI pointer scalar inequality
		/// </summary>
		/// <param name="other">RTTI pointer</param>
		/// <returns>true if not equal, false otherwise</returns>
		bool operator!=(const RTTI* other) const;

		bool operator==(const Scope& other) const;
		bool operator!=(const Scope& other) const;

		Scope& operator[](size_t index);
		const Scope& operator[](size_t index) const;

		/// <summary>
		/// adds the given value to the end of the list
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		void PushBack(const std::int32_t& value);

		/// <summary>
		/// adds the given value to the end of the list
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		void PushBack(const float& value);

		/// <summary>
		/// adds the given value to the end of the list
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		void PushBack(const glm::vec4& value);

		/// <summary>
		/// adds the given value to the end of the list
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		void PushBack(const glm::mat4& value);

		/// <summary>
		/// adds the given value to the end of the list
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		void PushBack(const std::string& value);

		/// <summary>
		/// adds the given value to the end of the list
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		void PushBack(std::string&& value);

		/// <summary>
		/// adds the given value to the end of the list
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		void PushBack(RTTI* value);

		/// <summary>
		/// sets the element at the given index to the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="value">new value</param>
		/// <param name="index">index of element</param>
		void Set(const std::int32_t& value, size_t index = 0);

		/// <summary>
		/// sets the element at the given index to the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="value">new value</param>
		/// <param name="index">index of element</param>
		void Set(const float& value, size_t index = 0);

		/// <summary>
		/// sets the element at the given index to the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="value">new value</param>
		/// <param name="index">index of element</param>
		void Set(const glm::vec4& value, size_t index = 0);

		/// <summary>
		/// sets the element at the given index to the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="value">new value</param>
		/// <param name="index">index of element</param>
		void Set(const glm::mat4& value, size_t index = 0);

		/// <summary>
		/// sets the element at the given index to the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="value">new value</param>
		/// <param name="index">index of element</param>
		void Set(const std::string& value, size_t index = 0);

		/// <summary>
		/// sets the element at the given index to the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="value">new value</param>
		/// <param name="index">index of element</param>
		void Set(std::string&& value, size_t index = 0);

		/// <summary>
		/// sets the element at the given index to the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="value">new value</param>
		/// <param name="index">index of element</param>
		void Set(RTTI* value, size_t index = 0);

		/// <summary>
		/// sets the element at the given index to the value parsed from the given string
		/// </summary>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="string">string to parse</param>
		/// <param name="index">index of element</param>
		void SetFromString(const std::string& string, size_t index = 0);

		/// <summary>
		/// sets this datum to point to an external buffer of data
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="data">external buffer of data</param>
		/// <param name="size">size of the external buffer</param>
		void SetStorage(std::int32_t* data, size_t size);

		/// <summary>
		/// sets this datum to point to an external buffer of data
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="data">external buffer of data</param>
		/// <param name="size">size of the external buffer</param>
		void SetStorage(float* data, size_t size);

		/// <summary>
		/// sets this datum to point to an external buffer of data
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="data">external buffer of data</param>
		/// <param name="size">size of the external buffer</param>
		void SetStorage(glm::vec4* data, size_t size);

		/// <summary>
		/// sets this datum to point to an external buffer of data
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="data">external buffer of data</param>
		/// <param name="size">size of the external buffer</param>
		void SetStorage(glm::mat4* data, size_t size);

		/// <summary>
		/// sets this datum to point to an external buffer of data
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="data">external buffer of data</param>
		/// <param name="size">size of the external buffer</param>
		void SetStorage(std::string* data, size_t size);

		/// <summary>
		/// sets this datum to point to an external buffer of data
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="data">external buffer of data</param>
		/// <param name="size">size of the external buffer</param>
		void SetStorage(RTTI** data, size_t size);

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		Iterator Find(const std::int32_t& value);

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		Iterator Find(const float& value);

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		Iterator Find(const glm::vec4& value);

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		Iterator Find(const glm::mat4& value);

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		Iterator Find(const std::string& value);

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		Iterator Find(const RTTI* value);

		Iterator Find(const Scope& value);

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		ConstIterator Find(const std::int32_t& value) const;

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		ConstIterator Find(const float& value) const;

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		ConstIterator Find(const glm::vec4& value) const;

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		ConstIterator Find(const glm::mat4& value) const;

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		ConstIterator Find(const std::string& value) const;

		/// <summary>
		/// looks for the given value in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <param name="value">value</param>
		/// <returns>iterator pointing to the value if found, end otherwise</returns>
		ConstIterator Find(const RTTI* value) const;

		ConstIterator Find(const Scope& value) const;

		/// <summary>
		/// removes the first occurrence of the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		/// <returns>true if removed, false otherwise</returns>
		bool Remove(const std::int32_t& value);

		/// <summary>
		/// removes the first occurrence of the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		/// <returns>true if removed, false otherwise</returns>
		bool Remove(const float& value);

		/// <summary>
		/// removes the first occurrence of the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		/// <returns>true if removed, false otherwise</returns>
		bool Remove(const glm::vec4& value);

		/// <summary>
		/// removes the first occurrence of the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		/// <returns>true if removed, false otherwise</returns>
		bool Remove(const glm::mat4& value);

		/// <summary>
		/// removes the first occurrence of the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		/// <returns>true if removed, false otherwise</returns>
		bool Remove(const std::string& value);

		/// <summary>
		/// removes the first occurrence of the given value
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="value">value</param>
		/// <returns>true if removed, false otherwise</returns>
		bool Remove(const RTTI* value);

		/// <summary>
		/// removes the element the given iterator points to
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if iterator is invalid</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="it">iterator</param>
		/// <returns>true if removed, false otherwise</returns>
		bool Remove(const Iterator& it);

		/// <summary>
		/// removes the element at the given index
		/// </summary>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <param name="index">index</param>
		void RemoveAt(size_t index = 0);

		/// <summary>
		/// removes the last element of the datum
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if empty</exception>
		/// </summary>
		void PopBack();

		/// <summary>
		/// resize the datum, default constructing new elements or destructing them
		/// </summary>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="size">new size</param>
		void Resize(size_t size);

		/// <summary>
		/// resize the datum, placing the given value in the new elements or destructing them
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="size">new size</param>
		/// <param name="value">new value(s)</param>
		void Resize(size_t size, const std::int32_t& value);

		/// <summary>
		/// resize the datum, placing the given value in the new elements or destructing them
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="size">new size</param>
		/// <param name="value">new value(s)</param>
		void Resize(size_t size, const float& value);

		/// <summary>
		/// resize the datum, placing the given value in the new elements or destructing them
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="size">new size</param>
		/// <param name="value">new value(s)</param>
		void Resize(size_t size, const glm::vec4& value);

		/// <summary>
		/// resize the datum, placing the given value in the new elements or destructing them
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="size">new size</param>
		/// <param name="value">new value(s)</param>
		void Resize(size_t size, const glm::mat4& value);

		/// <summary>
		/// resize the datum, placing the given value in the new elements or destructing them
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="size">new size</param>
		/// <param name="value">new value(s)</param>
		void Resize(size_t size, const std::string& value);

		/// <summary>
		/// resize the datum, placing the given value in the new elements or destructing them
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="size">new size</param>
		/// <param name="value">new value(s)</param>
		void Resize(size_t size, std::string&& value);

		/// <summary>
		/// resize the datum, placing the given value in the new elements or destructing them
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="size">new size</param>
		/// <param name="value">new value(s)</param>
		void Resize(size_t size, RTTI* value);

		/// <summary>
		/// allocates memory if necessary to have the given capacity
		/// </summary>
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// <param name="capacity">new capacity</param>
		void Reserve(size_t capacity);

		/// <summary>
		/// frees unused capacity
		/// <exception cref="std::exception">thrown if storage is external</exception>
		/// <exception cref="std::exception">thrown if type is not set</exception>
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// destructs all elements
		/// </summary>
		void Clear();

		/// <summary>
		/// gets the elements at the given index
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="index">index</param>
		/// <returns>reference to the value</returns>
		template <typename T>
		T& Get(size_t index = 0);

		/// <summary>
		/// gets the elements at the given index
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="index">index</param>
		/// <returns>reference to the value</returns>
		template <typename T>
		const T& Get(size_t index = 0) const;

		/// <summary>
		/// gets the first element in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <returns>reference to first element</returns>
		template <typename T>
		T& Front();

		/// <summary>
		/// gets the first element in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <returns>reference to first element</returns>
		template <typename T>
		const T& Front() const;

		/// <summary>
		/// gets the last element in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <returns>reference to last element</returns>
		template <typename T>
		T& Back();

		/// <summary>
		/// gets the last element in the datum
		/// </summary>
		/// <exception cref="std::exception">thrown if types differ</exception>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <returns>reference to last element</returns>
		template <typename T>
		const T& Back() const;

		/// <summary>
		/// gets iterator pointing to first element in datum
		/// </summary>
		/// <returns>iterator to first</returns>
		Iterator begin();

		/// <summary>
		/// gets iterator pointing to first element in datum
		/// </summary>
		/// <returns>iterator to first</returns>
		ConstIterator begin() const;

		/// <summary>
		/// gets iterator pointing past the last element in datum
		/// </summary>
		/// <returns>iterator to past the last</returns>
		Iterator end();

		/// <summary>
		/// gets iterator pointing past the last element in datum
		/// </summary>
		/// <returns>iterator to past the last</returns>
		ConstIterator end() const;

		/// <summary>
		/// gets iterator pointing to first element in datum
		/// </summary>
		/// <returns>iterator to first</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// gets iterator pointing past the last element in datum
		/// </summary>
		/// <returns>iterator to past the last</returns>
		ConstIterator cend() const;

		/// <summary>
		/// gets the type of the datum
		/// </summary>
		/// <returns>type of the datum</returns>
		DatumType Type() const;

		/// <summary>
		/// sets the type of the datum
		/// </summary>
		/// <param name="type">new type</param>
		void SetType(DatumType type);

		/// <summary>
		/// gets the size of the datum
		/// </summary>
		/// <returns>size of the datum</returns>
		size_t Size() const;

		/// <summary>
		/// get the capacity of the datum
		/// </summary>
		/// <returns>capacity of the datum</returns>
		size_t Capacity() const;

		/// <summary>
		/// whether or not this datum owns its storage
		/// </summary>
		/// <returns>true if storage is internal, false if external</returns>
		bool OwnsStorage() const;

		/// <summary>
		/// gets a string representation of the element at the given index
		/// </summary>
		/// <exception cref="std::exception">thrown if index is out of range</exception>
		/// <param name="index">index</param>
		/// <returns>string representing element</returns>
		std::string ToString(size_t index = 0) const;

	private:
		/// <summary>
		/// discriminated union of pointers to represent data buffer
		/// </summary>
		union DatumStorage
		{
			std::int32_t* Integer = nullptr;
			float* Float;
			glm::vec4* Vector;
			glm::mat4* Matrix;
			std::string* String;
			RTTI** Pointer;
			Scope** Table;
			void* Wildcard;
		};

		/// <summary>
		/// storage of the datum
		/// </summary>
		DatumStorage mStorage;

		/// <summary>
		/// type of the datum
		/// </summary>
		DatumType mType = DatumType::UNKNOWN;

		/// <summary>
		/// size of the datum
		/// </summary>
		size_t mSize = 0;

		/// <summary>
		/// capacity of the datum
		/// </summary>
		size_t mCapacity = 0;

		/// <summary>
		/// whether or not the datum is internal storage or external
		/// </summary>
		bool mOwnsStorage = true;

		/// <summary>
		/// sets the storage to the given buffer according to the given type
		/// </summary>
		/// <param name="data">external buffer</param>
		/// <param name="size">size of buffer</param>
		/// <param name="type">type of data</param>
		void SetStorage(void* data, size_t size, DatumType type);

		/// <summary>
		/// deep copies given datum into this datum
		/// </summary>
		/// <param name="other">datum</param>
		void DeepCopy(const Datum& other);

		template <typename T>
		void CopyInitializerList(std::initializer_list<T> list);

		/// <summary>
		/// frees allocated memory if internal
		/// </summary>
		void FreeMemory();

		/// <summary>
		/// destructs all elements and frees memory
		/// </summary>
		void Destroy();

		/// <summary>
		/// guards against unset type and type mismatch
		/// </summary>
		/// <param name="type">intended type</param>
		void TypeGuard(DatumType type) const;

		void UnknownTypeGuard() const;

		/// <summary>
		/// guards against out of range indexes
		/// </summary>
		/// <param name="index">intended index</param>
		void IndexGuard(size_t index = 0) const;

		/// <summary>
		/// guards against mutation of external storage
		/// </summary>
		void ExternalStorageGuard() const;

		/// <summary>
		/// push back check for first element being pushed
		/// </summary>
		/// <param name="type">intended type</param>
		void PushBackPreCheck(DatumType type);

		/// <summary>
		/// vector parsed from given string
		/// </summary>
		/// <exception cref="std::exception">thrown if malformed string</exception>
		/// <param name="string">string</param>
		/// <returns>vector</returns>
		glm::vec4 StringToVector(const std::string& string) const;

		/// <summary>
		/// matrix parsed from given string
		/// </summary>
		/// <exception cref="std::exception">thrown if malformed string</exception>
		/// <param name="string">string</param>
		/// <returns>matrix</returns>
		glm::mat4 StringToMatrix(const std::string& string) const;
		
		Datum(Scope& value);
		Datum& operator=(Scope& value);
		void PushBack(Scope& value);
		void Set(Scope& value, size_t index = 0);
		void Resize(size_t size, Scope& value);

		size_t TypeOffset() const;

		static const size_t sTypeCount = static_cast<size_t>(DatumType::LAST) + 1;
		static size_t sTypeSizes[sTypeCount];

		using EqualityFunc = bool(Datum::*)(const Datum&) const;
		static const EqualityFunc sEqualityFuncs[sTypeCount];

		bool ComparePOD(const Datum& other) const;
		bool CompareStrings(const Datum& other) const;
		bool ComparePointers(const Datum& other) const;

		using DeepCopyFunc = void(Datum::*)(const Datum&);
		static const DeepCopyFunc sDeepCopyFuncs[sTypeCount];

		void DeepCopyPOD(const Datum& other);
		void DeepCopyStrings(const Datum& other);
		void CopyAssignStrings(const Datum& other);
		void MoveAssignStrings(const Datum& other);

		using CreateDefaultFunc = void(Datum::*)(size_t);
		static const CreateDefaultFunc sCreateDefaultFuncs[sTypeCount];

		template <typename T>
		void CreateDefault(size_t index);

		using ToStringFunc = std::string(Datum::*)(size_t) const;
		static const ToStringFunc sToStringFuncs[sTypeCount];

		template <typename T>
		std::string ToString(size_t index) const;

		using SetFromStringFunc = void(Datum::*)(const std::string&, size_t);
		static const SetFromStringFunc sSetFromStringFuncs[sTypeCount];

		template <typename T>
		void SetFromString(const std::string& string, size_t index);
	};
}

#include "Datum.inl"
