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
		struct BehaviourConstructionArgs : ComponentConstructionArgs { };

		explicit
		Behaviour(BehaviourConstructionArgs const& a_args)
			: Component(a_args) {}

		~Behaviour() override = default;
	public:
	};
}
