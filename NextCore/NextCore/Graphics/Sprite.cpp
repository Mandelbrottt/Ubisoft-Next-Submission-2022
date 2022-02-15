#include "pch.h"

#include "Sprite.h"

#include "Application/Time.h"

#include "Reflection/Constructor.h"

namespace NextCore
{
	using Math::Vector2;
	using Math::Vector3;

	Sprite::Sprite(std::string_view a_fileName, unsigned int a_nColumns, unsigned int a_nRows)
	{
		if (a_fileName.empty())
		{
			return;
		}

		LoadFromTexture(a_fileName, a_nColumns, a_nRows);
	}

	void
	Sprite::OnUpdate()
	{
		if (!IsValid())
		{
			return;
		}

		// NextAPI works in milliseconds while we work in seconds
		float adjustedDeltaTime = Time::DeltaTime();
		
		// Bug with update, there's no bounds checking so be careful to pass in
		// sufficiently small values of delta time
		adjustedDeltaTime = std::min(adjustedDeltaTime, 1.f / 30.f);
		m_sprite->Update(adjustedDeltaTime);
	}

	bool
	Sprite::LoadFromTexture(std::string_view a_fileName, unsigned a_nColumns, unsigned a_nRows)
	{
		m_sprite = App::CreateSprite(a_fileName.data(), a_nColumns, a_nRows);

		if (!m_sprite)
		{
			return false;
		}

		SetFrame(0);

		return true;
	}

	void
	Sprite::Render()
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->Draw();
	}

	Vector2
	Sprite::GetPosition() const
	{
		if (!IsValid())
		{
			return {};
		}

		Vector2 result;
		m_sprite->GetPosition(result.x, result.y);
		return result;
	}

	float
	Sprite::GetWidth() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_sprite->GetWidth();
	}

	float
	Sprite::GetHeight() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_sprite->GetHeight();
	}

	Vector2
	Sprite::GetSize() const
	{
		if (!IsValid())
		{
			return {};
		}

		Vector2 result {
			m_sprite->GetWidth(),
			m_sprite->GetHeight()
		};

		return result;
	}

	float
	Sprite::GetAngle() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_sprite->GetAngle();
	}

	float
	Sprite::GetScale() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_sprite->GetScale();
	}

	unsigned
	Sprite::GetFrame() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_sprite->GetFrame();
	}

	/**
	 * \brief 
	 * \param a_index The index of quad's vertex in the range [0, 3]
	 * \return The value of the vertex
	 */
	Vector2
	Sprite::GetVertex(unsigned int a_index) const
	{
		if (!IsValid())
		{
			return {};
		}

		// Multiply input index by 2 because underlying api uses individual floats instead of structs
		unsigned x_index = a_index * 2;
		unsigned y_index = x_index + 1;

		Vector2 result {
			m_sprite->GetVertex(x_index),
			m_sprite->GetVertex(y_index)
		};

		return result;
	}

	void
	Sprite::SetPosition(Vector2 a_position)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetPosition(a_position.x, a_position.y);
	}

	void
	Sprite::SetPosition(float a_x, float a_y)
	{
		if (!IsValid())
		{
			return;
		}

		this->SetPosition({ a_x, a_y });
	}

	void
	Sprite::SetAngle(float a_angle)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetAngle(a_angle);
	}

	void
	Sprite::SetScale(float a_scale)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetScale(a_scale);
	}

	void
	Sprite::SetFrame(unsigned int a_frame)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetFrame(a_frame);
	}

	void
	Sprite::SetAnimation(unsigned int a_id)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetAnimation(a_id);
	}

	void
	Sprite::SetColor(Vector3 a_color)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetColor(a_color.r, a_color.g, a_color.b);
	}

	void
	Sprite::CreateAnimation(unsigned int a_id, float a_speed, const std::vector<int>& a_frames)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->CreateAnimation(a_id, a_speed, a_frames);
	}

	/**
	 * \brief 
	 * \param a_index The index of the quad's vertex in the range [0, 3]
	 * \param a_value The value of the vertex
	 */
	void
	Sprite::SetVertex(unsigned int a_index, Vector2 a_value)
	{
		if (!IsValid())
		{
			return;
		}

		// Multiply input index by 2 because underlying api uses individual floats instead of structs
		unsigned x_index = a_index * 2;
		unsigned y_index = x_index + 1;

		m_sprite->SetVertex(x_index, a_value.x);
		m_sprite->SetVertex(y_index, a_value.y);
	}

	inline bool
	Sprite::IsValid() const
	{
		return m_sprite != nullptr;
	}
}
