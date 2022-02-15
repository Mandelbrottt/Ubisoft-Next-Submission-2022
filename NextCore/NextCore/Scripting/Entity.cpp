#include "pch.h"

#include "Entity.h"
#include "Component.h"

namespace NextCore::Scripting
{
	Entity::Entity()
	{
		m_entityId = EntityId::Null;
	}

	void
	Entity::OnUpdate()
	{
		for (auto& id : m_needsFirstUpdate)
		{
			auto iter = FindComponentById(id);
			if (iter == m_components.end())
			{
				continue;
			}

			iter->component->OnFirstUpdate();
		}

		for (auto& [id, component] : m_components)
		{
			component->OnUpdate();
		}
	}

	Component*
	Entity::AddComponent(Reflection::StaticTypeId a_typeId)
	{
		if (a_typeId == Reflection::StaticTypeId::Null)
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
		
		ComponentListElement element = { a_typeId, result };
		OnAddComponent(element);

		return result;
	}

	bool
	Entity::RemoveComponent(Reflection::StaticTypeId a_typeId)
	{
		if (a_typeId == Reflection::StaticTypeId::Null)
		{
			return false;
		}

		auto iter = FindComponentById(a_typeId);

		return RemoveComponentByIterator(iter);
	}

	bool
	Entity::RemoveComponent(Component* a_component)
	{
		if (a_component == nullptr)
		{
			return false;
		}

		auto iter = FindComponent(a_component);

		return RemoveComponentByIterator(iter);
	}

	Component*
	Entity::GetComponent(Reflection::StaticTypeId a_typeId)
	{
		if (a_typeId == Reflection::StaticTypeId::Null)
		{
			return nullptr;
		}

		Component* result = nullptr;

		if (auto iter = FindComponentById(a_typeId); iter != m_components.end())
		{
			result = iter->component;
		}

		return result;
	}

	decltype(Entity::m_components)::iterator
	Entity::FindComponentById(Reflection::StaticTypeId a_id)
	{		
		auto iter = m_components.begin();
		auto end  = m_components.end();

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

	decltype(Entity::m_components)::iterator
	Entity::FindComponent(Component* a_component)
	{
		auto iter = m_components.begin();
		auto end  = m_components.end();

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
	Entity::RemoveComponentByIterator(decltype(m_components)::iterator a_iter)
	{
		bool result = a_iter != m_components.end();

		// If component exists and it's type is registered with reflection, destruct it
		if (result)
		{
			auto& [id, component] = *a_iter;
			ComponentListElement element { id,  component };
			OnRemoveComponent(element);
			
			if (auto* componentType = Reflection::Type::TryGet(id); componentType != nullptr)
			{
				auto* constructor = componentType->GetConstructor();
				constructor->Destruct(component);
			}
		}

		return result;
	}

	void
	Entity::OnAddComponent(ComponentListElement& a_listElement)
	{
		auto& [id, component] = a_listElement;

		component->m_entity = this;
		component->entityId = m_entityId;

		component->OnCreate();

		m_needsFirstUpdate.push_back(id);
		
		m_components.emplace_back(std::move(a_listElement));
	}

	void
	Entity::OnRemoveComponent(ComponentListElement& a_listElement)
	{
		auto& [id, component] = a_listElement;

		component->OnDestroy();

		auto iter = FindComponentById(id);

		m_components.erase(iter);
	}
}
