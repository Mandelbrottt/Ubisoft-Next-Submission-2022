#include "pch.h"

#include "Entity.h"

namespace NextCore::Scripting
{
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

		auto* constructor = componentType->GetConstructor();
		void* p = constructor->Construct();
		Component* result = static_cast<Component*>(p);

		m_components.push_back({ a_typeId, result });
		
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
			if (auto* componentType = Reflection::Type::TryGet(a_iter->id); componentType != nullptr)
			{
				auto* constructor = componentType->GetConstructor();
				constructor->Destruct(a_iter->component);
			}
			
			m_components.erase(a_iter);
		}

		return result;
	}
}
