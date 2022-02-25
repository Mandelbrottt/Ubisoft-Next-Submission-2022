#pragma once

#include "EntityId.h"
#include "Object.h"

#include "Reflection/Type.h"
#include "Reflection/Factory.h"

#include <type_traits>

namespace Next
{
	class Transform;
	class Component;

	/**
	 * \brief 
	 */
	class Entity : public Object
	{
	public:
		// TODO: Convert to protected when entity system is in place
		Entity();

		void
		OnCreate();

		void
		OnUpdate();

		EntityId
		GetEntityId() const
		{
			return m_entityId;
		}

		Next::Transform*
		Transform();

		Next::Transform const*
		Transform() const
		{
			return const_cast<Entity*>(this)->Transform();
		}

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		AddComponent();

		Component*
		AddComponent(Reflection::Type const& a_type)
		{
			return AddComponent(a_type.GetStaticId());
		}

		Component*
		AddComponent(Reflection::StaticTypeId a_typeId);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent()
		{
			// HACK: Use specialization shenanigans to allow private specialization for Transform
			return RemoveComponent<TComponent>(identity<TComponent> {});
		}

		bool
		RemoveComponent(Reflection::Type const& a_type)
		{
			return RemoveComponent(a_type.GetStaticId());
		}

		bool
		RemoveComponent(Reflection::StaticTypeId a_typeId);

		bool
		RemoveComponent(Component* a_component);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		GetComponent()
		{
			auto static_id = Reflection::GetStaticId<TComponent>();
			return static_cast<TComponent*>(GetComponent(static_id));
		}

		Component*
		GetComponent(Reflection::Type const& a_type)
		{
			return GetComponent(a_type.GetStaticId());
		}

		Component*
		GetComponent(Reflection::StaticTypeId a_typeId);
		
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent const*
		GetComponent() const
		{
			return const_cast<Entity*>(this)->GetComponent<TComponent>();
		}

		Component const*
		GetComponent(Reflection::Type const& a_type) const
		{
			return const_cast<Entity*>(this)->GetComponent(a_type);
		}

		Component*
		GetComponent(Reflection::StaticTypeId a_typeId) const
		{
			return const_cast<Entity*>(this)->GetComponent(a_typeId);
		}

		/**
		 * \brief 
		 * \tparam TComponent 
		 * \param a_outCount 
		 * \return 
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent**
		GetComponents(int* a_outCount)
		{
			auto static_id = Reflection::GetStaticId<TComponent>();
			return reinterpret_cast<TComponent**>(GetComponents(static_id, a_outCount));
		}

		/**
		 * \brief 
		 * \param a_type 
		 * \param a_outCount 
		 * \return 
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		Component**
		GetComponents(Reflection::Type const& a_type, int* a_outCount)
		{
			return GetComponents(a_type.GetStaticId(), a_outCount);
		}

		/**
		 * \brief 
		 * \param a_typeId 
		 * \param a_outCount 
		 * \return
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		Component**
		GetComponents(Reflection::StaticTypeId a_typeId, int* a_outCount);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		int
		NumComponents() const
		{
			auto static_id = Reflection::GetStaticId<TComponent>();
			return NumComponents(static_id);
		}

		int
		NumComponents(Reflection::Type const& a_type) const
		{
			return NumComponents(a_type.GetStaticId());
		}

		int
		NumComponents(Reflection::StaticTypeId a_typeId) const;

	private:
		EntityId m_entityId;

		struct ComponentListElement
		{
			Reflection::StaticTypeId id;

			Component* component;
		};

		std::vector<ComponentListElement> m_components;

		std::vector<Reflection::StaticTypeId> m_needsFirstUpdate;

	private:
		decltype(m_components)::iterator
		FindComponentById(Reflection::StaticTypeId a_id);

		decltype(m_components)::iterator
		FindComponent(Component* a_component);
		
		decltype(m_components)::const_iterator
		FindComponentById(Reflection::StaticTypeId a_id) const
		{
			return const_cast<Entity*>(this)->FindComponentById(a_id);
		}

		decltype(m_components)::const_iterator
		FindComponent(Component* a_component) const
		{
			return const_cast<Entity*>(this)->FindComponent(a_component);
		}

		bool
		RemoveComponentByIterator(decltype(m_components)::iterator a_iter);

		void
		OnAddComponent(ComponentListElement& a_listElement);

		void
		OnRemoveComponent(ComponentListElement& a_listElement);

		template<typename T>
		struct identity
		{
			using type = T;
		};

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent(identity<TComponent>)
		{
			auto static_id = Reflection::GetStaticId<TComponent>();
			return RemoveComponent(static_id);
		}

		/**
		 * \brief Specialization to only allow this and friends to remove transform
		 */
		bool
		RemoveComponent(identity<Next::Transform>);
	};

	template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool>>
	TComponent*
	Entity::AddComponent()
	{
		auto static_id = Reflection::GetStaticId<TComponent>();

		// TODO: Convert to use a pool allocator
		Reflection::TypedFactory<TComponent> constructor;
		auto                                result = static_cast<TComponent*>(constructor.Construct());

		ComponentListElement element = { static_id, result };
		OnAddComponent(element);

		return result;
	}
}
