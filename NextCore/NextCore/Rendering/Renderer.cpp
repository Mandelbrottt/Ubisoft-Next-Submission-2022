#include "pch.h"

#include "Renderer.h"

#include "Components/Transform.h"

#include "Graphics/Model.h"

namespace NextCore::Renderer
{
	using Math::Vector3;
	using Math::Vector4;
	using Math::Matrix4;
	
	struct RenderQueueElement
	{
		explicit
		RenderQueueElement(Graphics::Primitive const& a_primitive)
			: primitive(a_primitive) { }
		
		Graphics::Primitive primitive;

		//Scripting::EntityId entityId;

		// Should these be moved to a separate list to optimize cache for rendering,
		// or stay here to optimize cache for calculations?
		Vector4 positions[4];

		Vector3 normals[2];

		Graphics::Color color;

		float depth = 0;
	};

	struct TransformationCacheElement
	{
		std::size_t numPrimitives;

		Matrix4 modelMatrix;
	};

	static std::vector<TransformationCacheElement> g_transformationCache;

	// TODO: Do all calculations in-place instead of copying them over every frame
	static std::vector<RenderQueueElement> g_primitiveRenderQueue;

	static std::mutex g_rasterQueueMutex;
	static std::vector<RenderQueueElement> g_primitiveRasterQueue;

	static Matrix4 g_viewMatrix;
	static Matrix4 g_projectionMatrix;
	
	// If capacity is under recent max for X number of frames, shrink to fit next smallest maximum
	static int g_shrinkToFitCounter = 0;

	static
	void
	TransformPrimitivesByModelMatrix(std::size_t a_offset, std::size_t a_count, Matrix4 const& a_modelMatrix);
	
	static
	void
	TransformPrimitivesByViewProjectionMatrix(std::size_t a_offset, std::size_t a_count);
	
	void
	PrepareScene(Matrix4 const& a_viewMatrix, Matrix4 const& a_projectionMatrix)
	{
		g_viewMatrix       = a_viewMatrix;
		g_projectionMatrix = a_projectionMatrix;
	}

	// TODO: Add SceneSetup function that takes in a camera and sets up the view matrix
	void
	Submit(Graphics::Model const& a_model, Scripting::Entity const& a_entity)
	{
		std::size_t numPrimitives = 0;

		std::size_t offset = g_primitiveRenderQueue.size();
		
		for (auto const& mesh : a_model.GetMeshes())
		{
			auto const& primitives = mesh.GetPrimitives();
			
			//g_primitiveRenderQueue.insert(queueEnd, primitives.begin(), primitives.end());
			g_primitiveRenderQueue.reserve(g_primitiveRenderQueue.size() + primitives.size());

			for (auto const& primitive : primitives)
			{
				//RenderQueueElement element = { primitive, a_entity.GetEntityId() };
				RenderQueueElement element { primitive };
				g_primitiveRenderQueue.emplace_back(std::move(element));
			}

			numPrimitives += primitives.size();
		}
		
		TransformationCacheElement element = { numPrimitives, a_entity.Transform()->GetTransformationMatrix() };

		// TODO: Add to transformation cache and call TransformPrimitivesByModelMatrix on separate thread(s)
		//g_transformationCache.emplace_back(std::move(element));
		TransformPrimitivesByModelMatrix(offset, element.numPrimitives, element.modelMatrix);
	}

	static
	bool
	PrimitiveBackToFrontSortPredicate(RenderQueueElement const& a_lhs, RenderQueueElement const& a_rhs);
	
	/**
	 * \brief Flush the renderer's command buffer and draw to the screen.
	 * \remark This function needs to be manually declared to use, as
	 *         this function is only intended to be called by the system at
	 *         the end of each frame
	 */
	void
	Flush()
	{
		TransformPrimitivesByViewProjectionMatrix(0, g_primitiveRasterQueue.size());
		
		// We use the painter's algorithm (https://en.wikipedia.org/wiki/Painter%27s_algorithm)
		// This is because there is no depth buffer in NextAPI, and so this is a simple solution, albeit with
		// some artifacting.
		// In the future we could probably find a more efficient drawing algorithm (BSPs like doom does?)
		std::sort(g_primitiveRasterQueue.begin(), g_primitiveRasterQueue.end(), PrimitiveBackToFrontSortPredicate);
		
		for (auto& element : g_primitiveRasterQueue)
		{
			float depth = element.depth;
			if (depth < 0 || depth > 1)
			{
				break;
			}

			auto& primitive = element.primitive;
			auto& positions = element.positions;

			auto* sprite = primitive.GetSprite();
			
			int numVerts = primitive.GetPrimitiveType() == Graphics::PrimitiveType::Triangle ? 3 : 4;

			for (int i = 0; i < 4; i++)
			{
				sprite->SetColor(element.color);
			}
			
			for (int i = 0; i < numVerts; i++)
			{
				auto const& uv = primitive.GetVertex(i).uv;

				sprite->m_simpleSprite->SetVertex(2 * i,     positions[i].x);
				sprite->m_simpleSprite->SetVertex(2 * i + 1, positions[i].y);
				
				sprite->m_simpleSprite->SetUv(2 * i,     uv.x);
				sprite->m_simpleSprite->SetUv(2 * i + 1, uv.y);
			}
			
			// If primitive is a triangle, fourth vertex shares values with first
			if (primitive.GetPrimitiveType() == Graphics::PrimitiveType::Triangle)
			{
				auto* s = sprite->m_simpleSprite;
				s->SetVertex(6, s->GetVertex(0));
				s->SetVertex(7, s->GetVertex(1));
				s->SetUv(6, s->GetUv(0));
				s->SetUv(7, s->GetUv(1));
			}
			
			element.primitive.OnRender();
		}

		// Reallocation / capacity change not guaranteed according to
		// https://www.cplusplus.com/reference/vector/vector/capacity/
		// When does it happen? Can we avoid this?
		g_primitiveRenderQueue.clear();
		g_primitiveRasterQueue.clear();
		g_transformationCache.clear();
	}

