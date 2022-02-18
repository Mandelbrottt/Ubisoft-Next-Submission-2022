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
	Sprite::OnRender()
	{
		if (!IsValid())
		{
			return;
		}
		
		//model[2][3] = -10;
		
		auto* transform = Transform();
		auto& position = transform->Position();
		position.z = 5;

		auto widthOn2  = m_sprite->GetWidth()  / 2;
		auto heightOn2 = m_sprite->GetHeight() / 2;

		// Ensure a consistent model size for now
		//SetVertex(0, { -widthOn2, -heightOn2 });
		//SetVertex(1, {  widthOn2, -heightOn2 });
		//SetVertex(2, {  widthOn2,  heightOn2 });
		//SetVertex(3, { -widthOn2,  heightOn2 });

		SetVertex(0, { -1, -1 });
		SetVertex(1, {  1, -1 });
		SetVertex(2, {  1,  1 });
		SetVertex(3, { -1,  1 });
		
		auto perspective = Math::Perspective(90, 16.f / 9.f, 0.1, 1000);

		auto model = Math::Translate(position);

		for (int i = 0; i < 4; i++)
		{
			Vector4 vertex = { GetVertex(i), 1.0f };
			vertex.z = -15;

			auto mvp = model * perspective;
			//auto mvp = perspective;
			auto adjustedVertex = mvp * vertex;

			if (adjustedVertex.w != 0.0f)
			{
				adjustedVertex.x /= adjustedVertex.w;
				adjustedVertex.y /= adjustedVertex.w;
				adjustedVertex.z /= adjustedVertex.w;
			}

			auto map = [](float a_value, float a_inStart, float a_inEnd, float a_outStart, float a_outEnd)
			{
				float slope = (a_outEnd - a_outStart) / (a_inEnd - a_inStart);
				return a_outStart + slope * (a_value - a_inStart);
			};

			adjustedVertex.x = map(adjustedVertex.x, -1, 1, 0, APP_VIRTUAL_WIDTH);
			adjustedVertex.y = map(adjustedVertex.y, -1, 1, 0, APP_VIRTUAL_HEIGHT);

			SetVertex(i, { adjustedVertex.x, adjustedVertex.y });
		}
		
		//m_sprite->SetPosition(transform->Position().x, transform->Position().y);
		//m_sprite->SetAngle(transform->Rotation().z);
		//m_sprite->SetScale(transform->Scale().z);

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
	Vector3
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

	Math::Vector2
	Sprite::GetUv(unsigned a_index) const
	{
		if (!IsValid())
		{
			return {};
		}

		// Multiply input index by 2 because underlying api uses individual floats instead of structs
		unsigned x_index = a_index * 2;
		unsigned y_index = x_index + 1;

		Vector2 result {
			m_sprite->GetUv(x_index),
			m_sprite->GetUv(y_index)
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

	void
	Sprite::SetUv(unsigned a_index, Math::Vector2 a_value)
	{
		if (!IsValid())
		{
			return;
		}

		// Multiply input index by 2 because underlying api uses individual floats instead of structs
		unsigned x_index = a_index * 2;
		unsigned y_index = x_index + 1;

		m_sprite->SetUv(x_index, a_value.x);
		m_sprite->SetUv(y_index, a_value.y);
	}

	bool
	Sprite::IsValid() const
	{
		return m_sprite != nullptr;
	}
}
