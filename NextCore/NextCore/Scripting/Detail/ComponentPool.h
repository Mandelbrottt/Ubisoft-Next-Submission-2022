#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

#include "Reflection/TypeId.h"

#include "Scripting/EntityId.h"

namespace Next
{
	namespace Reflection {
		struct GenericFactory;
	}

	class Component;
}

namespace Next::Detail
{
	using ResizeCallback = void(Reflection::TypeId);
	
	class ComponentPool
	{
	public:
		using size_type = uint32_t;

		explicit
		ComponentPool(Reflection::GenericFactory const* a_factory, size_type a_initialSize = 1);

		template<typename T>
		void
		SetResizeCallbacks(
			T* a_listener, 
			void (T::*a_preResizeCallback)(Reflection::TypeId),
			void (T::*a_postResizeCallback)(Reflection::TypeId)
		)
		{
			m_preResizeCallback  = std::bind(a_preResizeCallback,  a_listener, std::placeholders::_1);
			m_postResizeCallback = std::bind(a_postResizeCallback, a_listener, std::placeholders::_1);
		}
		
		Component*
		AddComponent(EntityId a_entityId);

		Component*
		GetComponent(EntityId a_entityId);

		Component const*
		GetComponent(EntityId a_entityId) const
		{
			return const_cast<ComponentPool*>(this)->GetComponent(a_entityId);
		}

		void
		GetAllComponents(std::vector<Component*>* a_outComponents);

		bool
		RemoveComponent(EntityId a_entityId);

		void
		Resize(size_type a_newSize);
		
	private:
		size_type
		AutoCalculateNewSize() const;
	
		Component*
		GetComponentFromIndex(size_type a_index);
	
		Component const*
		GetComponentFromIndex(size_type a_index) const
		{
			return const_cast<ComponentPool*>(this)->GetComponentFromIndex(a_index);
		}
	
	private:
		// Currently stores components in a packed byte array
		// TODO: Add in boolean byte or word that says if the component is valid or not
		std::vector<std::byte> m_componentStorage;

		std::queue<size_type> m_availableIndexQueue;
		std::unordered_map<EntityId, size_type> m_indexMap;

		// Use raw pointer here because the lifetime of factories is until program-end once constructed
		Reflection::GenericFactory const* const m_factory;
		const Reflection::TypeId m_typeId;
		
		const size_type m_elementSize;
		
		size_type m_count;

		std::function<ResizeCallback> m_preResizeCallback;
		std::function<ResizeCallback> m_postResizeCallback;
	};
}
