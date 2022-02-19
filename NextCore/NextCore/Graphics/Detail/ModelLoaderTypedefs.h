// This file serves to provide typedefs used by all aspects of the model loader without
// having to include ModelLoader.h everywhere

#pragma once

#include <vector>

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace NextCore::Graphics
{
	class Mesh;
}

namespace NextCore::Graphics::Detail
{
	struct Vertex
	{
		Math::Vector3 position;
		Math::Vector2 uv;
		Math::Vector3 normal;
	};

	using vertex_container_t = std::vector<Vertex>;
	using mesh_container_t   = std::vector<Mesh>;
}
