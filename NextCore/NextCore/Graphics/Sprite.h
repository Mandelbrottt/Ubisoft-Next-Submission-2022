#pragma once

// Keep to avoid errors when including Sprite.h
#include <string_view>

#include "Common.h"
#include "Math/Vector.h"

class CSimpleSprite;

extern void Shutdown();

namespace NextCore
{
	class NEXT_CORE_EXPORT Sprite final
	{
		friend void ::Shutdown();
		
		// If width, height and UV coords are not provided then they will be derived from the texture size.
		explicit
		Sprite(std::string_view a_fileName, unsigned int a_nColumns = 1, unsigned int a_nRows = 1);

		~Sprite() = default;

		static
		void
		Cleanup();
		
	public:
		static
		Sprite* 
		Create(std::string_view a_fileName, unsigned int a_nColumns = 1, unsigned int a_nRows = 1);
		
		static
		void 
		Destroy(Sprite* a_sprite);
		
		void
		Update();
		
		void
		Render();
		
		Math::Vector2
		GetPosition() const;
		
		float
		GetWidth() const;
		
		float
		GetHeight() const;
		
		Math::Vector2
		GetSize() const;
		
		float
		GetAngle() const;
		
		float
		GetScale() const;
		
		unsigned int
		GetFrame() const;
		
		/**
		 * \brief 
		 * \param a_index The index of quad's vertex in the range [0, 3]
		 * \return The value of the vertex
		 */
		Math::Vector2
		GetVertex(unsigned int a_index) const;
		
		void
		SetPosition(Math::Vector2 a_position);
		
		void
		SetPosition(float a_x, float a_y);
		
		void
		SetAngle(float a_angle);
		
		void
		SetScale(float a_scale);
		
		void
		SetFrame(unsigned int a_frame);
		
		void
		SetAnimation(int a_id);
		
		void
		SetColor(Math::Vector3 a_color);
		
		void
		CreateAnimation(unsigned int a_id, float a_speed, const std::vector<int>& a_frames);

		/**
		 * \brief 
		 * \param a_index The index of the quad's vertex in the range [0, 3]
		 * \param a_value The value of the vertex
		 */
		void
		SetVertex(unsigned int a_index, Math::Vector2 a_value);
		
		bool
		IsValid() const;

	private:
		::CSimpleSprite* m_sprite;
	};
}
