#pragma once

#include <unordered_set>

#include "ComponentPool.h"

#include "Reflection/Type.h"
#include "Reflection/TypeId.h"

#include "Scripting/EntityId.h"

namespace Next
{
	class Component;
}

namespace Next::Detail
{
	class Registry
	{
	public:
		EntityId
		OnCreateEntity();

		void
		OnDestroyEntity(EntityId a_entityId);
		
		void
		OnFirstUpdate(EntityId a_entityId, Reflection::TypeId a_typeId);
		
		void
		OnUpdate();

		bool
		IsValid(EntityId a_entityId) const;

		std::unordered_set<EntityId>&
		GetActiveEntities()
		{
			return m_activeEntities;
		}

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		AddComponent(EntityId a_entityId)
		{
			Reflection::TypeId static_id = Reflection::Type::Get<TComponent>().GetTypeId();

			auto* result = AddComponent(a_entityId, static_id);
			return static_cast<TComponent*>(result);
		}

		Component*
		AddComponent(EntityId a_entityId, Reflection::Type const& a_type);

		Component*
		AddComponent(EntityId a_entityId, Reflection::TypeId a_typeId);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent(EntityId a_entityId)
		{
			Reflection::TypeId static_id = Reflection::Type::Get<TComponent>().GetTypeId();

			auto result = RemoveComponent(a_entityId, static_id);
			return result;
		}

		bool
		RemoveComponent(EntityId a_entityId, Reflection::Type const& a_type);

		bool
		RemoveComponent(EntityId a_entityId, Reflection::TypeId a_typeId);
		
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		GetComponent(EntityId a_entityId)
		{
			Reflection::TypeId static_id = Reflection::Type::Get<TComponent>().GetTypeId();

			auto* result = GetComponent(a_entityId, static_id);
			return static_cast<TComponent*>(result);
		}

		Component*
		GetComponent(EntityId a_entityId, Reflection::Type const& a_type);

		Component*
		GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent const*
		GetComponent(EntityId a_entityId) const
		{
			return const_cast<Registry*>(this)->GetComponent<TComponent>(a_entityId);
		}

		Component const*
		GetComponent(EntityId a_entityId, Reflection::Type const& a_type) const;

		Component*
		GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId) const;

	private:
		EntityId
		IncrementEntityCounter();

		ComponentPool*
		EnsureComponentPoolInstantiated(Reflection::TypeId a_typeId);

	private:
		EntityId m_nextEntityId = EntityId::FirstValid;

		std::unordered_set<EntityId> m_activeEntities;

		std::unordered_map<Reflection::TypeId, ComponentPool> m_componentPools;
	};
}
