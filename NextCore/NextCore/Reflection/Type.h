#pragma once

#include <cstdint>
#include <map>
#include <typeinfo>
#include <unordered_map>

#include "Field.h"
#include "TypeTraits.h"

namespace NextCore::Reflection
{
	struct Name
	{
		constexpr
		Name()
			: c_str("") {}

		constexpr
		Name(const char* a_value)
			: c_str(a_value) {}

		const char* c_str;
	};

	struct Description
	{
		constexpr
		Description()
			: c_str("") {}

		constexpr
		Description(const char* a_value)
			: c_str(a_value) {}

		const char* c_str;
	};

	class Type
	{
		static uint32_t s_staticIdCounter;

		using types_container_t = std::unordered_map<int, class Type>;

		static types_container_t& Types()
		{
			static types_container_t container;
			return container;
		}

		template<typename T/*, std::enable_if_t<std::is_default_constructible_v<T>, bool>*/>
		friend uint32_t GetStaticId() noexcept;

		explicit
		Type(std::string a_name) noexcept
			: name(a_name) {}

		Type(const Type& a_other) = default;

		Type&
		operator =(const Type&) = delete;

		Type&
		operator =(Type&&) = delete;

	public:
		Type(Type&& a_other) noexcept = default;

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
			int  id   = GetStaticId<T>();
			auto iter = Types().emplace(id, Type::Reflect<T>()).first;
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
			int id = GetStaticId<T>();

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
			int id = GetStaticId<T>();
			
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
		TryGet(decltype(s_staticIdCounter) a_id) noexcept
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

		inline
		Type&
		operator()(Field&& a_field) noexcept
		{
			ReflectInternal(std::move(a_field));

			return *this;
		}
		
		template<typename TBase, typename TDerived>
		inline
		Type&
		operator()() noexcept
		{
			static_assert(std::is_base_of_v<TBase, TDerived>, "Second argument of REFLECT_DECLARE MUST be derived from the first!");
			
			auto& baseType   = Get<TBase>();
			auto& baseFields = baseType.instanceFields;
			
			instanceFields.insert(baseFields.begin(), baseFields.end());

			return *this;
		}

	public:
		std::string name;

		std::map<std::string, Field> instanceFields;

	private:
		template<class T>
		static
		Type
		Reflect() noexcept
		{
			Type reflector(typeid(T).name());
			T::Reflect(reflector);
			return reflector;
		}

		void
		ReflectInternal(Field&& a_fieldInfo) noexcept
		{
			instanceFields.emplace(a_fieldInfo.fieldName, a_fieldInfo);
		}
	};

	template<typename T/*, std::enable_if_t<std::is_default_constructible_v<T>, bool> = true*/>
	uint32_t
	GetStaticId() noexcept
	{
		static uint32_t staticId = Type::s_staticIdCounter++;
		return staticId;
	}
}

// Write-Only
static NextCore::Reflection::Name r_name;

// Write-Only
static NextCore::Reflection::Description r_description;

// Write-Only
static NextCore::Reflection::FieldFlags r_flags;
