#include "pch.h"
#include "Registry.h"

#include "Scripting/Component.h"

namespace Next::Detail
{
	EntityId
	Registry::OnCreateEntity()
	{
		EntityId newEntity = IncrementEntityCounter();

		m_activeEntities.insert(newEntity);

		return newEntity;
	}

	void
	Registry::OnDestroyEntity(EntityId a_entityId)
	{
		m_activeEntities.erase(a_entityId);

		for (auto& [typeId, pool] : m_componentPools)
		{
			pool.RemoveComponent(a_entityId);
		}
	}

	void
	Registry::OnFirstUpdate(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		auto iter = m_activeEntities.find(a_entityId);

		if (iter == m_activeEntities.end())
		{
			return;
		}

		ComponentPool* pool = EnsureComponentPoolInstantiated(a_typeId);

		if (!pool)
		{
			return;
		}

		pool->GetComponent(a_entityId)->OnFirstUpdate();
	}
	
	void
	Registry::OnUpdate()
	{
		for (auto entityId : m_activeEntities)
		{
			for (auto& [typeId, pool] : m_componentPools)
			{
				auto* component = pool.GetComponent(entityId);

				if (!component)
				{
					continue;
				}
				
				component->OnUpdate();
			}
		}
	}

	bool
	Registry::IsValid(EntityId a_entityId) const
	{
		// Is find() or count() faster on average?
		return m_activeEntities.find(a_entityId) != m_activeEntities.end();
		//return m_activeEntities.count(a_entityId) != 0;
	}

	Component*
	Registry::AddComponent(EntityId a_entityId, Reflection::Type const& a_type)
	{
		return AddComponent(a_entityId, a_type.GetTypeId());
	}

	Component*
	Registry::AddComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);

		if (!componentPool)
		{
			return nullptr;
		}
		
		auto result = componentPool->AddComponent(a_entityId);
		
		return result;
	}

	bool
	Registry::RemoveComponent(EntityId a_entityId, Reflection::Type const& a_type)
	{
		return RemoveComponent(a_entityId, a_type.GetTypeId());
	}

	bool
	Registry::RemoveComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);
		
		if (!componentPool)
		{
			return nullptr;
		}
		
		auto result = componentPool->RemoveComponent(a_entityId);
		
		return result;
	}

	Component*
	Registry::GetComponent(EntityId a_entityId, Reflection::Type const& a_type)
	{
		return GetComponent(a_entityId, a_type.GetTypeId());
	}

	Component*
	Registry::GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId)
	{
		ComponentPool* componentPool = EnsureComponentPoolInstantiated(a_typeId);
		
		if (!componentPool)
		{
			return nullptr;
		}
		
		auto result = componentPool->GetComponent(a_entityId);
		
		return result;
	}

	Component const*
	Registry::GetComponent(EntityId a_entityId, Reflection::Type const& a_type) const
	{
		return const_cast<Registry*>(this)->GetComponent(a_entityId, a_type);
	}

	Component*
	Registry::GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId) const
	{
		return const_cast<Registry*>(this)->GetComponent(a_entityId, a_typeId);
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

		if (iter == m_componentPools.end())
		{
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

			auto componentPool = ComponentPool(type->GetFactory());
			auto pair = std::make_pair(a_typeId, std::move(componentPool));
			iter = m_componentPools.emplace(std::move(pair)).first;
		}
		
		return &iter->second;
	}
}
