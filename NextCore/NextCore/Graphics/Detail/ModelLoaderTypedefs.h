// This file serves to provide typedefs used by all aspects of the model loader without
// having to include ModelLoader.h everywhere

#pragma once

#include <vector>

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace Next
{
	class Mesh;

	namespace Detail
	{
		struct Vertex
		{
			Vector3 position;
			Vector2 uv;
			Vector3 normal;
		};

		using vertex_container_t = std::vector<Vertex>;
		using mesh_container_t = std::vector<Mesh>;
	}
}
