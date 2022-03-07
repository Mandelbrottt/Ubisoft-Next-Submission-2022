#pragma once

#include <type_traits>
#include <unordered_set>

#include "EntityId.h"

#include "Reflection/Type.h"

#include "Detail/Registry.h"

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
		class Registry;

		//struct ComponentListElement
		//{
		//	Reflection::TypeId id;

		//	Component* component;
		//};

		//struct EntityRepresentation
		//{
		//	std::vector<ComponentListElement> components;

		//	std::vector<Reflection::TypeId> needsFirstUpdate;
		//};
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

		template<typename TComponent>
		static
		void
		GetAllComponents(std::vector<TComponent*>& a_vectorToPopulate);
		
		template<typename TComponent>
		static
		std::vector<TComponent*>
		GetAllComponents();

	public:
		Entity(Entity const& a_other) = default;
		Entity&
		operator =(Entity const& a_other) = default;

		Entity(Entity&& a_other) = default;
		Entity&
		operator =(Entity&& a_other) = default;

		bool
		operator ==(Entity const& a_other) const;
		
		bool
		operator !=(Entity const& a_other) const;
		
		explicit
		Entity(EntityId a_id);

		static
		Entity
		Create();

		static
		void
		Destroy(Entity& a_entity);

		void
		Destroy();

		void
		OnCreate();
		
		EntityId
		GetEntityId() const
		{
			return m_entityId;
		}

		bool
		IsValid() const;

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
			auto typeId = Reflection::Type::Get<TComponent>().GetTypeId();
			
			Component* result = OnAddComponent(m_entityId, typeId);

			return static_cast<TComponent*>(result);
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
		
	private:
		EntityId m_entityId;

		// TODO: Somehow store this in the registry and access from there instead of storing locally
		std::string m_name;
		
	private:
		static
		Component*
		OnAddComponent(EntityId a_id, Reflection::TypeId a_typeId);

		static
		bool
		OnRemoveComponent(EntityId a_id, Reflection::TypeId a_typeId);

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
		Detail::Registry&
		Registry();

	private:
		//static std::unordered_map<EntityId, Detail::EntityRepresentation> s_entityRepresentations;

		static std::unordered_set<EntityId> s_entityIdDestroyBuffer;
		
		static std::vector<std::pair<EntityId, Reflection::TypeId>> s_entityIdFirstUpdateBuffer;
	};

	template<typename TComponent>
	void
	Entity::GetAllComponents(std::vector<TComponent*>& a_vectorToPopulate)
	{
		const auto typeId = Reflection::Type::Get<TComponent>().GetTypeId();

		a_vectorToPopulate.clear();

		auto& activeEntities = Registry().GetActiveEntities();

		a_vectorToPopulate.reserve(activeEntities.size());
		
		for (EntityId entityId : activeEntities)
		{
			auto component = Registry().GetComponent(entityId, typeId);

			if (!component)
			{
				continue;
			}

			a_vectorToPopulate.push_back(static_cast<TComponent*>(component));
		}
	}

	template<typename TComponent>
	std::vector<TComponent*>
	Entity::GetAllComponents()
	{
		std::vector<TComponent*> result;
		GetAllComponents<TComponent>(result);
		return result;
	}
}
