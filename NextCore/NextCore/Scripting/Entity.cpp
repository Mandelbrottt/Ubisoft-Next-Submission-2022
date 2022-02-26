#include "pch.h"

#include "Entity.h"
#include "Component.h"

#include "Components/Transform.h"

namespace Next
{
	//decltype(Entity::s_entityIds) Entity::s_entityIds;

	decltype(Entity::s_entityIdDestroyBuffer) Entity::s_entityIdDestroyBuffer;

	decltype(Entity::s_entityRepresentations) Entity::s_entityRepresentations;

	static EntityId g_nextEntityCounter = EntityId::FirstValid;

	static EntityId
	GetNextEntityId()
	{
		auto result = g_nextEntityCounter;

		auto& counterReference = reinterpret_cast<entity_id_underlying_t&>(g_nextEntityCounter);
		counterReference++;

		return result;
	}

	Entity::Entity()
	{
		m_entityId = GetNextEntityId();

		// TODO: Will eventually be implemented by the registry when a new entity is created
		OnCreate();
	}

	Entity
	Entity::Create()
	{
		Entity entity;

		s_entityRepresentations.emplace(entity.m_entityId, Detail::EntityRepresentation {});

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

		auto* componentType = Reflection::Type::TryGet(a_typeId);

		if (componentType == nullptr)
		{
			return nullptr;
		}

		auto*      constructor = componentType->GetConstructor();
		void*      p           = constructor->Construct();
		Component* result      = static_cast<Component*>(p);

		auto* rep = GetCurrentEntityRepresentation();

		if (!rep)
		{
			return nullptr;
		}

		Detail::ComponentListElement element = { a_typeId, result };
		OnAddComponent(m_entityId, rep, element);

		return result;
	}

	bool
	Entity::RemoveComponent(Reflection::TypeId a_typeId)
	{
		if (a_typeId == Reflection::TypeId::Null)
		{
			return false;
		}

		auto* rep = GetCurrentEntityRepresentation();

		if (!rep)
		{
			return false;
		}

		auto iter = FindComponentById(rep, a_typeId);

		return RemoveComponentByIterator(rep, iter);
	}

	bool
	Entity::RemoveComponent(Component* a_component)
	{
		if (a_component == nullptr)
		{
			return false;
		}

		auto* rep = GetCurrentEntityRepresentation();

		if (!rep)
		{
			return false;
		}

		auto iter = FindComponent(rep, a_component);

		return RemoveComponentByIterator(rep, iter);
	}

	Component*
	Entity::GetComponent(Reflection::TypeId a_typeId)
	{
		if (a_typeId == Reflection::TypeId::Null)
		{
			return nullptr;
		}

		auto* rep = GetCurrentEntityRepresentation();

		Component* result = nullptr;

		if (auto iter = FindComponentById(rep, a_typeId); rep && iter != rep->components.end())
		{
			result = iter->component;
		}

		return result;
	}

	Component**
	Entity::GetComponents(Reflection::TypeId a_typeId, int* a_outCount)
	{
		if (a_typeId == Reflection::TypeId::Null || !a_outCount)
		{
			return nullptr;
		}

		auto* rep = GetCurrentEntityRepresentation();

		if (!rep)
		{
			return nullptr;
		}

		*a_outCount = NumComponents(a_typeId);

		Component** components = new Component*[*a_outCount];

		int count = 0;
		for (const auto& [id, component] : rep->components)
		{
			if (id == a_typeId)
			{
				components[count] = component;
				count++;
			}
		}

		return components;
	}

	int
	Entity::NumComponents(Reflection::TypeId a_typeId) const
	{
		auto* rep = GetCurrentEntityRepresentation();

		if (!rep)
		{
			return 0;
		}

		auto predicate = [&](Detail::ComponentListElement const& a_value)
		{
			return a_value.id == a_typeId;
		};

		auto numComponents = std::count_if(rep->components.begin(), rep->components.end(), predicate);

		return static_cast<int>(numComponents);
	}

