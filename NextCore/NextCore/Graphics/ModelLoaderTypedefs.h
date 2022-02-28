// This file serves to provide typedefs used by all aspects of the model loader without
// having to include ModelLoader.h everywhere

#pragma once

#include <fstream>
#include <vector>

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace Next
{
	class Mesh;
	
	struct ModelVertex
	{
		Vector3 position;
		Vector2 uv;
		Vector3 normal;
	};

	using model_file_stream_t = std::ifstream;

	using vertex_container_t       = std::vector<ModelVertex>;
	using vertex_count_container_t = std::vector<uint8_t>;

	using vertex_position_container_t = std::vector<Vector3>;
	using vertex_uv_container_t       = std::vector<Vector2>;
	using vertex_normal_container_t   = std::vector<Vector3>;
	
	using mesh_container_t         = std::vector<Mesh>;
}
