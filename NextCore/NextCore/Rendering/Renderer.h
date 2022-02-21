#pragma once

#include "NextCoreCommon.h"

#include "Scripting/Entity.h"

namespace NextCore::Graphics
{
	class Model;
	class Mesh;
	class Primitive;
}

namespace NextCore::Renderer
{	
	NEXT_CORE_EXPORT
	extern
	void
    Submit(Graphics::Model const& a_model, Scripting::Entity a_entityId);

	// TODO: Add Submit overload for the component containing the model
}