	void
	TransformPrimitivesByModelMatrix(std::size_t a_offset, std::size_t a_count, Matrix4 const& a_modelMatrix)
	{
		// TODO: Find way to avoid allocation in thread safe way
		std::vector<std::size_t> primitivesToRaster;
		primitivesToRaster.reserve(a_count);

		for (std::size_t i = a_offset; i < a_offset + a_count; i++)
		{
			auto& element = g_primitiveRenderQueue[i];
			auto& primitive = element.primitive;
			
			int numVerts = primitive.GetPrimitiveType() == Graphics::PrimitiveType::Triangle ? 3 : 4;

			auto& positions = element.positions;
			
			for (int j = 0; j < numVerts; j++)
			{
				auto const& vertex = primitive.GetVertex(j);

				auto transformedVertex = a_modelMatrix * Vector4(vertex.position, 1.0f);
				positions[j] = transformedVertex;
			}
			
			auto& normals   = element.normals;

			for (int j = 0; j < numVerts - 2; j++)
			{
				Vector3 line1 = Vector3(positions[j + 1]) - Vector3(positions[0]);
				Vector3 line2 = Vector3(positions[j + 2]) - Vector3(positions[0]);

				normals[j] = Normalize(Cross(line1, line2));
			}

			Vector3 cameraPosition = { 0, 0, 0 };

			Vector3 commonPointOnPrimitive1 = Vector3(positions[0]) - cameraPosition;
			Vector3 commonPointOnPrimitive2 = Vector3(positions[3]) - cameraPosition;

			if (Dot(normals[0], commonPointOnPrimitive1) < 0 || Dot(normals[1], commonPointOnPrimitive2) < 0)
			{
				primitivesToRaster.push_back(i);
			}
		}

		std::scoped_lock lock(g_rasterQueueMutex);
		for (std::size_t i = 0; i < primitivesToRaster.size(); i++) 
		{
			std::size_t index = primitivesToRaster[i];
			
			g_primitiveRasterQueue.emplace_back(std::move(g_primitiveRenderQueue[index]));
		}
	}

	void
	TransformPrimitivesByViewProjectionMatrix(std::size_t a_offset, std::size_t a_count)
	{
		for (std::size_t i = a_offset; i < a_count; i++)
		{
			auto& element = g_primitiveRasterQueue[i];
			auto& primitive = element.primitive;
			auto& positions = element.positions;
			auto& normals   = element.normals;
			
			auto* sprite = primitive.GetSprite();
			
			int numVerts = primitive.GetPrimitiveType() == Graphics::PrimitiveType::Triangle ? 3 : 4;
			
			Vector3 commonNormal = normals[0];

			if (normals[1] != Vector3(0))
			{
				commonNormal += normals[1];
				commonNormal = Normalize(commonNormal * 0.5f);
			}
			
			for (int j = 0; j < numVerts; j++)
			{
				Vector3 lightDirection = { -1, 1, 1 };
				lightDirection.Normalize();

				float dotProduct = Dot(commonNormal, -lightDirection);
				dotProduct = std::max(dotProduct, 0.f) + 0.3f;
				
				auto color = Graphics::Color(dotProduct, dotProduct, dotProduct);

				element.color = color;

				//TODO: Factor in view matrix
				
				auto const& [position, uv, normal] = primitive.GetVertex(j);

				auto projectedVertex = g_projectionMatrix * positions[j];

				projectedVertex.x /= projectedVertex.w;
				projectedVertex.y /= projectedVertex.w;
				projectedVertex.z /= projectedVertex.w;
				
				element.depth += projectedVertex.z;
				
				positions[j] = projectedVertex;
			}

			element.depth = element.depth / numVerts;
		}
	}

	bool
	PrimitiveBackToFrontSortPredicate(RenderQueueElement const& a_lhs, RenderQueueElement const& a_rhs)
	{
		auto const& lhsDepth = a_lhs.depth;
		auto const& rhsDepth = a_rhs.depth;

		// Depth is front-to-back 1-0
		if (lhsDepth < 0 || lhsDepth > 1)
		{
			return false;
		}

		if (rhsDepth < 0 || rhsDepth > 1)
		{
			return true;
		}

		return lhsDepth > rhsDepth;
	}
}
