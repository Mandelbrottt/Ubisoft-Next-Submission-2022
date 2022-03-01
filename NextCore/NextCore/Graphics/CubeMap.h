#pragma once

class CSimpleSprite;

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
		Left,
		Right,
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

	private:
		CSimpleSprite* m_faces[6];
	};
}
