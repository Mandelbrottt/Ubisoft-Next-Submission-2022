#pragma once

#define GenerateConstructors(_class_) \
		_MACRO_AUTO_FORMAT_INDENT \
	protected: \
		template<typename T, bool>\
		friend struct ::Next::Reflection::TypedFactory; \
		friend ::Next::Reflection::Detail::typed_factory_friend_helper; \
		\
		_class_() \
			: Base({ }) { } \
		~_class_() override = default;

// Include after GenerateConstructors is defined because there is a #define that relies on GenerateConstructors
#include "Reflection/Reflection.h"

#include "Object.h"
#include "Entity.h"

namespace Next::Reflection
{
	template<typename T, bool>
	struct TypedFactory;

	namespace Detail
	{
		struct typed_factory_friend_helper;
	}
}

namespace Next
{
	class Transform;

	/**
	 * \brief The base class for all components in NextCore
	 */
	class Component : public Object
	{
		friend class Entity;

		ReflectDeclareNoConstructors(Component, Object)

	protected:
		struct ComponentConstructionArgs : ObjectConstructionArgs { };

		explicit
		Component(ComponentConstructionArgs const& a_args);

		~Component() override = default;

	public:		
		#pragma region Getters and Setters
		EntityId
		GetEntityId() const;

		Entity
		GetEntity() const;

		Transform*
		Transform();

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
			return GetEntity().AddComponent<TComponent>();
		}

		Component*
		AddComponent(Reflection::Type const& a_type);

		Component*
		AddComponent(Reflection::TypeId a_typeId);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent()
		{
			return GetEntity().RemoveComponent<TComponent>();
		}

		bool
		RemoveComponent(Reflection::Type const& a_type);

		bool
		RemoveComponent(Reflection::TypeId a_typeId);

		bool
		RemoveComponent(Component* a_component);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent*
		GetComponent()
		{
			return GetEntity().GetComponent<TComponent>();
		}

		Component*
		GetComponent(Reflection::Type const& a_type);

		Component*
		GetComponent(Reflection::TypeId a_typeId);
		
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent const*
		GetComponent() const
		{
			return const_cast<Component*>(this)->GetComponent<TComponent>();
		}

		Component const*
		GetComponent(Reflection::Type const& a_type) const
		{
			return const_cast<Component*>(this)->GetComponent(a_type);
		}

		Component*
		GetComponent(Reflection::TypeId a_typeId) const
		{
			return const_cast<Component*>(this)->GetComponent(a_typeId);
		}

		/**
		 * \brief 
		 * \tparam TComponent 
		 * \param a_outComponents 
		 * \return 
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		void
		GetComponents(std::vector<TComponent*>& a_outComponents) const
		{
			return GetEntity().GetComponents<TComponent>(a_outComponents);
		}
		
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		std::vector<TComponent*>
		GetComponents() const
		{
			std::vector<TComponent*> result;
			GetEntity().GetComponents<TComponent>(result);
			return result;
		}

		/**
		 * \brief 
		 * \param a_type 
		 * \param a_outComponents 
		 * \return 
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		void
		GetComponents(Reflection::Type const& a_type, std::vector<Component*>& a_outComponents) const
		{
			return GetEntity().GetComponents(a_type, a_outComponents);
		}

		std::vector<Component*>
		GetComponents(Reflection::Type const& a_type) const
		{
			std::vector<Component*> result;
			GetComponents(a_type, result);
			return result;
		}

		/**
		 * \brief 
		 * \param a_typeId 
		 * \param a_outComponents 
		 * \return 
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		void
		GetComponents(Reflection::TypeId a_typeId, std::vector<Component*>& a_outComponents) const
		{
			return GetEntity().GetComponents(a_typeId, a_outComponents);
		}

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
			return GetEntity().NumComponents<TComponent>();
		}
		
		int
		NumComponents(Reflection::Type const& a_type) const;
		
		int
		NumComponents(Reflection::TypeId a_typeId) const;
		#pragma endregion

	private:
		EntityId m_entityId = EntityId::Null;
	};
}
