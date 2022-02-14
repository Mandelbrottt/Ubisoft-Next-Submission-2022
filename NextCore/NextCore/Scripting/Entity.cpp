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

		throw "Not Implemented";

		// TODO: Need a way to construct from just a type id
		return nullptr;
	}

	bool
	Entity::RemoveComponent(Reflection::StaticTypeId a_typeId)
	{
		if (a_typeId == Reflection::StaticTypeId::Null)
		{
			return false;
		}

		auto iter   = FindComponentById(a_typeId);
		bool result = iter != m_components.end();

		if (result)
		{
			delete iter->component;
			m_components.erase(iter);
		}

		return result;
	}

	bool
	Entity::RemoveComponent(Component* a_component)
	{
		if (a_component == nullptr)
		{
			return false;
		}

		auto iter   = FindComponent(a_component);
		bool result = iter != m_components.end();

		if (result)
		{
			m_components.erase(iter);
		}

		return result;
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
}
