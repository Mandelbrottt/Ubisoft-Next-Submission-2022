#pragma once

#include "Object.h"
#include "Entity.h"

#include "Reflection/Type.h"

namespace NextCore::Scripting
{
	/**
	 * \brief The base class for all components in NextCore
	 */
	class Component : public Object
	{
		friend class Entity;
	protected:
		Component() = default;

		~Component() override = default;
	public:
		#pragma region Getters and Setters
		EntityId
		GetEntityId() const;

		Entity*
		GetEntity() const;

		// TEMPORARY:
		void
		SetEntity(Entity* a_entity);
		#pragma endregion

		#pragma region Event Functions
		virtual
		void
		OnCreate() {}

		virtual
		void
		OnFirstUpdate() {}

		virtual
		void
		OnUpdate() {}

		virtual
		void
		OnDestroy() {}
		#pragma endregion

		#pragma region Component Access Functions
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		AddComponent()
		{
			return m_entity->AddComponent<TComponent>();
		}

		Component*
		AddComponent(Reflection::Type const& a_type);

		Component*
		AddComponent(Reflection::StaticTypeId a_typeId);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent()
		{
			return m_entity->RemoveComponent<TComponent>();
		}

		bool
		RemoveComponent(Reflection::Type const& a_type);

		bool
		RemoveComponent(Reflection::StaticTypeId a_typeId);

		bool
		RemoveComponent(Component* a_component);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		GetComponent()
		{
			return m_entity->GetComponent<TComponent>();
		}

		Component*
		GetComponent(Reflection::Type const& a_type);

		Component*
		GetComponent(Reflection::StaticTypeId a_typeId);
		#pragma endregion

	private:
		EntityId entityId;

		Entity* m_entity;
	};
}
