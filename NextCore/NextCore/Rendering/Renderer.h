#pragma once

#include "NextCoreCommon.h"

#include "Scripting/Entity.h"

#include "Math/Matrix4.h"

namespace Next
{
	class Model;
	class Mesh;
	class RenderPrimitive;
}

namespace Next::Renderer
{	
	NEXT_CORE_EXPORT
	extern
	void
    PrepareScene(Matrix4 const& a_viewMatrix, Matrix4 const& a_projectionMatrix);
	
	NEXT_CORE_EXPORT
	extern
	void
    Submit(Model const& a_model, Entity const& a_entityId);
	
	NEXT_CORE_EXPORT
	extern
	void
    Flush();

	// TODO: Add Submit overload for the component containing the model
}