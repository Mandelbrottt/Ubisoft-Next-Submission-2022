#pragma once

#include "NextCoreCommon.h"

#include "Components/ModelRenderer.h"

#include "Graphics/CubeMap.h"

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
	struct PrepareSceneDescriptor
	{
		Vector3 cameraPosition;
		Vector3 cameraForward;
		
		Matrix4 viewMatrix;
		Matrix4 projectionMatrix;

		CubeMap skybox;
	};

	/**
	 * \brief Prepare the scene for rendering.
	 * \remarks Must be called each frame during the Render function before the call to \link Flush \endlink.
	 */
	NEXT_CORE_EXPORT
	extern
	void
	PrepareScene(PrepareSceneDescriptor& a_descriptor);

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
