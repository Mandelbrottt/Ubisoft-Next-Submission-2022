#pragma once

#include <cstdint>

namespace Next
{
	using entity_id_underlying_t = uint32_t;
	
	/**
	 * \brief Opaque type representing the ID of an entity in the game world
	 */
	enum class EntityId : entity_id_underlying_t { Null = 0, FirstValid = 1 };
	
	inline
	bool
	operator ==(EntityId a_lhs, EntityId a_rhs)
	{
		entity_id_underlying_t lhs = static_cast<entity_id_underlying_t>(a_lhs);
		entity_id_underlying_t rhs = static_cast<entity_id_underlying_t>(a_rhs);
		return lhs == rhs;
	}
	
	inline
	bool
	operator !=(EntityId a_lhs, EntityId a_rhs)
	{
		return !(a_lhs == a_rhs);
	}
}