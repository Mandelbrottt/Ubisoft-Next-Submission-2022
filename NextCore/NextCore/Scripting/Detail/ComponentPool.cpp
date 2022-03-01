#include "pch.h"

#include "ComponentPool.h"

#include "Reflection/Factory.h"

namespace Next::Detail
{
	using Reflection::GenericFactory;

	ComponentPool::ComponentPool(GenericFactory const* a_factory, size_type a_initialSize)
		: m_factory(a_factory),
		  m_elementSize(a_factory->size),
		  m_currentSize(0)
	{
		assert(a_factory != nullptr);
		
		Resize(a_initialSize);
	}

	Component*
	ComponentPool::AddComponent(EntityId a_entityId)
	{
		if (m_availableIndexQueue.empty())
		{
			Resize(AutoCalculateNewSize());
		}

		size_type index = m_availableIndexQueue.front();
		m_availableIndexQueue.pop();

		Component* componentAddress = std::launder(reinterpret_cast<Component*>(&m_entityStorage[index]));

		auto pair = std::make_pair(a_entityId, index);
		m_indexMap.insert(pair);

		m_factory->Construct(componentAddress);

		return componentAddress;
	}

	Component*
	ComponentPool::GetComponent(EntityId a_entityId)
	{
		auto iter = m_indexMap.find(a_entityId);

		if (iter == m_indexMap.end())
		{
			return nullptr;
		}

		auto index = iter->second;

		Component* component = GetComponentFromIndex(index);

		return component;
	}

	bool
	ComponentPool::RemoveComponent(EntityId a_entityId)
	{
		auto iter = m_indexMap.find(a_entityId);

		if (iter == m_indexMap.end())
		{
			return false;
		}

		auto index = iter->second;

		auto* ptr = GetComponentFromIndex(index);

		m_factory->Destruct(ptr, false);
		
		std::memset(ptr, 0, m_elementSize);

		m_availableIndexQueue.push(index);

		return true;
	}

	void
	ComponentPool::Resize(size_type a_newSize)
	{
		// If the requested size is larger than the current container
		if (m_currentSize < a_newSize)
		{
			m_entityStorage.resize(m_elementSize * a_newSize);

			size_type newAvailableIndex = m_elementSize * m_currentSize;
			
			// Zero out the new elements
			size_type bytesToZero = (a_newSize - m_currentSize) * m_elementSize;
			std::memset(&m_entityStorage[newAvailableIndex], 0, bytesToZero);

			while (newAvailableIndex < m_elementSize * a_newSize)
			{
				m_availableIndexQueue.push(newAvailableIndex);
				newAvailableIndex += m_elementSize;
			}
		} else //If the requested size is smaller than the current container
		{
			// TODO: Implement down-sizing
		}
		
		m_currentSize = a_newSize;
	}

	ComponentPool::size_type
	ComponentPool::AutoCalculateNewSize() const
	{
		return m_currentSize * 2;
	}

	Component*
	ComponentPool::GetComponentFromIndex(size_type a_index)
	{
		size_type offset = a_index;
		Component* reinterpretedPtr = reinterpret_cast<Component*>(&m_entityStorage[offset]);
		return std::launder(reinterpretedPtr);
	}
}
