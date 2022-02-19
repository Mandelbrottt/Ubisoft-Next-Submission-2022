// This file serves to provide typedefs used by all aspects of the model loader without
// having to include ModelLoader.h everywhere

#pragma once

#include <vector>

#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace NextCore::Graphics::Detail
{
	struct Vertex
	{
		Math::Vector3 positions;
		Math::Vector2 uvs;
		Math::Vector3 normals;
	};

	using vertex_container_t  = std::vector<Vertex>;
}
