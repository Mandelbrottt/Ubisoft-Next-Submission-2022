#include "pch.h"

#include "Sprite.h"

#include "Application/Time.h"

#include "Components/Transform.h"

#include "Math/Transformations.h"

#include "Reflection/Constructor.h"

namespace NextCore::Graphics
{
	using Math::Vector2;
	using Math::Vector3;
	using Math::Vector4;

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
