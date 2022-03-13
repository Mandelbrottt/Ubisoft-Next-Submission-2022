#pragma once

#include <functional>

// This whole thing is bust because NextCore offers no guarantee that listeners will stay at the same location in memory
// This should be reimplemented once functions are added to reflection

namespace Next
{	
	//template<typename... TArgs>
	//class ActionCallback
	//{
	//public:
	//	template<typename TClass>
	//	void
	//	AddListener(TClass const* a_listener, void (TClass::*a_function)(TArgs ...));

	//	void
	//	Invoke()
	//	{
	//		for (auto& callback : m_callbacks)
	//		{
	//			callback();
	//		}
	//	}
	//
	//private:
	//	std::vector<std::function<void(TArgs ...)>> m_callbacks;
	//};
	//
	//template<typename... TArgs>
	//template<typename TClass>
	//void
	//ActionCallback<TArgs...>::AddListener(TClass const* a_listener, void (TClass::*a_function)(TArgs ...))
	//{
	//	static_assert(sizeof...(TArgs) <= 2);

	//	namespace ph = std::placeholders;

	//	std::function<void(TArgs ...)> callback;
	//	switch (sizeof...(TArgs))
	//	{
	//		case 0:
	//			callback = std::move(std::bind(a_function, a_listener));
	//			break;
	//		case 1:
	//			callback = std::move(std::bind(a_function, a_listener, ph::_1));
	//			break;
	//		case 2:
	//			callback = std::move(std::bind(a_function, a_listener, ph::_1, ph::_2));
	//			break;
	//		default:
	//			throw "Shouldn't get here";
	//	}

	//	m_callbacks.push_back(std::move(callback));
	//}

	//void
	//Foo()
	//{
	//	ActionCallback<int> callback;
	//	callback.AddListener();
	//	callback.RemoveListener();
	//	callback += listener;
	//	callback -= listener;
	//	callback.Invoke();
	//}

	//class Something
	//{
	//public:
	//	void
	//	Foo() {}
	//};

	//void
	//Bar()
	//{
	//	auto s   = Something();
	//	auto foo = Something::Foo;
	//}
}
