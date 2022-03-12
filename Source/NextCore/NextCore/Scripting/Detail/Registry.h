#pragma once

#include <unordered_set>

#include "ComponentPool.h"

#include "Reflection/Type.h"
#include "Reflection/TypeId.h"

#include "Scripting/EntityId.h"

namespace Next
{
	class Component;
}

namespace Next::Detail
{
	class Registry
	{
	public:
		void
		Reset();

		/**
		 * \brief Creates and registers an entityId with the registry.
		 * \return The EntityId of the newly created entity.
		 */
		[[nodiscard]]
		EntityId
		OnCreateEntity();

		/**
		 * \brief Destroys the entity with id a_entityId. 
		 * \param a_entityId The entity to destroy.
		 */
		void
		OnDestroyEntity(EntityId a_entityId);
		
		/**
		 * \brief Call OnFirstUpdate for the given entity and component.
		 * \param a_entityId The entity to call OnFirstUpdate on.
		 * \param a_typeId The component type to call OnFirstUpdate on.
		 */
		void
		OnFirstUpdate(EntityId a_entityId, Reflection::TypeId a_typeId);
		
		/**
		 * \brief Update all currently active components.
		 */
		void
		OnUpdate();

		std::string const&
		GetName(EntityId a_entityId);
		
		void
		SetName(EntityId a_entityId, std::string_view const& a_name);

		/**
		 * \brief Check if a given entity is currently active.
		 */
		bool
		IsValid(EntityId a_entityId) const;

		std::unordered_set<EntityId>&
		GetActiveEntities()
		{
			return m_activeEntities;
		}
		
		/**
		 * \brief Construct and register a component owned by the entity with id a_entityId
		 * \tparam TComponent The type of the component to register.
		 * \param a_entityId The entity to register the component to.
		 * \return The reference-tracked pointer to the component.
		 */
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		AddComponent(EntityId a_entityId)
		{
			Reflection::TypeId static_id = Reflection::Type::Get<TComponent>().GetTypeId();

			auto* result = AddComponent(a_entityId, static_id);
			return static_cast<TComponent*>(result);
		}
		
		/**
		 * \brief Construct and register a component owned by the entity with id a_entityId
		 * \param a_entityId The entity to register the component to.
		 * \param a_type The runtime Type of the component to create.
		 * \return The reference-tracked pointer to the component.
		 */
		Component*
		AddComponent(EntityId a_entityId, Reflection::Type const& a_type);
		
		/**
		 * \brief Construct and register a component owned by the entity with id a_entityId
		 * \param a_entityId The entity to register the component to.
		 * \param a_typeId The runtime type id of the component to create.
		 * \return The reference-tracked pointer to the component.
		 */
		Component*
		AddComponent(EntityId a_entityId, Reflection::TypeId a_typeId);

		/**
		 * \brief Destroy and unregister the component owned by entity with id a_entityId
		 * \tparam TComponent The type of the component to destroy.
		 * \param a_entityId The entity to remove the component from.
		 * \return True on success, false on failure
		 */
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent(EntityId a_entityId)
		{
			Reflection::TypeId static_id = Reflection::Type::Get<TComponent>().GetTypeId();

			auto result = RemoveComponent(a_entityId, static_id);
			return result;
		}
		
		/**
		 * \brief Destroy and unregister the component owned by entity with id a_entityId
		 * \param a_entityId The entity to remove the component from.
		 * \param a_type The runtime type of the component to be removed.
		 * \return True on success, false on failure
		 */
		bool
		RemoveComponent(EntityId a_entityId, Reflection::Type const& a_type);
		
		/**
		 * \brief Destroy and unregister the component owned by entity with id a_entityId
		 * \param a_entityId The entity to remove the component from.
		 * \param a_typeId The runtime type id of the component to be removed.
		 * \return True on success, false on failure
		 */
		bool
		RemoveComponent(EntityId a_entityId, Reflection::TypeId a_typeId);
		
		/**
		 * \brief Retrieve the component owned by entity with id a_entityId
		 * \tparam TComponent The type of the component to retrieve.
		 * \param a_entityId The entity to retrieve the component for.
		 * \return The reference-tracked pointer to the component.
		 */
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		GetComponent(EntityId a_entityId)
		{
			Reflection::TypeId static_id = Reflection::Type::Get<TComponent>().GetTypeId();

			auto* result = GetComponent(a_entityId, static_id);
			return static_cast<TComponent*>(result);
		}
		
