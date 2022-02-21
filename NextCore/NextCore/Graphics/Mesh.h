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
		
		primitive_container_t const&
		GetPrimitives() const
		{
			return m_primitives;
		}

	private:
		primitive_container_t m_primitives;
	};
}
