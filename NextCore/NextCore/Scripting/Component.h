#pragma once

#include "Object.h"
#include "Entity.h"

#include "Reflection/Reflection.h"

#define GenerateConstructors(_class_) \
		_MACRO_AUTO_FORMAT_INDENT \
	protected: \
		friend ::Next::Reflection::TypedFactory<_class_>; \
		friend ::Next::Reflection::Detail::typed_factory_friend_helper; \
		\
		_class_() \
			: Base({ #_class_ }) { } \
		~_class_() override = default;

namespace Next::Reflection
{
	template<typename T, bool IsConstructible>
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
	protected:
		struct ComponentConstructionArgs : ObjectConstructionArgs { };

		explicit
		Component(ComponentConstructionArgs const& a_args);

		~Component() override = default;

	public:
		#pragma region Getters and Setters
		EntityId
		GetEntityId() const;

		Entity*
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
			return m_entity->AddComponent<TComponent>();
		}

		Component*
		AddComponent(Reflection::Type const& a_type);

		Component*
		AddComponent(Reflection::TypeId a_typeId);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		bool
		RemoveComponent()
		{
			return m_entity->RemoveComponent<TComponent>();
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
			return m_entity->GetComponent<TComponent>();
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
		 * \param a_outCount 
		 * \return 
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent**
		GetComponents(int* a_outCount)
		{
			return m_entity->GetComponents<TComponent>(a_outCount);
		}

		/**
		 * \brief 
		 * \param a_type 
		 * \param a_outCount 
		 * \return 
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		Component**
		GetComponents(Reflection::Type const& a_type, int* a_outCount);

		/**
		 * \brief 
		 * \param a_typeId 
		 * \param a_outCount 
		 * \return 
		 * \remark It is the callers responsibility to call delete[] on the array.
		 */
		Component**
		GetComponents(Reflection::TypeId a_typeId, int* a_outCount);

		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		TComponent**
		GetComponents(int* a_outCount) const
		{
			return const_cast<Component*>(this)->GetComponents<TComponent>(a_outCount);
		}

		Component**
		GetComponents(Reflection::Type const& a_type, int* a_outCount) const
		{
			return const_cast<Component*>(this)->GetComponents(a_type, a_outCount);
		}
		
		Component**
		GetComponents(Reflection::TypeId a_typeId, int* a_outCount) const
		{
			return const_cast<Component*>(this)->GetComponents(a_typeId, a_outCount);
		}
		
		template<typename TComponent, std::enable_if_t<std::is_convertible_v<TComponent*, Component*>, bool> = true>
		int
		NumComponents() const
		{
			return m_entity->NumComponents<TComponent>();
		}
		
		int
		NumComponents(Reflection::Type const& a_type) const;
		
		int
		NumComponents(Reflection::TypeId a_typeId) const;
		#pragma endregion

	private:
		EntityId m_entityId = EntityId::Null;

		Entity* m_entity = nullptr;
	};
}
