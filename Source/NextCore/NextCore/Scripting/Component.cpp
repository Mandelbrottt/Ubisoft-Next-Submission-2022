#include "pch.h"
#include "Component.h"

#include "Entity.h"

#include "Components/Transform.h"

#include "Entity.h"

namespace Next
{
	ReflectRegister(Component);
	
	EntityId
	Component::GetEntityId() const
	{
		return m_entityId;
	}

	Entity
	Component::GetEntity() const
	{
		return Entity(m_entityId);
	}

	Transform*
	Component::Transform()
	{
		return GetComponent<Next::Transform>();
	}

	bool
	Component::IsActive() const
	{
		return m_isActive && GetEntity().IsActive();
	}

	void
	Component::SetActive(bool a_active)
	{
		m_isActive = a_active;
	}

	Component*
	Component::AddComponent(Reflection::Type const& a_type)
	{
		return GetEntity().AddComponent(a_type);
	}

	Component*
	Component::AddComponent(Reflection::TypeId a_typeId)
	{
		return GetEntity().AddComponent(a_typeId);
	}
	
	bool
	Component::RemoveComponent(Reflection::Type const& a_type)
	{
		return GetEntity().RemoveComponent(a_type.GetTypeId());
	}

	bool
	Component::RemoveComponent(Reflection::TypeId a_typeId)
	{
		return GetEntity().RemoveComponent(a_typeId);
	}
	
	Component*
	Component::GetComponent(Reflection::Type const& a_type)
	{
		return GetEntity().GetComponent(a_type.GetTypeId());
	}

	Component*
	Component::GetComponent(Reflection::TypeId a_typeId)
	{
		return GetEntity().GetComponent(a_typeId);
	}
}
