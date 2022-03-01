#pragma once

#include "RenderPrimitive.h"

namespace Next
{
	namespace Renderer
	{
		void
		Flush();
	}

	enum class CubeMapFaceDirection : std::uint8_t
	{
		Front = 0,
		Back,
		Top,
		Bottom,
		Right,
		Left,
	};

	struct CubeMapFaceDescriptor
	{
		CubeMapFaceDirection direction;

		std::string_view filename;
	};

	class CubeMap final
	{
		friend
		void
		Renderer::Flush();

	public:
		bool
		LoadFromFiles(CubeMapFaceDescriptor a_descriptors[6]);

		bool
		IsValid() const
		{
			for (auto const& primitive : m_faces)
			{
				if (!primitive.IsValid())
				{
					return false;
				}
			}

			return true;
		}

	private:
		void
		PopulateVertices();

	private:
		RenderPrimitive m_faces[6];
	};
}
