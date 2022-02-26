#pragma once

#include <cstdint>
#include <map>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>

#include "Field.h"
#include "TypeTraits.h"
#include "Factory.h"
#include "TypeId.h"

#include "NextCoreCommon.h"

#define DEPRECATED_WARNING_NUMBER 4996

namespace Next::Reflection
{
	/**
	 * \brief A runtime structure that represents a reflected data type.
	 *        Contains information about member fields, functions, and other type information. 
	 */
	class Type
	{
		using types_container_t = std::unordered_map<TypeId, class Type>;

		static types_container_t& Types()
		{			
			static types_container_t container {};
			return container;
		}

		explicit
		Type(std::type_info const& a_info) noexcept
			: name(a_info.name()), m_typeInfo(&a_info) {}

		Type(Type const& a_other) = default;

		Type&
		operator =(Type const&) = delete;
		
	public:
		// Warning because move constructor is public?
		#pragma warning(disable : DEPRECATED_WARNING_NUMBER)
		Type(Type&& a_other) = default;
		Type&
		operator =(Type&&) = default;
		#pragma warning(disable : DEPRECATED_WARNING_NUMBER)
		
		/**
		 * \brief Retrieve the statically assigned type id for the type represented by the current \link Type \endlink.
		 * \return The non-zero static type id if the current type is valid, zero otherwise.
		 */
		TypeId
		GetTypeId() const
		{
			return m_typeId;
		}
		
		/**
		 * \brief Retrieve the statically assigned type id for the base type of the type represented
		 *        by the current \link Type \endlink.
		 * \return The non-zero static type id if the current type is valid, zero otherwise.
		 */
		TypeId
		GetBaseTypeId() const
		{
			return m_baseTypeId;
		}

		bool
		IsConvertibleTo(TypeId a_typeId) const;
		
		bool
		IsConvertibleFrom(TypeId a_typeId) const;

		/**
		 * \brief Retrieve the value returned by sizeof for the type class represented by
		 *        the current \link Type \endlink.
		 * \return The size of the type.
		 */
		int
		GetSize() const
		{
			return m_size;
		}

		std::type_info const&
		GetTypeInfo() const
		{
			return *m_typeInfo;
		}
		
		#pragma warning(disable : DEPRECATED_WARNING_NUMBER)
		// ReSharper disable once CppDeprecatedEntity
		/**
		 * \brief Get the instance of the \link GenericFactory \endlink associated with this type.
		 * \return A pointer to the \link GenericFactory \endlink associated with this type.
		 */
		const GenericFactory* GetConstructor()
		{
			// See https://en.cppreference.com/w/cpp/types/aligned_storage
			auto pConstructor = std::launder(reinterpret_cast<GenericFactory*>(m_factoryData));
			return pConstructor;
		}
		#pragma warning(disable : DEPRECATED_WARNING_NUMBER)

		/**
		 * \brief Register a type for reflection
		 * \tparam T The type to register for reflection.
		 *           Must contain reflection macros, see REFLECT_PREPARE and #REFLECT_MEMBERS
		 * \return An iterator for the container of Reflectors pointing to the newly registered element
		 */
		template<class T>
		static
		types_container_t::iterator
		Register() noexcept
		{
			TypeId id = Reflection::GetTypeId<T>();

			auto& types = Types();
			
			if (auto iter = types.find(id); iter != types.end())
			{
				return iter;
			}

			Type type = Type::Reflect<T>();
			auto iter = types.emplace(id, std::move(type)).first;
			iter->second.m_typeId = id;
			iter->second.m_size = sizeof(T);
			
			return iter;
		}
		
		/**
		 * \brief Get a lazy-initialized \link Type \endlink representing T.
		 * \tparam T The type to retrieve the \link Type \endlink for.
		 * \return A \link Type \endlink for the given type.
		 * \remarks Registering new elements can invalidate this reference. If lazy-initializing Reflectors,
		 *          references and pointers should not be maintained beyond function scope. Lookup is O(1) time complexity,
		 *          so performance should not be a concern for retrieving the Reflector.
		 */
		template<class T>
		static
		Type&
		Get() noexcept
		{
			TypeId id = Reflection::GetTypeId<T>();

			auto iter = Types().find(id);
			if (iter == Types().end())
			{
				iter = Register<T>();
			}
			return iter->second;
		}

		/**
		 * \brief Get a pointer to a \link Type \endlink representing T if it has already been registered.
		 * \tparam T The type to retrieve the \link Type \endlink for.
		 * \return A pointer to the \link Type \endlink for T if it has been registered, otherwise nullptr.
		 * \remarks Registering new elements can invalidate this pointer. If lazy-initializing Reflectors,
		 *          references and pointers should not be maintained beyond function scope. Lookup is O(1) time complexity,
		 *          so performance should not be a concern for retrieving the Reflector.
		 */
		template<class T>
		static
		Type*
		TryGet() noexcept
		{
			TypeId id = Reflection::GetTypeId<T>();
			
			return TryGet(id);
		}

