#pragma once

#include <array>

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

		std::string filename;
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

		std::array<RenderPrimitive, 6> const&
		GetFaces() const
		{
			return m_faces;
		}

	private:
		void
		PopulateVertices();

	private:
		std::array<RenderPrimitive, 6> m_faces;
	};
}
