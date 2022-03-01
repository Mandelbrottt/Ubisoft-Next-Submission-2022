#include "pch.h"
#include "Component.h"

#include "Entity.h"

#include "Components/Transform.h"

#include "Entity.h"

namespace Next
{
	ReflectRegister(Component);
	
	Component::Component(ComponentConstructionArgs const& a_args)
		: Object(a_args) {}

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

	bool
	Component::RemoveComponent(Component* a_component)
	{
		return GetEntity().RemoveComponent(a_component);
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
	
	int
	Component::NumComponents(Reflection::Type const& a_type) const
	{
		return GetEntity().NumComponents(a_type);
	}

	int
	Component::NumComponents(Reflection::TypeId a_typeId) const
	{
		return GetEntity().NumComponents(a_typeId);
	}
}
