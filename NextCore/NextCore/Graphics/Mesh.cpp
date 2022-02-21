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
		int numVerticesPerPrimitive;

		switch (a_type)
		{
			case PrimitiveType::Triangle:
				numVerticesPerPrimitive = 3;
				break;
			case PrimitiveType::Quad:
				numVerticesPerPrimitive = 4;
				break;
			default:
				throw "Invalid File";
		}

		int numPrimitives = static_cast<int>(a_vertices.size() / numVerticesPerPrimitive);

		for (int i = 0; i < numPrimitives; i++)
		{
			Primitive p;
			p.m_primitiveType = a_type;
			p.LoadFromTexture(a_textureFilename);

			int baseIndex = i * numVerticesPerPrimitive;
			
			p.SetVertex(0, a_vertices[baseIndex]);
			p.SetVertex(1, a_vertices[baseIndex + 1]);
			p.SetVertex(2, a_vertices[baseIndex + 2]);
			// If numVerticesPerPrimitive is 3, set the last vertex to the first vertex since NextAPI
			// uses quads under the hood
			p.SetVertex(3, a_vertices[baseIndex + 3 % numVerticesPerPrimitive]);
			
			m_primitives.emplace_back(std::move(p));
		}
		
		return m_primitives.size() != 0;
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

	//void
	//Mesh::OnRenderPrepare(
	//	Math::Matrix4 const& a_model,
	//	Math::Matrix4 const& a_view,
	//	Math::Matrix4 const& a_projection
	//)
	//{
	//	for (int i = 0; i < m_primitives.size(); i++)
	//	{
	//		auto& primitive = m_primitives[i];
	//		primitive.OnRenderPrepare(a_model, a_view, a_projection);
	//	}
	//}
}
