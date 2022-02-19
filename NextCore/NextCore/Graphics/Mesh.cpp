#include "pch.h"

#include "Mesh.h"

#include "Application/Time.h"

#include "Detail/IModelLoader.h"

namespace NextCore::Graphics
{
	bool
	Mesh::GeneratePrimitives(
		std::string_view                  a_textureFilename,
		Detail::vertex_container_t const& a_vertices,
		PrimitiveType                     a_type
	)
	{
		// TODO: Implement model loading
		return false;
	}

	void
	Mesh::OnUpdate()
	{
		// NextAPI works in milliseconds while we work in seconds
		float adjustedDeltaTime = Time::DeltaTime();

		// Bug with update, there's no bounds checking so be careful to pass in
		// sufficiently small values of delta time
		adjustedDeltaTime = std::min(adjustedDeltaTime, 1.f / 30.f);

		for (int i = 0; i < m_primitives.size(); i++)
		{
			auto& primitive = m_primitives[i];
			primitive.OnUpdate(adjustedDeltaTime);
		}
	}

	void
	Mesh::OnRenderPrepare(
		Math::Matrix4 const& a_model,
		Math::Matrix4 const& a_view,
		Math::Matrix4 const& a_projection
	)
	{
		for (int i = 0; i < m_primitives.size(); i++)
		{
			auto& primitive = m_primitives[i];
			primitive.OnRenderPrepare(a_model, a_view, a_projection);
		}
	}
}
