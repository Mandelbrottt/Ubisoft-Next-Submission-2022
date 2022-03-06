#include "pch.h"

#include "ComponentPool.h"

#include "Reflection/Factory.h"

// Expose default size to users who explicitly want it, namely for tests
int g_componentPoolDefaultSize = 10;

namespace Next::Detail
{
	using Reflection::GenericFactory;

	ComponentPool::ComponentPool(GenericFactory const* a_factory)
		: ComponentPool(a_factory, g_componentPoolDefaultSize) {}

	ComponentPool::ComponentPool(GenericFactory const* a_factory, size_type a_initialSize)
		: m_factory(a_factory),
		  m_typeId(a_factory->GetTypeId()),
		  m_elementSize(a_factory->size),
		  m_count(0)
	{
		assert(a_factory != nullptr);
		
		Resize(a_initialSize);
	}

	ComponentPool::~ComponentPool()
	{
		for (auto const& [entityId, index] : m_indexMap)
		{
			RemoveComponent(entityId);
		}
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

		Component* componentAddress = std::launder(reinterpret_cast<Component*>(&m_componentStorage[index]));

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

	void
	ComponentPool::GetAllComponents(std::vector<Component*>* a_outComponents)
	{
		a_outComponents->clear();
		a_outComponents->reserve(m_count);

		// Check if all of the bytes are zero
		// This is a naive check, but it should work because all components have a non-zero entity id
		auto isZeroedOut = [&](std::byte* a_bytes)->bool
		{
			for (size_type i = 0; i < m_elementSize; i++)
			{
				if (static_cast<uint8_t>(a_bytes[i]) != 0)
				{
					return false;
				}
			}
			return true;
		};

		// find all non-zeroed components
		for (size_type i = 0; i < m_count; i++)
		{
			size_type index = i * m_elementSize;

			std::byte* componentBytes = &m_componentStorage[index];

			if (!isZeroedOut(componentBytes))
			{
				Component* component = std::launder(reinterpret_cast<Component*>(componentBytes));
				a_outComponents->push_back(component);
			}
		}
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

		m_indexMap.erase(iter);

		return true;
	}

	void
	ComponentPool::Resize(size_type a_newSize)
	{
		if (m_preResizeCallback)
			m_preResizeCallback(m_typeId);
		
		// If the requested size is larger than the current container
		if (m_count < a_newSize)
		{
			m_componentStorage.resize(m_elementSize * a_newSize);

			size_type newAvailableIndex = m_elementSize * m_count;
			
			// Zero out the new elements
			size_type bytesToZero = (a_newSize - m_count) * m_elementSize;
			std::memset(&m_componentStorage[newAvailableIndex], 0, bytesToZero);

			while (newAvailableIndex < m_elementSize * a_newSize)
			{
				m_availableIndexQueue.push(newAvailableIndex);
				newAvailableIndex += m_elementSize;
			}
		} else //If the requested size is smaller than the current container
		{
			// TODO: Implement down-sizing
		}
		
		m_count = a_newSize;

		if (m_postResizeCallback)
			m_postResizeCallback(m_typeId);
	}

	ComponentPool::size_type
	ComponentPool::AutoCalculateNewSize() const
	{
		return m_count * 2;
	}

	Component*
	ComponentPool::GetComponentFromIndex(size_type a_index)
	{
		size_type offset = a_index;
		Component* reinterpretedPtr = reinterpret_cast<Component*>(&m_componentStorage[offset]);
		return std::launder(reinterpretedPtr);
	}
}
