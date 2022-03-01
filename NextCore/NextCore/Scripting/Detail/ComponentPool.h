#pragma once

#include <vector>
#include <unordered_map>
#include <queue>

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
	class ComponentPool
	{
	public:
		using size_type = uint32_t;

		explicit
		ComponentPool(Reflection::GenericFactory const* a_factory, size_type a_initialSize = 1);

		Component*
		AddComponent(EntityId a_entityId);

		Component*
		GetComponent(EntityId a_entityId);

		Component const*
		GetComponent(EntityId a_entityId) const
		{
			return const_cast<ComponentPool*>(this)->GetComponent(a_entityId);
		}

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
		std::vector<std::byte> m_entityStorage;

		std::queue<size_type> m_availableIndexQueue;
		std::unordered_map<EntityId, size_type> m_indexMap;

		// Use raw pointer here because the lifetime of factories is until program end once constructed
		Reflection::GenericFactory const* const m_factory;
		
		const size_type m_elementSize;
		
		size_type m_currentSize;
	};
}
