#pragma once

#include "Scripting/Component.h"

namespace NextCore::Scripting
{
	/**
	 * \brief The base class for all user behaviours in NextCore
	 */
	class Behaviour : public Component
	{
	protected:
		Behaviour() = default;

		~Behaviour() override = default;
	public:
	};
}
