#include "Stack.h"

namespace FieaGameEngine
{
	template <typename T>
	template <typename... Args>
	inline void Stack<T>::Emplace(Args&&... args)
	{
		mList.EmplaceFront(std::forward<Args>(args)...);
	}

	template <typename T>
	inline void Stack<T>::Push(const T& value)
	{
		Emplace(value);
	}

	template <typename T>
	inline void Stack<T>::Pop()
	{
		mList.PopFront();
	}

	template <typename T>
	inline void Stack<T>::Clear()
	{
		mList.Clear();
	}

	template <typename T>
	inline T& Stack<T>::Top()
	{
		return mList.Front();
	}

	template <typename T>
	inline const T& Stack<T>::Top() const
	{
		return mList.Front();
	}

	template <typename T>
	inline bool Stack<T>::IsEmpty() const
	{
		return mList.IsEmpty();
	}

	template <typename T>
	inline size_t Stack<T>::Size() const
	{
		return mList.Size();
	}
}