	void
	Entity::Update()
	{
		if (s_entityIdDestroyBuffer.empty())
		{
			goto Update_Active_Entities;
		}

		for (auto id : s_entityIdDestroyBuffer)
		{
			auto& rep = s_entityRepresentations.at(id);
			OnDestroy(rep);
		}

		for (auto id : s_entityIdDestroyBuffer)
		{
			s_entityRepresentations.erase(id);
		}

		s_entityIdDestroyBuffer.clear();

	Update_Active_Entities:
		for (auto& [id, rep] : s_entityRepresentations)
		{
			OnUpdate(rep);
		}
	}

	Detail::EntityRepresentation*
	Entity::GetCurrentEntityRepresentation()
	{
		auto iter = s_entityRepresentations.find(m_entityId);

		Detail::EntityRepresentation* result = nullptr;

		if (iter != s_entityRepresentations.end())
		{
			result = &iter->second;
		}

		return result;
	}

	Detail::EntityRepresentation const*
	Entity::GetCurrentEntityRepresentation() const
	{
		return const_cast<Entity*>(this)->GetCurrentEntityRepresentation();
	}

	decltype(Detail::EntityRepresentation::components)::iterator
	Entity::FindComponentById(Detail::EntityRepresentation* a_rep, Reflection::TypeId a_id)
	{
		auto iter = a_rep->components.begin();
		auto end  = a_rep->components.end();

		while (iter != end)
		{
			if (iter->id == a_id)
			{
				break;
			}

			++iter;
		}

		return iter;
	}

	decltype(Detail::EntityRepresentation::components)::iterator
	Entity::FindComponent(Detail::EntityRepresentation* a_rep, Component* a_component)
	{
		auto iter = a_rep->components.begin();
		auto end  = a_rep->components.end();

		while (iter != end)
		{
			if (iter->component == a_component)
			{
				break;
			}

			++iter;
		}

		return iter;
	}

	bool
	Entity::RemoveComponentByIterator(
		Detail::EntityRepresentation*                                a_rep,
		decltype(Detail::EntityRepresentation::components)::iterator a_iter
	)
	{
		bool result = a_iter != a_rep->components.end();

		// If component exists and it's type is registered with reflection, destruct it
		if (result)
		{
			auto                         [id, component] = *a_iter;
			Detail::ComponentListElement element { id, component };
			OnRemoveComponent(a_rep, element);

			if (auto* componentType = Reflection::Type::TryGet(id); componentType != nullptr)
			{
				auto* constructor = componentType->GetConstructor();
				constructor->Destruct(component);
			}
		}

		return result;
	}

	void
	Entity::OnAddComponent(
		EntityId                      a_id,
		Detail::EntityRepresentation* a_rep,
		Detail::ComponentListElement& a_listElement
	)
	{
		auto& [id, component] = a_listElement;

		component->m_entityId = a_id;

		component->OnCreate();

		a_rep->needsFirstUpdate.push_back(id);

		a_rep->components.emplace_back(std::move(a_listElement));
	}

	void
	Entity::OnRemoveComponent(
		Detail::EntityRepresentation* a_rep,
		Detail::ComponentListElement& a_listElement
	)
	{
		auto& [id, component] = a_listElement;

		component->OnDestroy();

		auto iter = FindComponentById(a_rep, id);

		a_rep->components.erase(iter);
	}

	bool
	Entity::RemoveComponent(identity<Next::Transform>)
	{
		auto static_id = Reflection::GetTypeId<Next::Transform>();
		return RemoveComponent(static_id);
	}

	void
	Entity::OnUpdate(Detail::EntityRepresentation& a_rep)
	{
		for (auto id : a_rep.needsFirstUpdate)
		{
			auto iter = FindComponentById(&a_rep, id);
			if (iter == a_rep.components.end())
			{
				continue;
			}

			iter->component->OnFirstUpdate();
		}

		for (auto& [id, component] : a_rep.components)
		{
			component->OnUpdate();
		}
	}

	void
	Entity::OnDestroy(Detail::EntityRepresentation& a_rep)
	{
		for (auto& [id, component] : a_rep.components)
		{
			component->OnDestroy();
		}
	}
}
