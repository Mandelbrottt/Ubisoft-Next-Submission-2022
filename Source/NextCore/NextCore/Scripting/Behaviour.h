#pragma once

#include "Scripting/Component.h"

namespace Next
{
	/**
	 * \brief The base class for all user behaviours in NextCore
	 */
	class Behaviour : public Component
	{
		ReflectDeclare(Behaviour, Component)

	protected:
		Behaviour() = default;

		~Behaviour() override = default;
	
	public:
	};
}