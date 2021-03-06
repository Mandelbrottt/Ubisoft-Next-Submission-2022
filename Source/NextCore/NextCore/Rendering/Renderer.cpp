#include "pch.h"

#include "Renderer.h"

#include "Components/Transform.h"

#include "Graphics/Model.h"

#include "Math/Matrix3.h"

namespace Next::Renderer
{
	/**
	 * \brief An element in the render queue that contains all the information the
	 *        renderer needs to draw a render primitive
	 */
	struct RenderQueueElement
	{
		explicit
		RenderQueueElement(RenderPrimitive const& a_primitive)
			: primitive(a_primitive) { }

		RenderPrimitive primitive;

		//Scripting::EntityId entityId;

		// Should these be moved to a separate list to optimize cache for rendering,
		// or stay here to optimize cache for calculations?
		Vector4 positions[4];

		Vector3 normals[2];

		bool receiveLighting = true;

		Color color = Color::white;

		float depth = 0;
	};

	struct TransformationCacheElement
	{
		std::size_t numPrimitives;

		Matrix4 modelMatrix;
	};

	struct LightInfo
	{
		LightInfo(Vector3 a_lightVector, Vector3 a_ambient, Vector3 a_diffuse, LightType a_type)
			: lightVector(std::move(a_lightVector)),
			  ambient(std::move(a_ambient)),
			  diffuse(std::move(a_diffuse)),
			  type(a_type) {}

		Vector3 lightVector;
		
		Vector3 ambient;
		Vector3 diffuse;
		//Color specular;
		
		LightType type;
	};

	static std::vector<TransformationCacheElement> g_transformationCache;

	// TODO: Do all calculations in-place instead of copying them over every frame
	static std::vector<RenderQueueElement> g_primitiveRenderQueue;

	static std::mutex                      g_rasterQueueMutex;
	static std::vector<RenderQueueElement> g_primitiveRasterQueue;

	static std::vector<LightInfo> g_lightInfos;

	static Vector3 g_cameraPosition;
	static Vector3 g_cameraForward;

	static Matrix4 g_viewMatrix;
	static Matrix4 g_projectionMatrix;

	static CubeMap g_skybox;

	// If capacity is under recent max for X number of frames, shrink to fit next smallest maximum
	static int g_shrinkToFitCounter = 0;

	static
	void
	TransformPrimitivesByModelMatrix(std::size_t a_offset, std::size_t a_count, Matrix4 const& a_modelMatrix);

	static
	void
	TransformPrimitivesByViewProjectionMatrix(
		std::size_t    a_offset,
		std::size_t    a_count,
		Matrix4 const& a_view,
		Matrix4 const& a_projection
	);

	static
	void

	ApplyLightsToPrimitive(Color* a_outColor, Vector3 a_normal, Vector3 a_position);

	[[deprecated("Currrently not working")]]
	static
	void
	RenderSkybox(CubeMap const& a_skybox, Matrix4 const& a_view, Matrix4 const& a_projection);

	void
	PrepareScene(PrepareSceneDescriptor& a_descriptor)
	{
		g_cameraPosition = a_descriptor.cameraPosition;
		g_cameraForward  = a_descriptor.cameraForward;

		g_viewMatrix       = a_descriptor.viewMatrix;
		g_projectionMatrix = a_descriptor.projectionMatrix;

		g_skybox = std::move(a_descriptor.skybox);
	}

	void
	Submit(Light const* a_light, Transform const* a_transform)
	{
		LightInfo info
		{
			a_light->type == LightType::Directional ? a_transform->Forward() : a_transform->GetPosition(),
			Vector3(a_light->ambientColor),
			Vector3(a_light->diffuseColor),
			//a_light->specularColor,
			a_light->type
		};

		g_lightInfos.push_back(std::move(info));
	}

