#pragma once

// Keep to avoid errors when included
#include <string_view>

#include "Detail/ModelLoaderTypedefs.h"

#include "Math/Vector.h"
#include "Math/Matrix4.h"

class CSimpleSprite;

namespace NextCore::Graphics
{
	enum class PrimitiveType
	{
		// TODO: Add support for triangles
		Null = 0,
		Triangle,
		Quad,
	};
	
	class Primitive final
	{
		friend class Mesh;
		
		Primitive() = default;
	public:
		~Primitive() = default;
		
		Primitive(Primitive const& a_other) = delete;
		Primitive& 
		operator =(Primitive const& a_other) = delete;

		Primitive(Primitive&& a_other) = default;
		
		void
		OnUpdate(float a_deltaTime);
		
		/**
		 * \brief Load an image from the given file. The image can be a sprite sheet.
		 * \param a_fileName The name of the image file.
		 * \param a_nColumns If the image is a sprite sheet, the number of columns in the sheet.
		 * \param a_nRows    If the image is a sprite sheet, the number of rows in the sheet.
		 * \return True if the image was loaded successfully, false otherwise.
		 * \remark Currently only supports .bmp files.
		 */
		bool
		LoadFromTexture(std::string_view a_fileName, unsigned int a_nColumns = 1, unsigned int a_nRows = 1);
		
		/**
		 * \brief Prepare the primitive for rendering
		 */
		void
		OnRenderPrepare(Math::Matrix4 const& a_model, Math::Matrix4 const& a_view, Math::Matrix4 const& a_projection);

		/**
		 * \brief Render the sprite to the screen
		 */
		void
		OnRender();
		
		/**
		 * \return The average depth of this sprite
		 * \remark only valid on any given frame after the call to \link OnRenderPrepare \endlink
		 */
		float
		GetDepth() const
		{
			return m_depth;
		}
		
		/**
		 * \return The frame number that the animation is currently on 
		 */
		unsigned int
		GetFrame() const;

		/**
		 * \param a_index The index of quad or triangle's vertex in the range [0, 3] or [0, 2] respectively
		 * \return The value of the vertex
		 */
		Detail::Vertex
		GetVertex(unsigned int a_index) const;
		
		/**
		 * \param a_index The index of quad's Uv in the range [0, 3]
		 * \return The value of the uv
		 */
		Math::Vector2
		GetUv(unsigned int a_index) const;
		
		/**
		 * \param a_frame The frame of animation to move to. 
		 */
		void
		SetFrame(unsigned int a_frame);

		/**
		 * \brief Switch to the animation with id a_id
		 * \param a_id The id of the animation to switch to. 
		 */
		void
		SetAnimation(unsigned int a_id);

		/**
		 * \brief Set the color that the texture will blend with.
		 * \param a_color A color.
		 * \remark Great for primitive lighting
		 */
		void
		SetColor(Math::Vector3 a_color);

		/**
		 * \brief Create an animation reel that can be changed to later.
		 * \param a_id A unique animation Id. Used to change to the animation with \link SetAnimation \endlink
		 * \param a_speed The time in seconds between frames
		 * \param a_frames A list of the zero-base indexed frames, where 0 is top-left
		 *        and moves left to right, top to bottom
		 */
		void
		CreateAnimation(unsigned int a_id, float a_speed, const std::vector<int>& a_frames);

		/**
		 * \param a_index The index of the quad's vertex in the range [0, 3]
		 * \param a_value The value of the vertex
		 */
		void
		SetVertex(unsigned int a_index, Detail::Vertex a_value);
		
		/**
		 * \param a_index The index of the quad's uv in the range [0, 3]
		 * \param a_value The value of the uv
		 */
		void
		SetUv(unsigned int a_index, Math::Vector2 a_value);

		bool
		IsValid() const;

	private:
		::CSimpleSprite* m_sprite;

		Detail::Vertex m_vertices[4];

		float m_depth;
	};
}