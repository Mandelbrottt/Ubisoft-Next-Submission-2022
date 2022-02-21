#pragma once

#include "NextCoreCommon.h"

#include "Scripting/Entity.h"

#include "Math/Matrix4.h"

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
    PrepareScene(Math::Matrix4 const& a_viewMatrix, Math::Matrix4 const& a_projectionMatrix);
	
	NEXT_CORE_EXPORT
	extern
	void
    Submit(Graphics::Model const& a_model, Scripting::Entity const& a_entityId);
	
	NEXT_CORE_EXPORT
	extern
	void
    Flush();

	// TODO: Add Submit overload for the component containing the model
}