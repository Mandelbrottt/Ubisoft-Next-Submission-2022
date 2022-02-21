#pragma once

// Keep to avoid errors when included
#include <string_view>

#include "Color.h"
#include "Sprite.h"

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
		
	public:
		/* TODO: Find a way to have non-public constructor, and deleted / non-public copy constructors
		         and still be able to maintain a copied list in the renderer without resorting to memcpy */
		Primitive() = default;
		~Primitive() = default;
		Primitive(Primitive&& a_other) = default;
		
		Primitive(Primitive const& a_other) = default;
		Primitive& 
		operator =(Primitive const& a_other) = default;
		
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
		 * \brief Render the sprite to the screen
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
		Detail::Vertex const&
		GetVertex(unsigned int a_index) const;

		PrimitiveType
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
		SetVertex(unsigned int a_index, Detail::Vertex a_value);
		
		bool
		IsValid() const;

	private:
		/**
		 * \brief Process the given bmp file to ensure 32 bit pixels
		 * \param a_filename The name of the file to process
		 */
		bool
		ProcessBmp(std::string_view a_filename);
	
	private:
		Sprite m_sprite;

		Detail::Vertex m_vertices[4];
		
		PrimitiveType m_primitiveType;
	};
}