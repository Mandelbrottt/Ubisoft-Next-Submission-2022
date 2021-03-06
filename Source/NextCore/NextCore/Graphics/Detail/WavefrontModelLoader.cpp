#include "pch.h"

#include "WavefrontModelLoader.h"

#include "Graphics/Mesh.h"

#include <filesystem>
#include <fstream>

namespace Next::Detail
{
	bool
	WavefrontModelLoader::LoadFromFile(
		std::string_view     a_filename,
		model_file_stream_t& a_fileStream,
		mesh_container_t&    a_container
	)
	{
		a_container.clear();
		
		std::string line;
		while (!a_fileStream.eof())
		{
			std::getline(a_fileStream, line);

			if (line.size() < 2)
			{
				continue;
			}

			auto index = line.find_first_not_of(" \t");

			if (index == std::string::npos)
				index = 0;

			const char* line_c_str = line.c_str() + index;
			
			char& firstCharacter = line[index];			
			char& secondCharacter = line[index + 1];

			// Comments
			if (firstCharacter == '#')
			{
				continue;
			}

			if (firstCharacter == 'v')                     // Vertex Information
			{
				if (secondCharacter == 't')                // UV Coordinates
				{
					ProcessVertexUvData(line_c_str);
				} else if (secondCharacter == 'n')         // Normal Vector
				{
					ProcessVertexNormalData(line_c_str);
				} else                                     // Vertex Position
				{
					ProcessVertexPositionData(line_c_str);
				}
			} else if (firstCharacter == 'f')              // Face Information
			{
				ProcessFaceData(line_c_str);
			}
		}

		// Load in the texture
		std::filesystem::path path = a_filename;
		path.replace_extension(".bmp");

		if (path.extension() == ".bmp")
		{
			// BMPs are flipped along the y axis by the texture loader - account for this
			for (auto& [position, uv, normal] : m_vertices)
			{
				uv.y = 1.0f - uv.y;
			}
		}

		// Generate the mesh
		Mesh mesh;
		bool result = mesh.GeneratePrimitives(path.string(), m_vertices, m_vertexCount);

		if (result)
		{
			// Add the mesh to the mesh container
			a_container.emplace_back(std::move(mesh));
		}
		
		return result;
	}

	void
	WavefrontModelLoader::ProcessVertexPositionData(const char* a_line)
	{
		const char* format = "v %f %f %f";
		Vector3 position;

		int result = sscanf_s(a_line, format, &position.x, &position.y, &position.z);

		if (result != position.size)
		{
			return;
		}

		m_positions.push_back(position);
	}

	void
	WavefrontModelLoader::ProcessVertexUvData(const char* a_line)
	{
		const char* format = "vt %f %f";
		Vector2 uv;

		int result = sscanf_s(a_line, format, &uv.x, &uv.y);

		if (result != uv.size)
		{
			return;
		}

		m_uvs.push_back(uv);
	}

	void
	WavefrontModelLoader::ProcessVertexNormalData(const char* a_line)
	{
		const char* format = "vn %f %f %f";
		Vector3 normal;

		int result = sscanf_s(a_line, format, &normal.x, &normal.y, &normal.z);

		if (result != normal.size)
		{
			return;
		}

		m_normals.push_back(normal);
	}

	void
	WavefrontModelLoader::ProcessFaceData(const char* a_line)
	{
		const char* format = "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d";

		Vector3i indices[4];
		
		// Holy arguments, batman!
		// Read in face data
		// ReSharper disable CppBadListLineBreaks
		auto numIndicesRead = sscanf_s(
			a_line, format,
			&indices[0].x, &indices[0].y, &indices[0].z,
			&indices[1].x, &indices[1].y, &indices[1].z,
			&indices[2].x, &indices[2].y, &indices[2].z,
			&indices[3].x, &indices[3].y, &indices[3].z
		);
		// ReSharper restore CppBadListLineBreaks
		
		const int numVertices = numIndicesRead / 3;
		for (int i = 0; i < numVertices; i++)
		{
			ModelVertex v;

			// Subtract 1 from all indices because obj is one-base indexed
			int positionIndex = indices[i].x - 1;
			v.position = m_positions[positionIndex];
			
			int uvIndex = indices[i].y - 1;
			v.uv = m_uvs[uvIndex];
			
			int normalIndex = indices[i].z - 1;
			v.normal = m_normals[normalIndex];
			
			m_vertices.push_back(v);
		}
		m_vertexCount.push_back(numVertices);
	}
}
