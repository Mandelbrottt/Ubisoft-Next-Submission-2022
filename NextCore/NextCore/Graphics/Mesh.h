#pragma once

#include <vector>

#include "RenderPrimitive.h"

#include <string_view>

#include "Detail/IModelLoader.h"

namespace Next
{
	class Mesh final
	{
	public:
		using primitive_container_t = std::vector<RenderPrimitive>;

		bool
		GeneratePrimitives(
			std::string_view                        a_textureFilename,
			Detail::vertex_container_t const&       a_vertices,
			Detail::vertex_count_container_t const& a_vertexCounts
		);

		void
		OnUpdate();
		
		primitive_container_t const&
		GetPrimitives() const
		{
			return m_primitives;
		}

	private:
		primitive_container_t m_primitives;
	};
}
