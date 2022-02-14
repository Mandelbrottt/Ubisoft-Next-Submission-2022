#pragma once

#include "Object.h"

namespace NextCore::Scripting
{
	/**
	 * \brief The base class for all components in NextCore
	 */
	class Component : public Object
	{
		friend class Entity;
	protected:
		Component() = default;
		
		~Component() override = default;
	public:
	};
}
