#pragma once

#include <vector>
#include <unordered_map>
#include <queue>
#include <functional>

#include "Reflection/TypeId.h"

#include "Scripting/EntityId.h"

namespace Next
{
	namespace Reflection {
		struct GenericFactory;
	}

	class Component;
}

namespace Next::Detail
{
	using ResizeCallback = void(Reflection::TypeId);
	
	class ComponentPool
	{
	public:
		using size_type = uint32_t;

		explicit
		ComponentPool(Reflection::GenericFactory const* a_factory);
		
		explicit
		ComponentPool(Reflection::GenericFactory const* a_factory, size_type a_initialSize);

		~ComponentPool();

		/**
		 * \brief Set callbacks for when the component pool is resized. This gives the opportunity for
		 *        observers to collect any information about the state and memory of the pool before it
		 *        is reallocated.
		 * \tparam T The type of the class containing the callback
		 * \param a_listener A pointer the class that will receive the callbacks
		 * \param a_preResizeCallback  The function to be called before the pool is resized.
		 * \param a_postResizeCallback The function to be called after the pool is resized.
		 */
		template<typename T>
		void
		SetResizeCallbacks(
			T* a_listener, 
			void (T::*a_preResizeCallback)(Reflection::TypeId),
			void (T::*a_postResizeCallback)(Reflection::TypeId)
		)
		{
			m_preResizeCallback  = std::bind(a_preResizeCallback,  a_listener, std::placeholders::_1);
			m_postResizeCallback = std::bind(a_postResizeCallback, a_listener, std::placeholders::_1);
		}
		
		/**
		 * \brief Add a new component to the pool owned by the entity with id a_entityId
		 * \param a_entityId The entity to own the new component
		 * \return A reference-tracked pointer to the newly created component
		 */
		Component*
		AddComponent(EntityId a_entityId);

		/**
		 * \brief Get the component owned by the entity with id a_entityId
		 * \param a_entityId The entity that owns the component
		 * \return A reference-tracked pointer to the component
		 */
		Component*
		GetComponent(EntityId a_entityId);
		
		/**
		 * \brief Get the component owned by the entity with id a_entityId
		 * \param a_entityId The entity that owns the component
		 * \return A reference-tracked pointer to the component
		 */
		Component const*
		GetComponent(EntityId a_entityId) const
		{
			return const_cast<ComponentPool*>(this)->GetComponent(a_entityId);
		}

		/**
		 * \brief Get all of the components in the pool
		 * \param a_outComponents the vector to populate with component pointers
		 */
		void
		GetAllComponents(std::vector<Component*>& a_outComponents);

		/**
		 * \brief Remove and destroy the component owned by the entity with id a_entityId
		 * \param a_entityId The entity that owns the component
		 * \return True on success, false on failure
		 */
		bool
		RemoveComponent(EntityId a_entityId);

	private:
		/**
		 * \brief Resize the pool to accomodate at least a_newSize components
		 * \param a_newSize The new size of the pool in components
		 */
		void
		Resize(size_type a_newSize);
		
		/**
		 * \return The new size of the pool given its current size
		 */
		size_type
		AutoCalculateNewSize() const;
	
		/**
		 * \brief Retrieve a pointer to the component at the given index.
		 * \param a_index The index of the component to retrieve.
		 * \return The reference-tracked pointer to the component at the given index.
		 * \remark The offset into the byte array is calculated based off the given index, so
		 *         pass in an index as if the array value type was the component, and not bytes.
		 */
		Component*
		GetComponentFromIndex(size_type a_index);
	
		/**
		 * \brief Retrieve a pointer to the component at the given index.
		 * \param a_index The index of the component to retrieve.
		 * \return The reference-tracked pointer to the component at the given index.
		 * \remark The offset into the byte array is calculated based off the given index, so
		 *         pass in an index as if the array value type was the component, and not bytes.
		 */
		Component const*
		GetComponentFromIndex(size_type a_index) const
		{
			return const_cast<ComponentPool*>(this)->GetComponentFromIndex(a_index);
		}
	
	private:
		// The packed byte array that contains the components.
		std::vector<std::byte> m_componentStorage;

		// The list of indices that are not currently populated with a component.
		std::queue<size_type> m_availableIndexQueue;

		// A map of what entities own components in the pool, and what index their components are at.
		std::unordered_map<EntityId, size_type> m_indexMap;

		// Use raw pointer here because the lifetime of factories is until program-end once constructed
		Reflection::GenericFactory const* const m_factory;

		// The TypeId of the components in the pool.
		const Reflection::TypeId m_typeId;

		// The size of a given element in bytes.
		const size_type m_elementSize;

		// The number of active components in the pool.
		size_type m_count;

		std::function<ResizeCallback> m_preResizeCallback;
		std::function<ResizeCallback> m_postResizeCallback;
	};
}
