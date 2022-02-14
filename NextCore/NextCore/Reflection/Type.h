#pragma once

#include <cstdint>
#include <map>
#include <typeinfo>
#include <unordered_map>

#include "Field.h"
#include "TypeTraits.h"

#include "Common.h"

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

	using static_type_id_t = uint32_t;

	constexpr static_type_id_t INVALID_TYPE_ID     = 0;
	constexpr static_type_id_t FIRST_VALID_TYPE_ID = 1;

	template<typename T>
	static_type_id_t
	GetStaticId() noexcept;
	
	class Type
	{
		using types_container_t = std::unordered_map<int, class Type>;

		static static_type_id_t& StaticIdCounter()
		{
			static static_type_id_t staticIdCounter { FIRST_VALID_TYPE_ID };
			return staticIdCounter;
		}

		static types_container_t& Types()
		{
			static types_container_t container;
			return container;
		}

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
		 * \brief Retrieve the statically assigned type id for the class represented by the current \link Type \endlink
		 * \return The non-zero static type id if the current type is valid, zero otherwise
		 */
		static_type_id_t
		GetStaticId() const
		{
			return m_typeId;
		}

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
			int  id = Reflection::GetStaticId<T>();
			auto iter = Types().emplace(id, Type::Reflect<T>()).first;
			iter->second.m_typeId = id;
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
			int id = Reflection::GetStaticId<T>();

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
			int id = Reflection::GetStaticId<T>();
			
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
		TryGet(static_type_id_t a_id) noexcept
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

	private:
		static_type_id_t m_typeId = INVALID_TYPE_ID;

	private:
		template<typename T>
		friend
		static_type_id_t
		GetStaticId() noexcept;
	};

	namespace Detail
	{
		// We need this helper because you can't partially specialize functions to make sfinae work in
		// the way we want it to here, so pass in a reference to the static type id and increment it here
		template<typename T, typename = void>
		struct static_id_helper
		{
			static static_type_id_t Increment(static_type_id_t& a_id)
			{
				return 0;
			}
		};
		
		template<typename T>
		struct static_id_helper<T, std::void_t<decltype(sizeof(T::_REFLECT_VALID_REFLECTION_TYPE_ALIAS))>>
		{
			static static_type_id_t Increment(static_type_id_t& a_id)
			{
				return a_id++;
			}
		};
	}
	
	/**
	 * \brief  Retrieve the statically assigned type id for the given type
	 * \tparam T The type who's Id to retrieve
	 * \return A non-zero type id for a valid reflection type, zero otherwise
	 */
	template<typename T>
	static_type_id_t
	GetStaticId() noexcept
	{
		auto& static_id_counter = Type::StaticIdCounter();
		return Detail::static_id_helper<T>::Increment(static_id_counter);
	}
}

// Write-Only
static NextCore::Reflection::Name r_name;

// Write-Only
static NextCore::Reflection::Description r_description;

// Write-Only
static NextCore::Reflection::FieldFlags r_flags;
