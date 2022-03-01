#include "pch.h"

#include "Entity.h"
#include "Component.h"

#include "Components/Transform.h"

#include "Scripting/Detail/Registry.h"

namespace Next
{
	decltype(Entity::s_entityIdFirstUpdateBuffer) Entity::s_entityIdFirstUpdateBuffer;

	decltype(Entity::s_entityIdDestroyBuffer) Entity::s_entityIdDestroyBuffer;
	
	//decltype(Entity::s_registry) Entity::s_registry;

	static Detail::Registry g_registry;
	
	Entity::Entity(EntityId a_id)
	{
		m_entityId = a_id;
	}

	Entity
	Entity::Create()
	{
		Entity entity;
		entity.m_entityId = Registry().OnCreateEntity();
		
		entity.OnCreate();

		return entity;
	}

	void
	Entity::Destroy(Entity const& a_entity)
	{
		auto iter = s_entityIdDestroyBuffer.find(a_entity.m_entityId);

		if (iter == s_entityIdDestroyBuffer.end())
		{
			s_entityIdDestroyBuffer.insert(a_entity.m_entityId);
		}
	}

	void
	Entity::Destroy()
	{
		Destroy(*this);
	}

	void
	Entity::OnCreate()
	{
		AddComponent<Next::Transform>();
	}

	Transform*
	Entity::Transform()
	{
		return GetComponent<Next::Transform>();
	}

	Component*
	Entity::AddComponent(Reflection::TypeId a_typeId)
	{
		if (a_typeId == Reflection::TypeId::Null)
		{
			return nullptr;
		}
		
		Component* result = OnAddComponent(m_entityId, a_typeId);

		return result;
	}

	bool
	Entity::RemoveComponent(Reflection::TypeId a_typeId)
	{
		if (a_typeId == Reflection::TypeId::Null)
		{
			return false;
		}

		return OnRemoveComponent(m_entityId, a_typeId);
	}
	
	Component*
	Entity::GetComponent(Reflection::TypeId a_typeId)
	{
		if (a_typeId == Reflection::TypeId::Null)
		{
			return nullptr;
		}
		
		Component* result = Registry().GetComponent(m_entityId, a_typeId);
		
		return result;
	}
	
	void
	Entity::Update()
	{
		if (s_entityIdDestroyBuffer.empty())
		{
			goto First_Update_Active_Entities;
		}

		for (auto id : s_entityIdDestroyBuffer)
		{
			Registry().OnDestroyEntity(id);
		}
		
		s_entityIdDestroyBuffer.clear();

	First_Update_Active_Entities:

		if (s_entityIdFirstUpdateBuffer.empty())
		{
			goto Update_Active_Entities;
		}

		for (auto& [entityId, typeId] : s_entityIdFirstUpdateBuffer)
		{
			Registry().OnFirstUpdate(entityId, typeId);
		}

		s_entityIdFirstUpdateBuffer.clear();

	Update_Active_Entities:
		
		Registry().OnUpdate();
	}
	
	Component*
	Entity::OnAddComponent(EntityId a_id, Reflection::TypeId a_typeId)
	{
		Component* result = Registry().AddComponent(a_id, a_typeId);

		s_entityIdFirstUpdateBuffer.emplace_back(std::make_pair(a_id, a_typeId));

		result->m_entityId = a_id;

		result->OnCreate();

		return result;
	}

	bool
	Entity::OnRemoveComponent(EntityId a_id, Reflection::TypeId a_typeId)
	{
		Component* component = Registry().GetComponent(a_id, a_typeId);
		
		component->OnDestroy();

		return Registry().RemoveComponent(a_id, a_typeId);
	}

	bool
	Entity::RemoveComponent(identity<Next::Transform>)
	{
		auto static_id = Reflection::GetTypeId<Next::Transform>();
		return RemoveComponent(static_id);
	}

	Detail::Registry&
	Entity::Registry()
	{
		return g_registry;
	}
}
