#pragma once

#include "ModelLoaderTypedefs.h"

#include <fstream>

namespace NextCore::Graphics::Detail
{
	class ModelLoader
	{
	protected:
		ModelLoader()          = default;
		virtual ~ModelLoader() = default;

	public:
		using file_stream_t = std::ifstream;
		
		virtual
		bool
		LoadFromFile(std::string_view a_filename, file_stream_t& a_fileStream) = 0;

		vertex_container_t const&
		GetVertices() const
		{
			return m_vertices;
		}

	protected:
		vertex_container_t m_vertices;
	};
}
