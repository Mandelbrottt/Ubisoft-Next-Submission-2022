#pragma once

#include "Graphics/ModelLoaderTypedefs.h"

#include <fstream>

namespace Next::Detail
{
	class IModelLoader
	{
	protected:
		IModelLoader()          = default;
		virtual ~IModelLoader() = default;

	public:
		virtual
		bool
		LoadFromFile(std::string_view a_filename, model_file_stream_t& a_fileStream, mesh_container_t* a_container) = 0;
	};
}
