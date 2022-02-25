#include "pch.h"

#include "Mesh.h"

#include "Application/Time.h"

#include "Detail/IModelLoader.h"

namespace Next
{
	bool
	Mesh::GeneratePrimitives(
		std::string_view                        a_textureFilename,
		Detail::vertex_container_t const&       a_vertices,
		Detail::vertex_count_container_t const& a_vertexCounts
	)
	{
		int indexIntoVertices = 0;
		
		for (int i = 0; i < a_vertexCounts.size(); i++)
		{
			// Get how many vertices are in the current primitive
			int numVerticesPerPrimitive = a_vertexCounts[i];
			
			RenderPrimitive p;
			p.m_primitiveType = numVerticesPerPrimitive == 3
				                    ? RenderPrimitiveType::Triangle
				                    : RenderPrimitiveType::Quad;
			p.LoadFromTexture(a_textureFilename);

			// Set all of the vertices
			p.SetVertex(0, a_vertices[indexIntoVertices]);
			p.SetVertex(1, a_vertices[indexIntoVertices + 1]);
			p.SetVertex(2, a_vertices[indexIntoVertices + 2]);
			
			// If numVerticesPerPrimitive is 3, set the last vertex to the first vertex since NextAPI
			// uses quads under the hood
			p.SetVertex(3, a_vertices[indexIntoVertices + (3 % numVerticesPerPrimitive)]);
			
			m_primitives.emplace_back(std::move(p));

			indexIntoVertices += numVerticesPerPrimitive;
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
}