		/**
		 * \brief Retrieve the component owned by entity with id a_entityId
		 * \param a_entityId The entity to retrieve the component for.
		 * \param a_type The runtime type of the component to be retrieved.
		 * \return The reference-tracked pointer to the component.
		 */
		Component*
		GetComponent(EntityId a_entityId, Reflection::Type const& a_type);
		
		/**
		 * \brief Retrieve the component owned by entity with id a_entityId
		 * \param a_entityId The entity to retrieve the component for.
		 * \param a_typeId The runtime type id of the component to be retrieved.
		 * \return The reference-tracked pointer to the component.
		 */
		Component*
		GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId);
		
		/**
		 * \brief Retrieve the component owned by entity with id a_entityId
		 * \tparam TComponent The type of the component to retrieve.
		 * \param a_entityId The entity to retrieve the component for.
		 * \return The reference-tracked pointer to the component.
		 */
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent const*
		GetComponent(EntityId a_entityId) const
		{
			return const_cast<Registry*>(this)->GetComponent<TComponent>(a_entityId);
		}
		
		/**
		 * \brief Retrieve the component owned by entity with id a_entityId
		 * \param a_entityId The entity to retrieve the component for.
		 * \param a_type The runtime type of the component to be retrieved.
		 * \return The reference-tracked pointer to the component.
		 */
		Component const*
		GetComponent(EntityId a_entityId, Reflection::Type const& a_type) const;
		
		/**
		 * \brief Retrieve the component owned by entity with id a_entityId
		 * \param a_entityId The entity to retrieve the component for.
		 * \param a_typeId The runtime type id of the component to be retrieved.
		 * \return The reference-tracked pointer to the component.
		 */
		Component*
		GetComponent(EntityId a_entityId, Reflection::TypeId a_typeId) const;

		std::vector<Component*>
		GetAllComponentsForEntity(EntityId a_entityId) const;
		
		void
		GetAllComponentsForEntity(EntityId a_entityId, std::vector<Component*>& a_outVector) const;
		
	private:
		EntityId
		IncrementEntityCounter();

		/**
		 * \brief Construct a ComponentPool of the given type if it doesn't exist.
		 * \param a_typeId The type id of the component in the pool.
		 * \return A pointer to a valid component pool that contains components with the given type id.
		 */
		ComponentPool*
		EnsureComponentPoolInstantiated(Reflection::TypeId a_typeId);

		/**
		 * \brief Iterate all of the members of the given type, and add their reference-tracking information
		 *        to the appropriate component pool.
		 * \param a_typeId The type id to generate reference tracking info fo.
		 */
		void
		GenerateComponentReferenceTrackingInfo(Reflection::TypeId a_typeId);

		/**
		 * \brief Find all references in registered components and set them to nullptr if they reference
		 *        the component with the given type id owned by the entity with the given id.
		 */
		void
		InvalidateReferencesToComponent(EntityId a_entityId, Reflection::TypeId a_typeId);

		// Retrieves and caches all of the references stored in all of the components of the given type
		void
		CacheAllReferencesOfType(Reflection::TypeId a_referenceTypeId);
		
		// Restores all of the references stored in all of the components of the given type
		void
		RestoreAllReferencesOfType(Reflection::TypeId a_referenceTypeId);

	private:
		EntityId m_nextEntityId = EntityId::FirstValid;
		
		// The set of all active entities.
		std::unordered_set<EntityId> m_activeEntities;

		struct EntityInfo
		{
			std::string name;
		};

		// TODO: Find way to integrate with m_activeEntities without making it a map and without exposing entity info
		std::unordered_map<EntityId, EntityInfo> m_entityInfos;

		struct ComponentPoolInfo
		{
			ComponentPool pool;

			std::vector<Reflection::Field const*> fieldsToRefWatch;
		};

		// The map of all component pools and their types.
		std::unordered_map<Reflection::TypeId, ComponentPoolInfo> m_componentPoolInfos;
	};
}
