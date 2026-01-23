#include "pch.h"
#include "Sprite.h"

#include <NextAPI/SimpleSprite.h>

#include "Application/Time.h"

#include "Components/Transform.h"

#include "Math/Transformations.h"

namespace Next
{
	Sprite::Sprite(std::string_view a_fileName, unsigned a_nColumns, unsigned a_nRows)
	{
		LoadFromTexture(a_fileName, a_nColumns, a_nRows);
	}

	bool
	Sprite::LoadFromTexture(std::string_view a_fileName, unsigned a_nColumns, unsigned a_nRows)
	{
		m_simpleSprite = new CSimpleSprite(a_fileName.data(), a_nColumns, a_nRows);

		if (!m_simpleSprite)
		{
			return false;
		}

		m_simpleSprite->SetFrame(-1);

		return true;
	}
	
	void
	Sprite::OnUpdate(float a_deltaTime)
	{
		if (!IsValid())
		{
			return;
		}

		if (a_deltaTime == -1)
		{
			// NextAPI works in milliseconds while we work in seconds
			a_deltaTime = Time::DeltaTime();
			
			// Bug with update, there's no bounds checking so be careful to pass in
			// sufficiently small values of delta time
			a_deltaTime = std::min(a_deltaTime, 1.f / 30.f);
		}
		m_simpleSprite->Update(a_deltaTime * 1000.f);
	}

	void
	Sprite::OnRender()
	{
		if (!IsValid())
		{
			return;
		}
		
		m_simpleSprite->Draw();
	}
	
	float
	Sprite::GetWidth() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_simpleSprite->GetWidth();
	}

	float
	Sprite::GetHeight() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_simpleSprite->GetHeight();
	}

	Vector2
	Sprite::GetSize() const
	{
		if (!IsValid())
		{
			return {};
		}

		Vector2 result {
			m_simpleSprite->GetWidth(),
			m_simpleSprite->GetHeight()
		};

		return result;
	}
	
	unsigned
	Sprite::GetFrame() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_simpleSprite->GetFrame();
	}
	
	void
	Sprite::SetFrame(unsigned int a_frame)
	{
		if (!IsValid())
		{
			return;
		}

		m_simpleSprite->SetFrame(a_frame);
	}

	void
	Sprite::SetAnimation(unsigned int a_id)
	{
		if (!IsValid())
		{
			return;
		}

		m_simpleSprite->SetAnimation(a_id);
	}

	void
	Sprite::SetColor(Color a_color)
	{
		if (!IsValid())
		{
			return;
		}

		m_simpleSprite->SetColor(a_color.r, a_color.g, a_color.b);
	}

#ifdef NEXT_API_NO_CUSTOM_CODE
	float*
	GetCSimpleSpritePoints(::CSimpleSprite* a_sprite)
	{
		// Use pointer arithmetic to get a_sprite->m_points
		uintptr_t spritePtr = reinterpret_cast<uintptr_t>(a_sprite);
		spritePtr += sizeof(float) * 6;
		spritePtr += sizeof(unsigned int) * 3;
		return reinterpret_cast<float*>(spritePtr);
	}
#endif
	
	Vector2
	Sprite::GetVertex(unsigned int a_index) const
	{
		if (!IsValid() || a_index < 0 || a_index >= 4)
		{
			return {};
		}

	#ifdef NEXT_API_NO_CUSTOM_CODE
		float* points = GetCSimpleSpritePoints(m_simpleSprite);
		
		Vector2 result;
		result.x = points[2 * a_index];
		result.y = points[2 * a_index + 1];

		return result;
	#else
		Vector2 result;
		result.x = m_simpleSprite->GetVertex(2 * a_index);
		result.y = m_simpleSprite->GetVertex(2 * a_index + 1);

		return result;
	#endif
	}

	void
	Sprite::SetVertex(unsigned int a_index, Vector2 a_value)
	{
		if (!IsValid() || a_index < 0 || a_index >= 4)
		{
			return;
		}

	#ifdef NEXT_API_NO_CUSTOM_CODE
		float* points = GetCSimpleSpritePoints(m_simpleSprite);
		
		points[2 * a_index] = a_value.x;
		points[2 * a_index + 1] = a_value.y;
	#else
		m_simpleSprite->SetVertex(2 * a_index, a_value.x);
		m_simpleSprite->SetVertex(2 * a_index + 1, a_value.y);
	#endif
	}

#ifdef NEXT_API_NO_CUSTOM_CODE
	float*
	GetCSimpleSpriteUvCoords(::CSimpleSprite* a_sprite)
	{
		// Use pointer arithmetic to get a_sprite->m_uvcoords
		uintptr_t spritePtr = reinterpret_cast<uintptr_t>(a_sprite);
		spritePtr += sizeof(float) * 6;
		spritePtr += sizeof(unsigned int) * 3;
		spritePtr += sizeof(float[8]);
		return reinterpret_cast<float*>(spritePtr);
	}
#endif
	
	Vector2
	Sprite::GetUv(unsigned int a_index) const
	{
		if (!IsValid() || a_index < 0 || a_index >= 4)
		{
			return {};
		}

	#ifdef NEXT_API_NO_CUSTOM_CODE
		float* uvcoords = GetCSimpleSpriteUvCoords(m_simpleSprite);
		
		Vector2 result;
		result.x = uvcoords[2 * a_index];
		result.y = uvcoords[2 * a_index + 1];

		return result;
	#else
		Vector2 result;
		result.x = m_simpleSprite->GetUv(2 * a_index);
		result.y = m_simpleSprite->GetUv(2 * a_index + 1);

		return result;
	#endif
	}

	void
	Sprite::SetUv(unsigned int a_index, Vector2 a_value)
	{
		if (!IsValid() || a_index < 0 || a_index >= 4)
		{
			return;
		}

	#ifdef NEXT_API_NO_CUSTOM_CODE
		float* uvcoords = GetCSimpleSpriteUvCoords(m_simpleSprite);
		
		uvcoords[2 * a_index] = a_value.x;
		uvcoords[2 * a_index + 1] = a_value.y;
	#else
		m_simpleSprite->SetUv(2 * a_index, a_value.x);
		m_simpleSprite->SetUv(2 * a_index + 1, a_value.y);
	#endif
	}

	void
	Sprite::CreateAnimation(unsigned int a_id, float a_speed, const std::vector<int>& a_frames)
	{
		if (!IsValid())
		{
			return;
		}

		m_simpleSprite->CreateAnimation(a_id, a_speed, a_frames);
	}
	
	bool
	Sprite::IsValid() const
	{
		return m_simpleSprite != nullptr;
	}
}
