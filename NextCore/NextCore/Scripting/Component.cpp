#include "pch.h"
#include "Component.h"

#include "Entity.h"

#include "Components/Transform.h"

namespace Next
{
	Component::Component(ComponentConstructionArgs const& a_args)
		: Object(a_args) {}

	EntityId
	Component::GetEntityId() const
	{
		return m_entityId;
	}

	Entity*
	Component::GetEntity() const
	{
		return m_entity;
	}

	Transform*
	Component::Transform()
	{
		return GetComponent<Next::Transform>();
	}
	
	Component*
	Component::AddComponent(Reflection::Type const& a_type)
	{
		return m_entity->AddComponent(a_type);
	}

	Component*
	Component::AddComponent(Reflection::TypeId a_typeId)
	{
		return m_entity->AddComponent(a_typeId);
	}
	
	bool
	Component::RemoveComponent(Reflection::Type const& a_type)
	{
		return m_entity->RemoveComponent(a_type.GetTypeId());
	}

	bool
	Component::RemoveComponent(Reflection::TypeId a_typeId)
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
		return m_entity->GetComponent(a_type.GetTypeId());
	}

	Component*
	Component::GetComponent(Reflection::TypeId a_typeId)
	{
		return m_entity->GetComponent(a_typeId);
	}

	Component**
	Component::GetComponents(Reflection::Type const& a_type, int* a_outCount)
	{
		return m_entity->GetComponents(a_type, a_outCount);
	}

	Component**
	Component::GetComponents(Reflection::TypeId a_typeId, int* a_outCount)
	{
		return m_entity->GetComponents(a_typeId, a_outCount);
	}

	int
	Component::NumComponents(Reflection::Type const& a_type) const
	{
		return m_entity->NumComponents(a_type);
	}

	int
	Component::NumComponents(Reflection::TypeId a_typeId) const
	{
		return m_entity->NumComponents(a_typeId);
	}
}
