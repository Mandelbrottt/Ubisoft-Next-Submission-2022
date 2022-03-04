#pragma once

#include "IModelLoader.h"

namespace Next::Detail
{
	class WavefrontModelLoader : public IModelLoader
	{
	public:
		bool
		LoadFromFile(
			std::string_view     a_filename,
			model_file_stream_t& a_fileStream,
			mesh_container_t&    a_container
		) override;

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
		vertex_position_container_t m_positions;
		vertex_uv_container_t       m_uvs;
		vertex_normal_container_t   m_normals;

		vertex_container_t       m_vertices;
		vertex_count_container_t m_vertexCount;
	};
}
