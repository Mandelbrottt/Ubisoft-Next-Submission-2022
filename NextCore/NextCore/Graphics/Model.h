#pragma once

#include "Mesh.h"

#include <string_view>

namespace Next
{
	class Model final : Object
	{
	public:
		using mesh_container_t = std::vector<Mesh>;
		
		/**
		 * \brief Loads a model from file, and automatically detects textures and sub-meshes if possible
		 * \param a_filename The path to the model file to be loaded
		 * \return True for success, false otherwise
		 * \remark Currently only supports wavefront .obj files
		 */
		bool
		LoadFromFile(std::string_view a_filename);
		
		mesh_container_t const&
		GetMeshes() const
		{
			return m_meshes;
		}

	private:
		mesh_container_t m_meshes;
	};
}
