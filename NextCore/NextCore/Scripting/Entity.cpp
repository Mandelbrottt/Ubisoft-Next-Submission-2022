#include "pch.h"

#include "Entity.h"
#include "Component.h"

#include "Components/Transform.h"

namespace Next
{
	Entity::Entity()
		: Object({ "Entity" })
	{
		m_entityId = EntityId::Null;

		// TODO: Will eventually be implemented by the registry when a new entity is created
		OnCreate();
	}

	void
	Entity::OnCreate()
	{
		AddComponent<Next::Transform>();
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

	Transform*
	Entity::Transform()
	{
		return GetComponent<Next::Transform>();
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
	
	Component**
	Entity::GetComponents(Reflection::StaticTypeId a_typeId, int* a_outCount)
	{
		if (a_typeId == Reflection::StaticTypeId::Null || !a_outCount)
		{
			return nullptr;
		}

		*a_outCount = NumComponents(a_typeId);

		Component** components = new Component*[*a_outCount];

		int count = 0;
		for (const auto& [id, component] : m_components)
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
	Entity::NumComponents(Reflection::StaticTypeId a_typeId) const
	{
		auto predicate = [&](ComponentListElement const& a_value)
		{
			return a_value.id == a_typeId;
		};

		auto numComponents = std::count_if(m_components.begin(), m_components.end(), predicate);

		return static_cast<int>(numComponents);
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
		component->m_entityId = m_entityId;

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
	
	bool
	Entity::RemoveComponent(identity<Next::Transform>)
	{
		auto static_id = Reflection::GetStaticId<Next::Transform>();
		return RemoveComponent(static_id);
	}
}
