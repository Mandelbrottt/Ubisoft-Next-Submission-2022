#pragma once

#include "Color.h"
#include "Sprite.h"

#include "ModelLoaderTypedefs.h"

class CSimpleSprite;

namespace Next
{
	enum class RenderPrimitiveType
	{
		Null = 0,
		Triangle,
		Quad,
	};
	
	/**
	 * \brief A primitive that cannot be broken down anymore in the given underlying framework.
	 *        Represents a quad or a triangle with 3D vertices, uvs, and a texture.
	 */
	class RenderPrimitive final
	{
		friend class Mesh;
		friend class CubeMap;
		
	public:
		/* TODO: Find a way to have non-public constructor, and deleted / non-public copy constructors
		         and still be able to maintain a copied list in the renderer without resorting to memcpy */
		RenderPrimitive() = default;
		~RenderPrimitive() = default;
		RenderPrimitive(RenderPrimitive&& a_other) = default;
		
		RenderPrimitive(RenderPrimitive const& a_other) = default;
		RenderPrimitive& 
		operator =(RenderPrimitive const& a_other) = default;

		explicit
		RenderPrimitive(std::string_view a_fileName, unsigned int a_nColumns = 1, unsigned int a_nRows = 1);
		
		/**
		 * \brief Update the underlying data per-frame
		 * \param a_deltaTime Time since last frame in milliseconds.
		 *        Leave empty for time to be calculated automatically.
		 */
		void
		OnUpdate(float a_deltaTime = -1);
		
		/**
		 * \brief Load an image from the given file. The image can be a sprite sheet.
		 * \param a_fileName The name of the image file.
		 * \param a_nColumns If the image is a sprite sheet, the number of columns in the sheet.
		 * \param a_nRows    If the image is a sprite sheet, the number of rows in the sheet.
		 * \return True if the image was loaded successfully, false otherwise.
		 * \remark Currently only supports .bmp files.
		 */
		bool
		LoadFromFile(std::string_view a_fileName, unsigned int a_nColumns = 1, unsigned int a_nRows = 1);
		
		/**
		 * \brief Render the primitive to the screen
		 */
		void
		OnRender();
		
		Sprite*
		GetSprite()
		{
			return &m_sprite;
		}
		
		/**
		 * \param a_index The index of quad or triangle's vertex in the range [0, 3] or [0, 2] respectively
		 * \return The value of the vertex
		 */
		ModelVertex const&
		GetVertex(unsigned int a_index) const;

		RenderPrimitiveType
		GetPrimitiveType() const
		{
			return m_primitiveType;
		}
		
		/**
		 * \brief Set the color that the texture will blend with.
		 * \param a_color A color.
		 * \remark Great for primitive lighting
		 */
		void
		SetColor(Color a_color);
		
		/**
		 * \param a_index The index of the quad's vertex in the range [0, 3]
		 * \param a_value The value of the vertex
		 */
		void
		SetVertex(unsigned int a_index, ModelVertex a_value);
		
		bool
		IsValid() const;

	private:
		/**
		 * \brief Process the given bmp file to ensure 32 bit pixels
		 * \param a_filename The name of the file to process
		 */
		bool
		ProcessBmp(std::string_view a_filename) const;
	
	private:
		Sprite m_sprite;

		ModelVertex m_vertices[4];
		
		RenderPrimitiveType m_primitiveType = RenderPrimitiveType::Null;
	};
}