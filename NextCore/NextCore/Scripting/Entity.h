#pragma once

#include "EntityId.h"
#include "Object.h"

#include "Reflection/Type.h"
#include "Reflection/Factory.h"

#include <type_traits>
#include <unordered_set>
#include <unordered_map>

void
Init();

void
Render();

void
Update(float a_deltaTime);

void
Shutdown();

namespace Next
{
	class Transform;
	class Component;

	namespace Detail
	{
		struct ComponentListElement
		{
			Reflection::TypeId id;

			Component* component;
		};

		struct EntityRepresentation
		{
			std::vector<ComponentListElement> components;

			std::vector<Reflection::TypeId> needsFirstUpdate;
		};
	}
	
	/**
	 * \brief 
	 */
	class Entity
	{
		friend void ::Init();
		friend void ::Update(float a_deltaTime);
		friend void ::Render();
		friend void ::Shutdown();

		Entity() = default;

		static
		void
		Update();

	public:
		Entity(Entity const& a_other) = default;
		Entity&
		operator =(Entity const& a_other) = default;

		Entity(Entity&& a_other) = default;
		Entity&
		operator =(Entity&& a_other) = default;

		explicit
		Entity(EntityId a_id);

		static
		Entity
		Create();

		static
		void
		Destroy(Entity const& a_entity);

		void
		Destroy();

		void
		OnCreate();
		
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

		std::string const&
		GetName() const
		{
			return m_name;
		}

		std::string const&
		SetName(std::string_view a_name)
		{
			return (m_name = a_name);
		}

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		AddComponent()
		{
			auto* rep = GetCurrentEntityRepresentation();

			if (!rep)
			{
				return {};
			}

			auto static_id = Reflection::GetTypeId<TComponent>();

			// TODO: Convert to use a pool allocator
			Reflection::TypedFactory<TComponent> factory;

			auto result = static_cast<TComponent*>(factory.Construct());

			Detail::ComponentListElement element = { static_id, result };
			OnAddComponent(m_entityId, rep, element);

			return result;
		}

		Component*
		AddComponent(Reflection::Type const& a_type)
		{
			return AddComponent(a_type.GetTypeId());
		}

		Component*
		AddComponent(Reflection::TypeId a_typeId);

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
			return RemoveComponent(a_type.GetTypeId());
		}

		bool
		RemoveComponent(Reflection::TypeId a_typeId);

		bool
		RemoveComponent(Component* a_component);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		GetComponent()
		{
			auto static_id = Reflection::GetTypeId<TComponent>();
			return static_cast<TComponent*>(GetComponent(static_id));
		}

		Component*
		GetComponent(Reflection::Type const& a_type)
		{
			return GetComponent(a_type.GetTypeId());
		}

		Component*
		GetComponent(Reflection::TypeId a_typeId);

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
		GetComponent(Reflection::TypeId a_typeId) const
		{
			return const_cast<Entity*>(this)->GetComponent(a_typeId);
		}
		
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		void
		GetComponents(std::vector<TComponent*>& a_outComponents) const
		{
			// TODO: Find a way to avoid allocation in GetComponents. Copy body over?
			std::vector<Component*> result;
			auto static_id = Reflection::GetTypeId<TComponent>();
			GetComponents(static_id, result);
			a_outComponents.clear();
			a_outComponents.reserve(result.size());
			for (int i = 0; i < result.size(); i++)
			{
				a_outComponents.push_back(static_cast<TComponent*>(result[i]));
			}
		}
	
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		std::vector<TComponent*>
		GetComponents() const
		{
			std::vector<TComponent*> result;
			GetComponents<TComponent>(result);
			return result;
		}
	
		void
		GetComponents(Reflection::Type const& a_type, std::vector<Component*>& a_outComponents) const
		{
			return GetComponents(a_type.GetTypeId(), a_outComponents);
		}
		
		std::vector<Component*>
		GetComponents(Reflection::Type const& a_type) const
		{
			std::vector<Component*> result;
			GetComponents(a_type.GetTypeId(), result);
			return result;
		}
		
		void
		GetComponents(Reflection::TypeId a_typeId, std::vector<Component*>& a_outComponents) const;
		
		std::vector<Component*>
		GetComponents(Reflection::TypeId a_typeId) const
		{
			std::vector<Component*> result;
			GetComponents(a_typeId, result);
			return result;
		}

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		int
		NumComponents() const
		{
			auto static_id = Reflection::GetTypeId<TComponent>();
			return NumComponents(static_id);
		}

		int
		NumComponents(Reflection::Type const& a_type) const
		{
			return NumComponents(a_type.GetTypeId());
		}

		int
		NumComponents(Reflection::TypeId a_typeId) const;

	private:
		EntityId m_entityId;

		std::string m_name;

	private:
		Detail::EntityRepresentation*
		GetCurrentEntityRepresentation();

		Detail::EntityRepresentation const*
		GetCurrentEntityRepresentation() const;

		static
		decltype(Detail::EntityRepresentation::components)::iterator
		FindComponentById(Detail::EntityRepresentation* a_rep, Reflection::TypeId a_id);

		static
		decltype(Detail::EntityRepresentation::components)::iterator
		FindComponent(Detail::EntityRepresentation* a_rep, Component* a_component);

		static
		bool
		RemoveComponentByIterator(
			Detail::EntityRepresentation*                                  a_rep,
			decltype(Detail::EntityRepresentation::components)::iterator a_iter
		);

		static
		void
		OnAddComponent(EntityId a_id, Detail::EntityRepresentation* a_rep, Detail::ComponentListElement& a_listElement);

		static
		void
		OnRemoveComponent(Detail::EntityRepresentation* a_rep, Detail::ComponentListElement& a_listElement);

		template<typename T>
		struct identity
		{
			using type = T;
		};

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent(identity<TComponent>)
		{
			auto static_id = Reflection::GetTypeId<TComponent>();
			return RemoveComponent(static_id);
		}

		/**
		 * \brief Specialization to only allow this and friends to remove transform
		 */
		bool
		RemoveComponent(identity<Next::Transform>);

		static
		void
		OnUpdate(Detail::EntityRepresentation& a_rep);

		static
		void
		OnDestroy(Detail::EntityRepresentation& a_rep);

	private:
		static std::unordered_map<EntityId, Detail::EntityRepresentation> s_entityRepresentations;

		static std::unordered_set<EntityId> s_entityIdDestroyBuffer;
	};
}
