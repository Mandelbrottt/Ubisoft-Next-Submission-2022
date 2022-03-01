#pragma once

#include <string_view>
#include <vector>

#include "RenderPrimitive.h"

#include "Detail/IModelLoader.h"

namespace Next
{
	class Mesh final
	{
		friend class Model;
		
	public:
		using primitive_container_t = std::vector<RenderPrimitive>;
		
		/**
		 * \brief Generate the list of \link RenderPrimitive's \endlink that make up the mesh.
		 * \param a_textureFilename The name of the texture file to use as the base texture for the primitives
		 * \param a_vertices The list of vertices that make up the mesh
		 * \param a_vertexCounts the list of how many vertices make up each primitive
		 * \return True on success, false otherwise.
		 * \remark Only intended to be called by the internal model loader. Only call if you know
		 *         what you're doing.
		 */
		bool
		GeneratePrimitives(
			std::string_view                a_textureFilename,
			vertex_container_t const&       a_vertices,
			vertex_count_container_t const& a_vertexCounts
		);

		primitive_container_t const&
		GetPrimitives() const
		{
			return m_primitives;
		}

	private:
		void
		OnUpdate();

	private:
		primitive_container_t m_primitives;
	};
}
