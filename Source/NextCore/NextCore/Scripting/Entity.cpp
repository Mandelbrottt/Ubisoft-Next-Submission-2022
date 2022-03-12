#include "pch.h"

#include "Entity.h"
#include "Component.h"

#include "Components/Transform.h"

#include "Scripting/Detail/Registry.h"

namespace Next
{
	namespace Detail
	{
		// Expose main registry to users who explicitly want it, namely for tests
		Registry g_mainEntityRegistry;

		void
		SimulateEntityUpdate()
		{
			Entity::Update();
		}

		void
		ResetRegistryAndEntityProperties()
		{
			g_mainEntityRegistry.Reset();

			Entity::s_entityIdDestroyBuffer     = {};
			Entity::s_entityIdFirstUpdateBuffer = {};
		}
	}

	decltype(Entity::s_entityIdFirstUpdateBuffer) Entity::s_entityIdFirstUpdateBuffer;

	decltype(Entity::s_entityIdDestroyBuffer) Entity::s_entityIdDestroyBuffer;

	//decltype(Entity::s_registry) Entity::s_registry;

	bool
	Entity::operator==(Entity const& a_other) const
	{
		return m_entityId == a_other.m_entityId;
	}

	bool
	Entity::operator!=(Entity const& a_other) const
	{
		return !(*this == a_other);
	}

	Entity::operator bool() const
	{
		return IsValid();
	}

	Entity::Entity(EntityId a_id)
	{
		m_entityId = a_id;
	}

	Entity
	Entity::Create(std::string_view const& a_name)
	{
		Entity entity;
		entity.m_entityId = Registry().OnCreateEntity();

		std::string_view newName = a_name;

		if (newName.empty())
		{
			char entityName[128];
			sprintf_s(entityName, "Entity (%d)", entity.m_entityId);
			newName = entityName;
		}

		Registry().SetName(entity.m_entityId, newName);
		
		entity.OnCreate();

		return entity;
	}

	void
	Entity::Destroy(Entity a_entity)
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
	Entity::DestroyImmediate(Entity a_entity)
	{
		s_entityIdDestroyBuffer.erase(a_entity.m_entityId);

		Registry().OnDestroyEntity(a_entity.m_entityId);
		
		// TODO: Find better way to get children, keep track of it in the transform?
		static std::vector<Next::Transform*> transforms;

		// Should probably do this outside of this loop but i dont feel like checking for
		// transforms that have been destroyed
		GetAllComponents<Next::Transform>(transforms);
		for (auto* transform : transforms)
		{
			auto parent = transform->GetParent();
			if (parent && parent->m_entityId == a_entity.m_entityId)
			{
				Registry().OnDestroyEntity(parent->m_entityId);
			}
		}
	}

	void
	Entity::DestroyImmediate()
	{
		DestroyImmediate(*this);
	}

	Entity
	Entity::FindByName(std::string_view const& a_name)
	{
		for (auto entityId : Registry().GetActiveEntities())
		{
			std::string const& name = Registry().GetName(entityId);

			if (a_name == name)
			{
				return Entity(entityId);
			}
		}

		return Entity(EntityId::Null);
	}

	std::vector<Entity>
	Entity::FindAllByName(std::string_view const& a_name)
	{
		std::vector<Entity> result;
		FindAllByName(a_name, result);
		return result;
	}

	void
	Entity::FindAllByName(std::string_view const& a_name, std::vector<Entity>& a_outVector)
	{
		a_outVector.clear();

		for (auto entityId : Registry().GetActiveEntities())
		{
			std::string const& name = Registry().GetName(entityId);

			if (a_name == name)
			{
				a_outVector.push_back(Entity(entityId));
			}
		}
	}

	void
	Entity::OnCreate()
	{
		AddComponent<Next::Transform>();
	}

	bool
	Entity::IsValid() const
	{
		return m_entityId != EntityId::Null && Detail::g_mainEntityRegistry.IsValid(m_entityId);
	}

	Transform*
	Entity::Transform()
	{
		return GetComponent<Next::Transform>();
	}

	std::string const&
	Entity::GetName() const
	{
		return Registry().GetName(m_entityId);
	}

	void
	Entity::SetName(std::string_view const& a_name)
	{
		Registry().SetName(m_entityId, a_name);
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
			
			// TODO: Find better way to get children, keep track of it in the transform?
			static std::vector<Next::Transform*> transforms;

			// Should probably do this outside of this loop but i dont feel like checking for
			// transforms that have been destroyed
			GetAllComponents<Next::Transform>(transforms);
			for (auto* transform : transforms)
			{
				auto parent = transform->GetParent();
				if (parent && parent->GetEntityId() == id)
				{
					Registry().OnDestroyEntity(parent->GetEntityId());
				}
			}
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
		return Detail::g_mainEntityRegistry;
	}
}
