#pragma once

#include <string>

#include "Reflection/Reflection.h"

namespace Next
{
	/**
	 * \brief The root base class for all objects represented in NextCore (eg. Entities, Components, etc.)
	 */
	class Object
	{
		ReflectDeclare(Object)

	protected:
		Object() = default;
		
		virtual ~Object() = default;

		// Objects should only me referenced or moved, not copied
		Object(Object const& a_other) = delete;

		Object&
		operator =(Object const& a_other) = delete;
	public:
		Object(Object&& a_other) = default;
	};
}