		/**
		 * \brief Get a pointer to a \link Type \endlink representing T if it has already been registered.
		 * \param a_id The static id of the \link Type \endlink to get.
		 * \return A pointer to the \link Type \endlink for T if it has been registered, otherwise nullptr.
		 * \remarks Registering new elements can invalidate this pointer. If lazy-initializing Reflectors,
		 *          references and pointers should not be maintained beyond function scope. Lookup is O(1) time complexity,
		 *          so performance should not be a concern for retrieving the Reflector.
		 */
		static
		Type*
		TryGet(TypeId a_id) noexcept
		{
			Type* result = nullptr;
			if (auto iter = Types().find(a_id); iter != Types().end())
			{
				result = &iter->second;
			}

			return result;
		}
		
		static
		types_container_t&
		Enumerate()
		{
			return Types();
		}

		// Used by REFLECT_MEMBERS
		inline
		Type&
		operator()(Field&& a_field) noexcept
		{
			ReflectInternal(std::move(a_field));

			return *this;
		}

		// Used by REFLECT_DECLARE if reflecting the base class
		template<typename TBase, typename TDerived>
		inline
		Type&
		operator()() noexcept
		{
			static_assert(std::is_base_of_v<TBase, TDerived>, "Second argument of REFLECT_DECLARE MUST be derived from the first!");

			// Don't need to use TryGet here because this function can only be called if there is a base class
			auto& baseType = Get<TBase>();

			m_baseTypeId = baseType.GetTypeId();
			
			if (!baseType.instanceFields.empty())
			{
				auto& baseFields = baseType.instanceFields;
				
				instanceFields.insert(baseFields.begin(), baseFields.end());
			}

			return *this;
		}

	public:
		std::string name;

		std::map<std::string, Field> instanceFields;

	private:
		// We need this helper because you can't partially specialize functions to make sfinae work in
		// the way we want it to here, so do the reflection work in the static reflect function
		template<typename TReflected, typename = void>
		struct reflect_helper
		{
			static Type Reflect()
			{
				Type reflector(typeid(TReflected));
				
				PopulateFactory<TReflected>(reflector);

				return reflector;
			}
		};
		
		template<typename TReflected>
		struct reflect_helper<TReflected, std::void_t<decltype(sizeof(&TReflected::_Reflect))>>
		{
			static Type Reflect()
			{
				// Reflect type T and get information on members
				Type reflector(typeid(TReflected));
				
				TReflected::_Reflect(reflector);
				
				PopulateFactory<TReflected>(reflector);
				
				return reflector;
			}
		};
		
		template<typename TReflected>
		static
		void
		PopulateFactory(Type& a_type)
		{
			// Generate the constructor/destructor and use placement new under the hood
			// We use vtable shenanigans to make this work, so the data represented
			// by reflector.m_constructorData is stored "on the stack" inside this type while
			// being able to reference it like a pointer and use the vtable to call overridden
			// versions of Construct and Destruct
			#pragma warning(disable : DEPRECATED_WARNING_NUMBER)
			auto pConstructor = reinterpret_cast<GenericFactory*>(a_type.m_factoryData);
			// ReSharper disable once CppDeprecatedEntity
			new(pConstructor) TypedFactory<TReflected>;
			#pragma warning(disable : DEPRECATED_WARNING_NUMBER)
		}
		
		// Called by Type::Register
		template<class TReflected>
		static
		Type
		Reflect() noexcept
		{
			return reflect_helper<TReflected>::Reflect();
		}

		void
		ReflectInternal(Field&& a_fieldInfo) noexcept
		{
			instanceFields.emplace(a_fieldInfo.displayName, a_fieldInfo);
		}

	private:
		TypeId m_typeId = TypeId::Null;

		TypeId m_baseTypeId = TypeId::Null;

		std::type_info const* m_typeInfo;

		//std::aligned_storage_t<sizeof(GenericConstructor), alignof(GenericConstructor)> m_constructorData;
		std::byte m_factoryData[sizeof(GenericFactory)] { std::byte() };
		
		//GenericConstructor m_constructor;

		int m_size = 0;
	};
}

// Write-Only
static Next::Reflection::Name r_name;

// Write-Only
static Next::Reflection::Description r_description;

// Write-Only
static Next::Reflection::FieldFlags r_flags;

#undef DEPRECATED_WARNING_NUMBER
