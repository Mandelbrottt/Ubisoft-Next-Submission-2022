#include "pch.h"

#include "Registry.h"

namespace Next::Detail
{
	EntityId
	Registry::CreateEntity()
	{
		EntityId newEntity = IncrementEntityCounter();

		m_activeEntities.insert(newEntity);

		return newEntity;
	}

	void
	Registry::DestroyEntity(EntityId a_entityId)
	{
		m_activeEntities.erase(a_entityId);
	}

	bool
	Registry::IsValid(EntityId a_entityId) const
	{
		// Is find() or count() faster on average?
		return m_activeEntities.find(a_entityId) != m_activeEntities.end();
		//return m_activeEntities.count(a_entityId) != 0;
	}

	Component*
	Registry::AddComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);

		auto result = componentPool->AddComponent(a_entityId);
		
		return result;
	}

	bool
	Registry::RemoveComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);

		auto result = componentPool->RemoveComponent(a_entityId);
		
		return result;
	}

	Component*
	Registry::GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);

		auto result = componentPool->GetComponent(a_entityId);
		
		return result;
	}

	EntityId
	Registry::IncrementEntityCounter()
	{
		auto result = m_nextEntityId;

		auto rawEntityId = static_cast<entity_id_underlying_t>(m_nextEntityId);

		rawEntityId++;
		m_nextEntityId = static_cast<EntityId>(rawEntityId);

		return result;
	}

	ComponentPool*
	Registry::EnsureComponentPoolInstantiated(Reflection::TypeId a_typeId)
	{
		auto iter = m_componentPools.find(a_typeId);

		if (iter != m_componentPools.end())
		{
			return;
		}

		auto* type = Reflection::Type::TryGet(a_typeId);

		if (!type)
		{
			fprintf(
				stderr,
				"[Next::Registry::EnsureComponentPoolInstantiated] "
				"Type with id [%u] could not be found!",
				static_cast<Reflection::type_id_underlying_t>(a_typeId)
			);
			return nullptr;
		}

		auto pair = std::make_pair(a_typeId, ComponentPool(type->GetFactory()));
		iter = m_componentPools.emplace(std::move(pair)).first;
		return &iter->second;
	}
}