	void
	Submit(ModelRenderer const* a_modelRenderer, Transform const* a_transform)
	{
		std::size_t numPrimitives = 0;

		std::size_t offset = g_primitiveRenderQueue.size();

		for (auto const& mesh : a_modelRenderer->model->GetMeshes())
		{
			auto const& primitives = mesh.GetPrimitives();

			//g_primitiveRenderQueue.insert(queueEnd, primitives.begin(), primitives.end());
			g_primitiveRenderQueue.reserve(g_primitiveRenderQueue.size() + primitives.size());

			for (auto const& primitive : primitives)
			{
				//RenderQueueElement element = { primitive, a_entity.GetEntityId() };
				RenderQueueElement element { primitive };
				element.receiveLighting = a_modelRenderer->receiveLighting;
				g_primitiveRenderQueue.emplace_back(std::move(element));
			}

			numPrimitives += primitives.size();
		}

		TransformationCacheElement element = { numPrimitives, a_transform->GetTransformationMatrix() };

		// TODO: Add to transformation cache and call TransformPrimitivesByModelMatrix on separate thread(s)
		//g_transformationCache.emplace_back(std::move(element));
		TransformPrimitivesByModelMatrix(offset, element.numPrimitives, element.modelMatrix);
	}

	/**
	 * \brief Determine if the given render queue element represents a
	 *        primitive that can be properly rendered to the screen.
	 * \param a_element the RenderQueueElement to check. 
	 */
	static
	bool
	IsValidPrimitive(RenderQueueElement const& a_element);

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
		TransformPrimitivesByViewProjectionMatrix(0, g_primitiveRasterQueue.size(), g_viewMatrix, g_projectionMatrix);

		/* TODO: Fix skybox and make it work
		         Not sure if the math is off somewhere, or what but it is not rendering properly */
		//RenderSkybox(g_skybox, g_viewMatrix, g_projectionMatrix);
		
		// We use the painter's algorithm (https://en.wikipedia.org/wiki/Painter%27s_algorithm)
		// This is because there is no depth buffer in NextAPI, and so this is a simple solution, albeit with
		// some artifacting.
		// In the future we could probably find a more efficient drawing algorithm (BSPs like doom does?)
		std::sort(g_primitiveRasterQueue.begin(), g_primitiveRasterQueue.end(), PrimitiveBackToFrontSortPredicate);

		for (auto& element : g_primitiveRasterQueue)
		{
			if (!IsValidPrimitive(element))
			{
				break;
			}

			auto& primitive = element.primitive;
			auto& positions = element.positions;

			auto* sprite = primitive.GetSprite();

			int numVerts = primitive.GetPrimitiveType() == RenderPrimitiveType::Triangle ? 3 : 4;

			for (int i = 0; i < 4; i++)
			{
				sprite->SetColor(element.color);
			}

			for (int i = 0; i < numVerts; i++)
			{
				auto const& uv = primitive.GetVertex(i).uv;

				sprite->m_simpleSprite->SetVertex(2 * i, positions[i].x);
				sprite->m_simpleSprite->SetVertex(2 * i + 1, positions[i].y);

				sprite->m_simpleSprite->SetUv(2 * i, uv.u);
				sprite->m_simpleSprite->SetUv(2 * i + 1, uv.v);
			}

			// If primitive is a triangle, fourth vertex shares values with first
			if (primitive.GetPrimitiveType() == RenderPrimitiveType::Triangle)
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
		g_lightInfos.clear();
	}

	static std::vector<std::size_t> g_primitivesToRaster;

