#pragma once

#include <cstdint>
#include <map>
#include <typeinfo>
#include <unordered_map>

#include "Field.h"
#include "TypeTraits.h"
#include "Constructor.h"

#include "NextCoreCommon.h"

#define DEPRECATED_WARNING_NUMBER 4996

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

	using static_type_id_underlying_t = uint32_t;

	enum class StaticTypeId : static_type_id_underlying_t { Null = 0, FirstValid = 1 };

	constexpr
	bool
	operator ==(StaticTypeId a_lhs, StaticTypeId a_rhs)
	{
		auto lhs = static_cast<static_type_id_underlying_t>(a_lhs);
		auto rhs = static_cast<static_type_id_underlying_t>(a_rhs);

		return lhs == rhs;
	}

	constexpr
	bool
	operator !=(StaticTypeId a_lhs, StaticTypeId a_rhs)
	{
		return a_lhs == a_rhs;
	}

	template<typename T>
	StaticTypeId
	GetStaticId() noexcept;
	
	class Type
	{
		using types_container_t = std::unordered_map<StaticTypeId, class Type>;

		static StaticTypeId& StaticIdCounter()
		{
			static StaticTypeId staticIdCounter { StaticTypeId::FirstValid };
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
		// Warning because move constructor is public?
		#pragma warning(disable : DEPRECATED_WARNING_NUMBER)
		Type(Type&& a_other) noexcept = default;
		//{
		//	this->instanceFields = std::move(a_other.instanceFields);

		//	this->name = std::move(a_other.name);

		//	this->m_typeId = a_other.m_typeId;
		//	a_other.m_typeId = StaticTypeId::Null;

		//	this->m_size = a_other.m_size;
		//	a_other.m_size = 0;
		//	
		//	std::memcpy(this->m_constructorData, a_other.m_constructorData, sizeof(m_constructorData));
		//	std::memset(a_other.m_constructorData, 0, sizeof(m_constructorData));
		//};
		#pragma warning(disable : DEPRECATED_WARNING_NUMBER)

		/**
		 * \brief Retrieve the statically assigned type id for the type represented by the current \link Type \endlink.
		 * \return The non-zero static type id if the current type is valid, zero otherwise.
		 */
		StaticTypeId
		GetStaticId() const
		{
			return m_typeId;
		}
		
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
		
		#pragma warning(disable : DEPRECATED_WARNING_NUMBER)
		// ReSharper disable once CppDeprecatedEntity
		/**
		 * \brief Get the instance of the \link GenericConstructor \endlink associated with this type.
		 * \return A pointer to the \link GenericConstructor \endlink associated with this type.
		 */
		const GenericConstructor* GetConstructor()
		{
			// See https://en.cppreference.com/w/cpp/types/aligned_storage
			auto pConstructor = std::launder(reinterpret_cast<GenericConstructor*>(m_constructorData));
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
			StaticTypeId id = Reflection::GetStaticId<T>();

			Type type = Type::Reflect<T>();
			auto iter = Types().emplace(id, std::move(type)).first;
			iter->second.m_typeId = id;
			iter->second.m_size = sizeof(T);
			
			GenericConstructor* pConstructor = std::launder(reinterpret_cast<GenericConstructor*>(iter->second.m_constructorData));

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
			StaticTypeId id = Reflection::GetStaticId<T>();

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
			StaticTypeId id = Reflection::GetStaticId<T>();
			
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
		TryGet(StaticTypeId a_id) noexcept
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
			T::_Reflect(reflector);

			#pragma warning(disable : DEPRECATED_WARNING_NUMBER)
			auto pConstructor = reinterpret_cast<GenericConstructor*>(reflector.m_constructorData);
			// ReSharper disable once CppDeprecatedEntity
			T::_GetGenericConstructor(pConstructor);
			#pragma warning(disable : DEPRECATED_WARNING_NUMBER)

			pConstructor = std::launder(reinterpret_cast<GenericConstructor*>(reflector.m_constructorData));

			return reflector;
		}

		void
		ReflectInternal(Field&& a_fieldInfo) noexcept
		{
			instanceFields.emplace(a_fieldInfo.fieldName, a_fieldInfo);
		}

	private:
		StaticTypeId m_typeId = StaticTypeId::Null;

		//std::aligned_storage_t<sizeof(GenericConstructor), alignof(GenericConstructor)> m_constructorData;
		std::byte m_constructorData[sizeof(GenericConstructor)] { std::byte() };
		
		//GenericConstructor m_constructor;

		int m_size = 0;
		
	private:
		template<typename T>
		friend
		StaticTypeId
		GetStaticId() noexcept;
	};

	namespace Detail
	{
		// We need this helper because you can't partially specialize functions to make sfinae work in
		// the way we want it to here, so pass in a reference to the static type id and increment it here
		template<typename T, typename = void>
		struct static_id_helper
		{
			static StaticTypeId Increment(StaticTypeId& a_id)
			{
				return StaticTypeId::Null;
			}
		};
		
		template<typename T>
		struct static_id_helper<T, std::void_t<decltype(sizeof(T::_REFLECT_VALID_REFLECTION_TYPE_ALIAS))>>
		{
			static StaticTypeId Increment(StaticTypeId& a_id)
			{
				// Emulate pre-increment because id doesn't start at 0
				auto result = a_id;

				// Can't cast to reference and pre-increment, so write to temp var and reassign
				auto underlying_id = static_cast<static_type_id_underlying_t>(a_id);
				underlying_id++;

				a_id = static_cast<StaticTypeId>(underlying_id);
				
				return result;
			}
		};
	}
	
	/**
	 * \brief  Retrieve the statically assigned type id for the given type
	 * \tparam T The type who's Id to retrieve
	 * \return A non-zero type id for a valid reflection type, zero otherwise
	 */
	template<typename T>
	StaticTypeId
	GetStaticId() noexcept
	{
		static StaticTypeId result = [&]()
		{
			auto& static_id_counter = Type::StaticIdCounter();
			return Detail::static_id_helper<T>::Increment(static_id_counter);
		}();

		return result;
	}
}

// Write-Only
static NextCore::Reflection::Name r_name;

// Write-Only
static NextCore::Reflection::Description r_description;

// Write-Only
static NextCore::Reflection::FieldFlags r_flags;

#undef DEPRECATED_WARNING_NUMBER
