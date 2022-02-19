#include "pch.h"

#include "Primitive.h"

#include "Application/Time.h"

#include "Math/Transformations.h"

namespace NextCore::Graphics
{
	void
	Primitive::OnUpdate(float a_deltaTime)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->Update(a_deltaTime);
	}

	bool
	Primitive::LoadFromTexture(std::string_view a_fileName, unsigned a_nColumns, unsigned a_nRows)
	{
		m_sprite = new CSimpleSprite(a_fileName.data(), a_nColumns, a_nRows);

		if (!m_sprite)
		{
			return false;
		}

		SetFrame(0);

		return true;
	}

	void
	Primitive::OnRenderPrepare(
		Math::Matrix4 const& a_model,
		Math::Matrix4 const& a_view,
		Math::Matrix4 const& a_projection
	)
	{
		if (!IsValid())
		{
			return;
		}

		using namespace Math;

		//// Model Matrix
		//auto* transform = Transform();
		//auto& scale    = transform->Scale();
		//auto& rotation = transform->Rotation();
		//auto& position = transform->Position();

		//auto model = Matrix4::Identity();
		//model *= Scale(scale);
		//model *= RotateZ(rotation.z);
		//model *= RotateY(rotation.y);
		//model *= RotateX(rotation.x);
		//model *= Translate(position);

		//// View Matrix

		//// Perspective Matrix
		//float fov = 90;
		//float aspect = 16.f / 9.f;
		//
		//auto perspective = Perspective(fov, aspect, 0.1f, 1000.f);

		float runningDepthTotal = 0.0f;
		
		for (int i = 0; i < 4; i++)
		{
			auto const& vertex = m_vertices[i];
			
			Vector4 position = { vertex.position, 1.0f };

			auto transformedVertex = a_model * position;
			auto projectedVertex   = a_projection * transformedVertex;

			projectedVertex.x /= projectedVertex.w;
			projectedVertex.y /= projectedVertex.w;
			projectedVertex.z /= projectedVertex.w;

			//projectedVertex.x *= aspect;

			m_sprite->SetVertex(2 * i,     projectedVertex.x);
			m_sprite->SetVertex(2 * i + 1, projectedVertex.y);
			
			m_sprite->SetUv(2 * i,     vertex.uv.x);
			m_sprite->SetUv(2 * i + 1, vertex.uv.y);

			runningDepthTotal += projectedVertex.z;
		}

		m_depth = runningDepthTotal / 4;
	}

	void
	Primitive::OnRender()
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->Draw();
	}

	unsigned
	Primitive::GetFrame() const
	{
		if (!IsValid())
		{
			return {};
		}

		return m_sprite->GetFrame();
	}

	Detail::Vertex
	Primitive::GetVertex(unsigned a_index) const
	{
		if (!IsValid())
		{
			return {};
		}
		
		return m_vertices[a_index];
	}

	Math::Vector2
	Primitive::GetUv(unsigned a_index) const
	{
		if (!IsValid())
		{
			return {};
		}

		// Multiply input index by 2 because underlying api uses individual floats instead of structs
		unsigned x_index = a_index * 2;
		unsigned y_index = x_index + 1;

		Math::Vector2 result {
			m_sprite->GetUv(x_index),
			m_sprite->GetUv(y_index)
		};

		return result;
	}

	void
	Primitive::SetFrame(unsigned a_frame)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetFrame(a_frame);
	}

	void
	Primitive::SetAnimation(unsigned a_id)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetAnimation(a_id);
	}

	void
	Primitive::SetColor(Math::Vector3 a_color)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->SetColor(a_color.r, a_color.g, a_color.b);
	}

	void
	Primitive::CreateAnimation(unsigned a_id, float a_speed, const std::vector<int>& a_frames)
	{
		if (!IsValid())
		{
			return;
		}

		m_sprite->CreateAnimation(a_id, a_speed, a_frames);
	}

	void
	Primitive::SetVertex(unsigned a_index, Detail::Vertex a_value)
	{
		if (!IsValid())
		{
			return;
		}

		// Don't set the sprite's vertices; We maintain model space
		// vertices that are used to calculate the sprite's vertices per frame
		m_vertices[a_index] = a_value;
	}

	void
	Primitive::SetUv(unsigned a_index, Math::Vector2 a_value)
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
	Primitive::IsValid() const
	{
		return m_sprite != nullptr;
	}
}
