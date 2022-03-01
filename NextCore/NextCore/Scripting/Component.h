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
		#pragma endregion

	private:
		EntityId m_entityId = EntityId::Null;
	};
}
