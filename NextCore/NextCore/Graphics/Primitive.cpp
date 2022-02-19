#include "pch.h"

#include "Primitive.h"

#include "Math/Transformations.h"

#include "NextAPI/SimpleSprite.h"

namespace NextCore::Graphics
{
	void
	Primitive::OnUpdate(float a_deltaTime)
	{
		m_sprite.OnUpdate(a_deltaTime);
	}
	
	bool
	Primitive::LoadFromTexture(std::string_view a_fileName, unsigned a_nColumns, unsigned a_nRows)
	{
		m_sprite = Sprite(a_fileName.data(), a_nColumns, a_nRows);

		if (!m_sprite.IsValid())
		{
			return false;
		}

		m_sprite.SetFrame(-1);

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
		
		float runningDepthTotal = 0.0f;

		for (int i = 0; i < 4; i++)
		{
			const auto& [position, uv, normal] = m_vertices[i];
			
			auto transformedVertex = a_model * Vector4(position, 1.0f);
			auto projectedVertex   = a_projection * transformedVertex;

			projectedVertex.x /= projectedVertex.w;
			projectedVertex.y /= projectedVertex.w;
			projectedVertex.z /= projectedVertex.w;

			//projectedVertex.x *= aspect;

			m_sprite.m_simpleSprite->SetVertex(2 * i,     projectedVertex.x);
			m_sprite.m_simpleSprite->SetVertex(2 * i + 1, projectedVertex.y);
			
			m_sprite.m_simpleSprite->SetUv(2 * i,     uv.x);
			m_sprite.m_simpleSprite->SetUv(2 * i + 1, uv.y);

			runningDepthTotal += projectedVertex.z;
		}

		m_depth = runningDepthTotal / 4;
	}

	void
	Primitive::OnRender()
	{
		m_sprite.OnRender();
	}
	
	Detail::Vertex
	Primitive::GetVertex(unsigned a_index) const
	{
		return m_vertices[a_index];
	}
	
	void
	Primitive::SetColor(Color a_color)
	{
		m_sprite.SetColor(a_color);
	}
	
	void
	Primitive::SetVertex(unsigned a_index, Detail::Vertex a_value)
	{
		// Don't set the sprite's vertices; We maintain model space
		// vertices that are used to calculate the sprite's vertices per frame
		m_vertices[a_index] = a_value;
	}
	
	bool
	Primitive::IsValid() const
	{
		return m_sprite.IsValid();
	}
}
