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
	/**
	 * \brief Prepare the scene for rendering.
	 * \remarks Must be called each frame during the Render function before the call to \link Flush \endlink.
	 */
	NEXT_CORE_EXPORT
	extern
	void
    PrepareScene(Matrix4 const& a_viewMatrix, Matrix4 const& a_projectionMatrix);
	
	// TODO: Add Submit overload for the component containing the model
	/**
	 * \brief Submit a model for rendering at the given entity's Transform
	 * \param a_model The model to render
	 * \param a_entity The entity who's transform to use
	 */
	NEXT_CORE_EXPORT
	extern
	void
    Submit(Model const& a_model, Entity const& a_entity);
	
	/**
	 * \brief Flush the rasterization buffer and render all pending RenderPrimitives
	 */
	NEXT_CORE_EXPORT
	extern
	void
    Flush();
}