#pragma once

#include "ModelLoaderTypedefs.h"

#include "Graphics/Mesh.h"

#include <fstream>

namespace NextCore::Graphics::Detail
{
	using file_stream_t = std::ifstream;

	class IModelLoader
	{
	protected:
		IModelLoader()          = default;
		virtual ~IModelLoader() = default;

	public:
		virtual
		bool
		LoadFromFile(std::string_view a_filename, file_stream_t& a_fileStream, mesh_container_t* a_container) = 0;
	};
}