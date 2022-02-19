#pragma once

#include <vector>

#include "Primitive.h"

#include <string_view>

#include "Detail/IModelLoader.h"

namespace NextCore::Graphics
{
	class Mesh final
	{
	public:
		using primitive_container_t = std::vector<Primitive>;

		bool
		GeneratePrimitives(
			std::string_view                  a_textureFilename,
			Detail::vertex_container_t const& a_vertices,
			PrimitiveType                     a_type
		);

		void
		OnUpdate();

		void
		OnRenderPrepare(Math::Matrix4 const& a_model, Math::Matrix4 const& a_view, Math::Matrix4 const& a_projection);

		// There is no OnRender function here because the renderer collects all of the primitives,
		// sorts them, and then renders them directly

		primitive_container_t const&
		GetPrimitives() const
		{
			return m_primitives;
		}

	private:
		primitive_container_t m_primitives;
	};
}
