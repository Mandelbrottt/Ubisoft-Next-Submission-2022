#pragma once

#include "IModelLoader.h"

#include "Graphics/RenderPrimitive.h"

namespace Next::Detail
{
	class WavefrontModelLoader : public IModelLoader
	{
	public:
		bool
		LoadFromFile(std::string_view a_filename, file_stream_t& a_fileStream, mesh_container_t* a_container) override;

	private:
		void
		ProcessVertexPositionData(const char* a_line);

		void
		ProcessVertexUvData(const char* a_line);

		void
		ProcessVertexNormalData(const char* a_line);

		void
		ProcessFaceData(const char* a_line);
		
	private:
		std::vector<Vector3> m_positions;
		std::vector<Vector2> m_uvs;
		std::vector<Vector3> m_normals;

		vertex_container_t       m_vertices;
		vertex_count_container_t m_vertexCount;
	};
}
