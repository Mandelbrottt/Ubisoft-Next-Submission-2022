#pragma once

#include <Scripting/Component.h>

namespace Next
{
	class Collider : public Component
	{
		ReflectDeclare(Collider, Component)

	protected:
		Collider() = default;
		
		~Collider() override {};
	};
}
