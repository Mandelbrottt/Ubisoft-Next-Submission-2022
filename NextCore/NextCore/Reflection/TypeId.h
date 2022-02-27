#pragma once

#include <cstdint>

namespace Next::Reflection
{
	using type_id_underlying_t = uint32_t;

	enum class TypeId : type_id_underlying_t { Null = 0, FirstValid = 1 };

	constexpr
	bool
	operator ==(TypeId a_lhs, TypeId a_rhs)
	{
		auto lhs = static_cast<type_id_underlying_t>(a_lhs);
		auto rhs = static_cast<type_id_underlying_t>(a_rhs);

		return lhs == rhs;
	}

	constexpr
	bool
	operator !=(TypeId a_lhs, TypeId a_rhs)
	{
		return !(a_lhs == a_rhs);
	}
	
	namespace Detail
	{
		struct static_type_id_helper
		{
			template<typename T>
			friend
			TypeId
			GetRawTypeId() noexcept;

		private:
			static TypeId& StaticIdCounter()
			{
				// Maintain a local static variable to control order of initialization
				// A static in a translation unit wouldn't have a guaranteed construction order
				static TypeId staticIdCounter { TypeId::FirstValid };
				return staticIdCounter;
			}
		};

		template<typename T>
		static
		TypeId
		GetRawTypeId() noexcept
		{
			static TypeId result = []()
			{
				auto& static_id_counter = static_type_id_helper::StaticIdCounter();
				
				// Emulate pre-increment because id doesn't start at 0
				auto innerResult = static_id_counter;

				// Can't cast to reference and pre-increment, so write to temp var and reassign
				auto underlying_id = static_cast<type_id_underlying_t>(static_id_counter);
				underlying_id++;

				static_id_counter = static_cast<TypeId>(underlying_id);
				
				return innerResult;
			}();

			return result;
		}
	}
	
	/**
	 * \brief  Retrieve the statically assigned type id for the given type
	 * \tparam T The type who's Id to retrieve
	 * \return A non-zero type id for all types
	 * \remark returns the same TypeId for two types that decay to the same type.
	 *         ie. GetTypeId<const int> and GetTypeId<int> return the same id.
	 */
	template<typename T>
	TypeId
	GetTypeId() noexcept
	{
		typedef std::remove_pointer_t<std::decay_t<T>> raw_t;
		return Detail::GetRawTypeId<raw_t>();
	}
}
