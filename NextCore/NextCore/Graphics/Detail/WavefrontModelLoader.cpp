#include "pch.h"

#include "WavefrontModelLoader.h"

#include "Graphics/Mesh.h"

#include <filesystem>
#include <fstream>

namespace NextCore::Graphics::Detail
{
	using Math::Vector2;
	using Math::Vector3;

	bool
	WavefrontModelLoader::LoadFromFile(
		std::string_view  a_filename,
		file_stream_t&    a_fileStream,
		mesh_container_t* a_container
	)
	{
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

			if (firstCharacter == '#')
			{
				// Comments
				continue;
			}

			if (firstCharacter == 'v')
			{
				if (secondCharacter == 't')
				{
					ReadVertexUv(line_c_str);
				} else if (secondCharacter == 'n')
				{
					ReadVertexNormal(line_c_str);
				} else
				{
					ReadVertexPosition(line_c_str);
				}
			} else if (firstCharacter == 'f')
			{
				ReadFace(line_c_str);
			}
		}

		std::filesystem::path path = a_filename;
		path.replace_extension(".bmp");

		Mesh mesh;
		bool result = mesh.GeneratePrimitives(path.string(), m_vertices, m_primitiveType);

		if (result)
		{
			a_container->emplace_back(std::move(mesh));
		}
		
		return result;
	}

	void
	WavefrontModelLoader::ReadVertexPosition(const char* a_line)
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
	WavefrontModelLoader::ReadVertexUv(const char* a_line)
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
	WavefrontModelLoader::ReadVertexNormal(const char* a_line)
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
	WavefrontModelLoader::ReadFace(const char* a_line)
	{
		const char* format = "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d";

		Math::Vector3i indices[4];

		int runningIndex = 0;

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

		if (!PrimitiveTypeSanityCheck(numIndicesRead))
		{
			return;
		}

		const int numVertices = numIndicesRead / 3;
		for (int i = 0; i < numVertices; i++)
		{
			Vertex v;

			// Subtract 1 from all indices because obj is one-base indexed
			int positionIndex = indices[i].x - 1;
			v.position = m_positions[positionIndex];
			
			int uvIndex = indices[i].y - 1;
			v.uv = m_uvs[uvIndex];
			
			int normalIndex = indices[i].z - 1;
			v.normal = m_normals[normalIndex];
			
			m_vertices.push_back(v);
		}
	}

	bool
	WavefrontModelLoader::PrimitiveTypeSanityCheck(int a_numIndicesRead)
	{
		auto primitiveType = PrimitiveType::Null;
		
		if (a_numIndicesRead == 9)
		{
			primitiveType = PrimitiveType::Triangle;
		} else if (a_numIndicesRead == 12)
		{
			primitiveType = PrimitiveType::Quad;
		}

		if (m_primitiveType == PrimitiveType::Null)
		{
			m_primitiveType = primitiveType;
		}

		if (primitiveType == PrimitiveType::Null || primitiveType != m_primitiveType)
		{
			return false;
		}

		return true;
	}
}
