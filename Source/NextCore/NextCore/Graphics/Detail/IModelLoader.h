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
		/**
		 * \brief Load a model from the given file.
		 * \param a_filename The name of the file being read in.
		 * \param a_fileStream A file stream to the file
		 * \param a_container The container to populate with meshes
		 * \return True on success, false on failure
		 */
		virtual
		bool
		LoadFromFile(std::string_view a_filename, model_file_stream_t& a_fileStream, mesh_container_t& a_container) = 0;
	};
}
