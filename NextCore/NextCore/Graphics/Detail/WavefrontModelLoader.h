#pragma once

#include "IModelLoader.h"

#include "Graphics/Primitive.h"

namespace NextCore::Graphics::Detail
{
	class WavefrontModelLoader : public IModelLoader
	{
	public:
		bool
		LoadFromFile(std::string_view a_filename, file_stream_t& a_fileStream, mesh_container_t* a_container) override;

	private:
		void
		ReadVertexPosition(const char* a_line);

		void
		ReadVertexUv(const char* a_line);

		void
		ReadVertexNormal(const char* a_line);

		void
		ReadFace(const char* a_line);

		bool
		PrimitiveTypeSanityCheck(int a_numIndicesRead);

	private:
		std::vector<Math::Vector3> m_positions;
		std::vector<Math::Vector2> m_uvs;
		std::vector<Math::Vector3> m_normals;

		PrimitiveType m_primitiveType = PrimitiveType::Null;

		std::vector<Vertex> m_vertices;
	};
}
