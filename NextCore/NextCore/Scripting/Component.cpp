#include "pch.h"
#include "Component.h"

#include "Entity.h"

namespace NextCore::Scripting
{
	EntityId
	Component::GetEntityId() const
	{
		return entityId;
	}

	Entity*
	Component::GetEntity() const
	{
		return m_entity;
	}
	
	void
	Component::SetEntity(Entity* a_entity)
	{
		m_entity = a_entity;
	}

	Component*
	Component::AddComponent(Reflection::Type const& a_type)
	{
		return m_entity->AddComponent(a_type);
	}

	Component*
	Component::AddComponent(Reflection::StaticTypeId a_typeId)
	{
		return m_entity->AddComponent(a_typeId);
	}
	
	bool
	Component::RemoveComponent(Reflection::Type const& a_type)
	{
		return m_entity->RemoveComponent(a_type.GetStaticId());
	}

	bool
	Component::RemoveComponent(Reflection::StaticTypeId a_typeId)
	{
		return m_entity->RemoveComponent(a_typeId);
	}

	bool
	Component::RemoveComponent(Component* a_component)
	{
		return m_entity->RemoveComponent(a_component);
	}
	
	Component*
	Component::GetComponent(Reflection::Type const& a_type)
	{
		return m_entity->GetComponent(a_type.GetStaticId());
	}

	Component*
	Component::GetComponent(Reflection::StaticTypeId a_typeId)
	{
		return m_entity->GetComponent(a_typeId);
	}
}
