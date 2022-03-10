#pragma once
#include "Reflection/Reflection.h"

namespace Next
{
	class Scene
	{
		ReflectDeclare(Scene)
	
	protected:
		Scene() = default;
	public:
		virtual
		~Scene() = default;

		virtual
		void
		OnSceneCreate() {}

		virtual
		void
		OnSceneDestroy() {}
	};
}
