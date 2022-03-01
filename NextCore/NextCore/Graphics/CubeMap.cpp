#include "pch.h"
#include "CubeMap.h"

#include <filesystem>

namespace Next
{
	bool
	CubeMap::LoadFromFiles(CubeMapFaceDescriptor a_descriptors[6])
	{
		// Check if all of the files exist on disc
		for (int i = 0; i < 6; i++)
		{
			auto const& descriptor = a_descriptors[i];
			if (!std::filesystem::exists(descriptor.filename))
			{
				return false;
			}
		}

		// Load the textures
		for (int i = 0; i < 6; i++)
		{
			auto& [direction, filename] = a_descriptors[i];

			RenderPrimitive primitive { filename };

			auto index = static_cast<uint8_t>(direction);

			m_faces[index] = std::move(primitive);
		}

		PopulateVertices();

		return true;
	}

	void
	CubeMap::PopulateVertices()
	{
		// Declare same order as CubeMapFaceDirection is declared
		Vector3 directions[6] = {
			Vector3::Forward(),
			Vector3::Backward(),
			Vector3::Up(),
			Vector3::Down(),
			Vector3::Right(),
			Vector3::Left(),
		};

		constexpr auto face_direction_cast = [](uint8_t a_index)
		{
			return static_cast<CubeMapFaceDirection>(a_index);
		};

		for (int i = 0; i < 6; i++)
		{
			auto& face = m_faces[i];

			for (int j = 0; j < 4; j++)
			{
				auto& [position, uv, normal] = face.m_vertices[j];

				float x = j == 0 || j == 3 ? -1.0f : 1.0f;
				float y = j == 0 || j == 1 ? -1.0f : 1.0f;

				auto faceDirection = face_direction_cast(i);
				switch (faceDirection)
				{
					case CubeMapFaceDirection::Front:  position = {  x,  y,  1 };  break;
					case CubeMapFaceDirection::Back:   position = {  x,  y, -1 }; break;
					case CubeMapFaceDirection::Top:    position = {  y,  1,  x };  break;
					case CubeMapFaceDirection::Bottom: position = {  y, -1,  x }; break;
					case CubeMapFaceDirection::Right:  position = {  1,  x,  y };  break;
					case CubeMapFaceDirection::Left:   position = { -1,  x,  y }; break;
				}
				
				uv       = { x * 0.5f + 0.5f, y * 0.5f + 0.5f };
				normal   = -directions[i];
			}

			face.m_primitiveType = RenderPrimitiveType::Quad;
		}
	}
}
