#pragma once

#include "Graphics/Model.h"

#include "Scripting/Component.h"

namespace Next
{
	// TODO: Come up with a better name that doesn't imply that this class renders the models itself
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
