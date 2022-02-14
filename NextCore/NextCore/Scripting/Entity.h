#pragma once

#include "Component.h"
#include "EntityId.h"
#include "Object.h"

#include "Reflection/Type.h"

namespace NextCore::Scripting
{
	class Entity : public Object
	{
	public:
		// TODO: Convert to protected when entity system is in place
		Entity()
		{
			m_entityId = static_cast<EntityId>(0);
		}

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		AddComponent();

		Component*
		AddComponent(Reflection::Type const& a_type)
		{
			return AddComponent(a_type.GetStaticId());
		}
		
		Component*
		AddComponent(Reflection::StaticTypeId a_typeId);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent()
		{
			auto static_id = Reflection::GetStaticId<TComponent>();
			return RemoveComponent(static_id);
		}

		bool
		RemoveComponent(Reflection::Type const& a_type)
		{
			return RemoveComponent(a_type.GetStaticId());
		}

		bool
		RemoveComponent(Reflection::StaticTypeId a_typeId);

		bool
		RemoveComponent(Component* a_component);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		GetComponent()
		{
			auto static_id = Reflection::GetStaticId<TComponent>();
			return static_cast<TComponent*>(GetComponent(static_id));
		}

		Component*
		GetComponent(Reflection::Type const& a_type)
		{
			return GetComponent(a_type.GetStaticId());
		}

		Component*
		GetComponent(Reflection::StaticTypeId a_typeId);

	private:
		EntityId m_entityId;

		struct ComponentListElement
		{
			Reflection::StaticTypeId id;

			Component* component;
		};

		std::vector<ComponentListElement> m_components;

	private:
		decltype(m_components)::iterator
		FindComponentById(Reflection::StaticTypeId a_id);

		decltype(m_components)::iterator
		FindComponent(Component* a_component);
	};

	template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool>>
	TComponent*
	Entity::AddComponent()
	{
		auto static_id = Reflection::GetStaticId<TComponent>();

		// TODO: Convert to use a pool allocator
		auto result = new TComponent;

		m_components.push_back({ static_id, result });

		return result;
	}
}
