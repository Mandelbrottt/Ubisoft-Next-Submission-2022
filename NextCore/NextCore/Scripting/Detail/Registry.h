#pragma once

#include <unordered_set>

#include "ComponentPool.h"

#include "Reflection/Type.h"
#include "Reflection/TypeId.h"

#include "Scripting/EntityId.h"
#include "Scripting/Component.h"

namespace Next::Detail
{
	class Registry
	{
	public:
		EntityId
		CreateEntity();

		void
		DestroyEntity(EntityId a_entityId);

		bool
		IsValid(EntityId a_entityId) const;

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		AddComponent(EntityId a_entityId)
		{
			Reflection::TypeId static_id = Reflection::Type::Get<TComponent>().GetTypeId();

			auto* result = AddComponent(a_entityId, static_id);
			return static_cast<TComponent*>(result);
		}

		Component*
		AddComponent(EntityId a_entityId, Reflection::Type const& a_type)
		{
			return AddComponent(a_entityId, a_type.GetTypeId());
		}

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
		RemoveComponent(EntityId a_entityId, Reflection::Type const& a_type)
		{
			return RemoveComponent(a_entityId, a_type.GetTypeId());
		}

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
		GetComponent(EntityId a_entityId, Reflection::Type const& a_type)
		{
			return GetComponent(a_entityId, a_type.GetTypeId());
		}

		Component*
		GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent const*
		GetComponent(EntityId a_entityId) const
		{
			return const_cast<Registry*>(this)->GetComponent<TComponent>(a_entityId);
		}

		Component const*
		GetComponent(EntityId a_entityId, Reflection::Type const& a_type) const
		{
			return const_cast<Registry*>(this)->GetComponent(a_entityId, a_type);
		}

		Component*
		GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId) const
		{
			return const_cast<Registry*>(this)->GetComponent(a_entityId, a_typeId);
		}

	private:
		EntityId
		IncrementEntityCounter();

		ComponentPool*
		EnsureComponentPoolInstantiated(Reflection::TypeId a_typeId);

	private:
		EntityId m_nextEntityId = EntityId::Null;

		std::unordered_set<EntityId> m_activeEntities;

		std::unordered_map<Reflection::TypeId, ComponentPool> m_componentPools;
	};
}
