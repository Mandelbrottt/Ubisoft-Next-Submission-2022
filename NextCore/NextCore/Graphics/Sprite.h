#pragma once

// Keep to avoid errors when including Sprite.h
#include <string_view>

#include "NextCoreCommon.h"
#include "Math/Vector.h"

#include "Reflection/Reflection.h"

#include "Scripting/Component.h"

class CSimpleSprite;

namespace NextCore
{
	class NEXT_CORE_EXPORT Sprite : public Scripting::Component
	{
		GenerateConstructors(Sprite)
		
	public:
		void OnUpdate() override;

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
		Render();
		
		/**
		 * \return The width of the sprite in pixels. 
		 */
		float
		GetWidth() const;

		/**
		 * \return The height of the sprite in pixels. 
		 */
		float
		GetHeight() const;

		/**
		 * \return The size of the sprite in pixels. 
		 */
		Math::Vector2
		GetSize() const;
		
		/**
		 * \return The frame number that the animation is currently on 
		 */
		unsigned int
		GetFrame() const;

		/**
		 * \param a_index The index of quad's vertex in the range [0, 3]
		 * \return The value of the vertex
		 */
		Math::Vector2
		GetVertex(unsigned int a_index) const;
		
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
		 * \brief Set the color that the sprite will blend with.
		 * \param a_color A color.
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
		SetVertex(unsigned int a_index, Math::Vector2 a_value);

		bool
		IsValid() const;

	private:
		// TODO: Keep local copies of variables and synchronise them once per frame with the sprite
		::CSimpleSprite* m_sprite;

		REFLECT_DECLARE(Sprite)

		REFLECT_MEMBERS(
			REFLECT_FIELD(m_sprite)
		)
	};
}
