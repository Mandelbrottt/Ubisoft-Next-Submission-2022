#pragma once

#include <cstdint>

namespace NextCore::Scripting
{
	using entity_underlying_t = uint32_t;
	
	/**
	 * \brief Opaque type representing the ID of an entity in the game world
	 */
	enum class EntityId : entity_underlying_t { Null = 0, FirstValid = 1 };
	
	inline
	bool
	operator ==(EntityId a_lhs, EntityId a_rhs)
	{
		entity_underlying_t lhs = static_cast<entity_underlying_t>(a_lhs);
		entity_underlying_t rhs = static_cast<entity_underlying_t>(a_rhs);
		return lhs == rhs;
	}
	
	inline
	bool
	operator !=(EntityId a_lhs, EntityId a_rhs)
	{
		return !(a_lhs == a_rhs);
	}
}