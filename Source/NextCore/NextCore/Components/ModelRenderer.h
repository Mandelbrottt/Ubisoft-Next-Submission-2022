#pragma once

#include "Graphics/Model.h"

#include "Scripting/Component.h"

namespace Next
{
	/**
	 * \brief Represents a model to be rendered in the game world, and the parameters with which to render it.
	 */
	class ModelRenderer : public Component
	{
		ReflectDeclare(ModelRenderer, Component)

	public:
		Model* model;
		
		ReflectMembers(
			ReflectField(model)
		)
	};
}
