#pragma once

#include "NextCoreCommon.h"

#include "Components/ModelRenderer.h"

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
	
	/**
	 * \brief Submit a ModelRenderer for rendering at the given Transform
	 * \param a_modelRenderer The ModelRenderer to render
	 * \param a_transform The transform to use
	 */
	NEXT_CORE_EXPORT
	extern
	void
    Submit(ModelRenderer const* a_modelRenderer, Transform const* a_transform);
	
	/**
	 * \brief Flush the rasterization buffer and render all pending RenderPrimitives
	 */
	NEXT_CORE_EXPORT
	extern
	void
    Flush();
}