	void
	TransformPrimitivesByModelMatrix(std::size_t a_offset, std::size_t a_count, Matrix4 const& a_modelMatrix)
	{
		// TODO: Shrink-to-fit the array at some point, maybe if models haven't been loaded recently?
		g_primitivesToRaster.reserve(a_count);

		for (std::size_t i = a_offset; i < a_offset + a_count; i++)
		{
			auto& element   = g_primitiveRenderQueue[i];
			auto& primitive = element.primitive;

			int numVerts = primitive.GetPrimitiveType() == RenderPrimitiveType::Triangle ? 3 : 4;

			auto& positions = element.positions;

			for (int j = 0; j < numVerts; j++)
			{
				auto const& vertex = primitive.GetVertex(j);

				auto transformedVertex = Vector4(vertex.position, 1.0f) * a_modelMatrix;
				positions[j]           = transformedVertex;
			}

			// positions[3] is normally never set for triangles, which messes things up
			if (numVerts == 3)
			{
				positions[3] = positions[0];
			}

			auto& normals = element.normals;

			for (int j = 0; j < 2; j++)
			{
				Vector3 line1 = Vector3(positions[j + 1]) - Vector3(positions[0]);
				Vector3 line2 = Vector3(positions[j + 2]) - Vector3(positions[0]);

				normals[j] = Vector::Normalize(Vector::Cross(line1, line2));
			}

			Vector3 const& cameraPosition = g_cameraPosition;

			Vector3 commonPointOnPrimitive1 = Vector3(positions[0]) - cameraPosition;
			Vector3 commonPointOnPrimitive2 = Vector3(positions[3]) - cameraPosition;

			// Only render the primitive if it is in front of the camera
			// This can't be handled with vertex depth testing alone because of the edge case
			// where some vertices are behind the camera and some are in front such that the average depth
			// is positive, but the primitive doesn't intersect with the camera frustum
			bool isInFrontOfCamera = Vector::Dot(g_cameraForward, commonPointOnPrimitive1) > 0 &&
			                         Vector::Dot(g_cameraForward, commonPointOnPrimitive2) > 0;

			// Back-face culling
			bool isFrontFace = Vector::Dot(normals[0], commonPointOnPrimitive1) < 0 ||
			                   Vector::Dot(normals[1], commonPointOnPrimitive2) < 0;

			if (isFrontFace && isInFrontOfCamera)
			{
				g_primitivesToRaster.push_back(i);
			}
		}

		std::scoped_lock lock(g_rasterQueueMutex);
		for (std::size_t i = 0; i < g_primitivesToRaster.size(); i++)
		{
			std::size_t index = g_primitivesToRaster[i];

			g_primitiveRasterQueue.emplace_back(std::move(g_primitiveRenderQueue[index]));
		}

		g_primitivesToRaster.clear();
	}

	void
	TransformPrimitivesByViewProjectionMatrix(
		std::size_t    a_offset,
		std::size_t    a_count,
		Matrix4 const& a_view,
		Matrix4 const& a_projection
	)
	{
		for (std::size_t i = a_offset; i < a_count; i++)
		{
			auto& element   = g_primitiveRasterQueue[i];
			auto& primitive = element.primitive;
			auto& positions = element.positions;
			auto& normals   = element.normals;

			int numVerts = primitive.GetPrimitiveType() == RenderPrimitiveType::Triangle ? 3 : 4;

			Vector3 averageNormal = normals[0];
			Vector3 averagePosition { 0 };

			// Compute average position of primitive
			for (int j = 0; j < numVerts; j++)
			{
				averagePosition += Vector3(positions[j]);
			}
			averagePosition /= static_cast<float>(numVerts);
			
			if (normals[1] != Vector3(0))
			{
				averageNormal += normals[1];
				averageNormal = Vector::Normalize(averageNormal * 0.5f);
			}

			for (int j = 0; j < numVerts; j++)
			{
				if (element.receiveLighting)
				{
					ApplyLightsToPrimitive(&element.color, averageNormal, averagePosition);
				} else
				{
					element.color = Color::white;
				}
				
				auto projectedVertex = positions[j] * a_view * a_projection;

				projectedVertex.x /= projectedVertex.w;
				projectedVertex.y /= projectedVertex.w;
				projectedVertex.z /= projectedVertex.w;

				element.depth += projectedVertex.z;

				positions[j] = projectedVertex;
			}

			element.depth = element.depth / numVerts;
		}
	}
	
	void
	ApplyLightsToPrimitive(Color* a_outColor, Vector3 a_normal, Vector3 a_position)
	{
		if (g_lightInfos.empty())
		{
			*a_outColor = Color(1, 1, 1);
			return;
		}

		Vector3 colorVector { 0 };
		
		for (auto const& lightInfo : g_lightInfos)
		{
			switch (lightInfo.type)
			{
				case LightType::Directional:
				{
					// lightInfo.lightVector is interpreted as light direction here
					float dotProduct = Vector::Dot(a_normal, -lightInfo.lightVector);
					dotProduct       = std::max(dotProduct, 0.f);

					Vector3 colorResult = Vector3(dotProduct) * lightInfo.diffuse;
					colorResult += lightInfo.ambient;
					
					colorVector += colorResult;
				
					break;
				}
				case LightType::Point:
				{
					// lightInfo.lightVector is interpreted as light position here
					Vector3 lightDir = Vector::Normalize(lightInfo.lightVector - a_position);

					float diff = std::max(Vector::Dot(a_normal, lightDir), 0.0f);
					Vector3 diffuse = lightInfo.diffuse * diff;
					
					Vector3 colorResult = lightInfo.ambient + diffuse;

					colorVector += colorResult;
				
					break;
				}
				default:
					assert(false); // Not implemented
			}
		}

		// Reinhardt tone mapping to not blow out the light color to white if multiple high-intensity lights hit it
		// Not a great solution because the color we're calculating here is not necessarily the color applied to
		// the frame-buffer, and so the real output color cannot be more than the input texture but it does something
		colorVector = colorVector / (colorVector + Vector3(1.0f));
		
		*a_outColor = Color(colorVector);
	}

