#pragma once

#include "Collider.h"

namespace Next
{
	class SphereCollider : public Collider
	{
		ComponentDeclare(SphereCollider, Collider)
			
	public:
		float radius = 1;
	
	private:
		ReflectMembers(
			ReflectField(radius)
		)
	};
}
