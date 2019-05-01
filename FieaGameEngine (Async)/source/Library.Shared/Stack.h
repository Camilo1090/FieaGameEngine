#pragma once

#include "SList.h"
#include <cstddef>

namespace FieaGameEngine
{
	template <typename T>
	class Stack final
	{
	public:
		Stack() = default;
		Stack(const Stack&) = default;
		Stack(Stack&&) = default;
		Stack& operator=(const Stack&) = default;
		Stack& operator=(Stack&&) = default;
		~Stack() = default;

		template <typename... Args>
		void Emplace(Args&&... args);

		void Push(const T& value);
		void Pop();
		void Clear();

		T& Top();
		const T& Top() const;

		bool IsEmpty() const;
		size_t Size() const;

	private:
		SList<T> mList;
	};
}

#include "Stack.inl"
