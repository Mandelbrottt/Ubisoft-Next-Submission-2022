#include "pch.h"

#include "Sprite.h"

#include "Application/Time.h"

#include "Components/Transform.h"

namespace NextCore::Graphics
{
	using Math::Vector2;
	using Math::Vector3;

	bool
	Sprite::LoadFromTexture(std::string_view a_fileName, unsigned a_nColumns, unsigned a_nRows)
	{
		m_sprite = new CSimpleSprite(a_fileName.data(), a_nColumns, a_nRows);

		if (!m_sprite)
		{
			return false;
		}

		SetFrame(-1);

		return true;
	}

	static uint8_t*
	ByteArrayFromColorArray(Color* a_imageData, uint32_t a_imageSize)
	{
		uint8_t* data = new uint8_t[4 * a_imageSize];

		constexpr auto colorByteFromFloat = [](float a_component)
		{
			a_component = std::clamp(a_component, 0.f, 1.f);
			return static_cast<uint8_t>(a_component * 0xff);
		};

		for (uint32_t i = 0; i < a_imageSize; i++)
		{
			data[4 * i]     = colorByteFromFloat(a_imageData[i].r);
			data[4 * i + 1] = colorByteFromFloat(a_imageData[i].g);
			data[4 * i + 2] = colorByteFromFloat(a_imageData[i].b);
			data[4 * i + 3] = colorByteFromFloat(a_imageData[i].a);
		}

		return data;
	}

	bool
	Sprite::LoadFromData(
		Color*   a_imageData,
		uint32_t a_width,
		uint32_t a_height,
		uint32_t a_nColumns,
		uint32_t a_nRows
	)
	{
		if (!a_imageData)
		{
			return false;
		}

		uint32_t imageSize = a_width * a_height;

		uint8_t* imageData = ByteArrayFromColorArray(a_imageData, imageSize);

		m_sprite = new CSimpleSprite(imageData, a_width, a_height, a_nColumns, a_nRows);

		if (!m_sprite)
		{
			return false;
		}

		SetFrame(-1);

		return true;
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
		m_sprite->Update(adjustedDeltaTime * 1000.f);
	}

	void
	Sprite::OnRender()
	{
		if (!IsValid())
		{
			return;
		}

		auto* transform = Transform();
		m_sprite->SetPosition(transform->Position().x, transform->Position().y);
		m_sprite->SetAngle(transform->Rotation().z);
		m_sprite->SetScale(transform->Scale().z);

		m_sprite->Draw();
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

	bool
	Sprite::IsValid() const
	{
		return m_sprite != nullptr;
	}
}