	static
	void
	RenderSkybox(CubeMap const& a_skybox, Matrix4 const& a_view, Matrix4 const& a_projection)
	{
		for (auto const& p : a_skybox.GetFaces())
		{
			if (!p.IsValid())
			{
				continue;
			}
			
			RenderQueueElement element { p };
			
			auto& primitive = element.primitive;
			auto& positions = element.positions;
			auto& normals   = element.normals;

			int numVerts = primitive.GetPrimitiveType() == RenderPrimitiveType::Triangle ? 3 : 4;

			for (int i = 0; i < numVerts; i++)
			{
				auto const& position = primitive.GetVertex(i).position;
				positions[i] = Vector4(position);
			}

			for (int j = 0; j < numVerts - 2; j++)
			{
				Vector3 line1 = Vector3(positions[j + 1]) - Vector3(positions[0]);
				Vector3 line2 = Vector3(positions[j + 2]) - Vector3(positions[0]);

				normals[j] = Vector::Normalize(Vector::Cross(line1, line2));
			}
			
			// Only render the primitive if it is in front of the camera
			// This can't be handled with vertex depth testing alone because of the edge case
			// where some vertices are behind the camera and some are in front such that the average depth
			// is positive, but the primitive doesn't intersect with the camera frustum
			bool isInFrontOfCamera = Vector::Dot(g_cameraForward, -normals[0]) < 0 ||
			                         Vector::Dot(g_cameraForward, -normals[1]) < 0;

			if (!isInFrontOfCamera)
			{
				continue;
			}
			
			// Extract only rotation information from view matrix
			auto adjustedViewMatrix = Matrix4(Matrix3(a_view));
			adjustedViewMatrix[3][3] = 1;
			
			for (int j = 0; j < numVerts; j++)
			{
				auto projectedVertex = positions[j] * adjustedViewMatrix * a_projection;

				projectedVertex.x /= projectedVertex.w;
				projectedVertex.y /= projectedVertex.w;
				projectedVertex.z /= projectedVertex.w;
				
				positions[j] = projectedVertex;
			}

			element.depth = 1;

			g_primitiveRasterQueue.push_back(element);
		}
	}

	// Determine if a transformed and projected primitive is should be rendered or culled
	bool
	IsValidPrimitive(RenderQueueElement const& a_element)
	{
		// Cull primitives outside of clip-space
		if (a_element.depth < 0 || a_element.depth > 1)
		{
			return false;
		}

		auto& positions = a_element.positions;

		// Cull off-screen primitives
		// TODO: Edge case where all four vertices are outside of ndc but the primitive still covers the screen
		bool isAnyInbounds = false;
		for (int i = 0; i < sizeof(positions) / sizeof(*positions); i++)
		{
			auto& position = positions[i];
			if (position.x >= -1 && position.x <= 1 && position.y >= -1 && position.y <= 1)
			{
				isAnyInbounds = true;
			}
		}

		return isAnyInbounds;
	}

	bool
	PrimitiveBackToFrontSortPredicate(RenderQueueElement const& a_lhs, RenderQueueElement const& a_rhs)
	{
		auto const& lhsDepth = a_lhs.depth;
		auto const& rhsDepth = a_rhs.depth;

		// Depth is front-to-back [1, 0]
		if (!IsValidPrimitive(a_lhs))
		{
			return false;
		}

		if (!IsValidPrimitive(a_rhs))
		{
			return true;
		}

		return lhsDepth > rhsDepth;
	}
}
