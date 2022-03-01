#include "pch.h"
#include "CubeMap.h"

#include <filesystem>

namespace Next
{
	bool
	CubeMap::LoadFromFiles(CubeMapFaceDescriptor a_descriptors[6])
	{
		for (int i = 0; i < 6; i++)
		{
			auto const& descriptor = a_descriptors[i];
			if (std::filesystem::exists(descriptor.filename))
			{
				return false;
			}
		}
		
		for (int i = 0; i < 6; i++)
		{
			auto& [direction, filename] = a_descriptors[i];
			
			auto* sprite = new CSimpleSprite(filename.data());

			auto index = static_cast<uint8_t>(direction);

			m_faces[index] = sprite;
		}

		return true;
	}
}
