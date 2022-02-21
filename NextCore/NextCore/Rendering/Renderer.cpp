#include "pch.h"

#include "Renderer.h"

#include <vector>

#include "Components/Transform.h"

#include "Graphics/Model.h"

namespace NextCore::Renderer
{
	struct RenderQueueElement
	{
		Graphics::Primitive primitive;

		Scripting::EntityId entityId;
	};

	// TODO: Store offsets and sizes in separate array instead of in each primitive to reduce storage requirements
	std::vector<RenderQueueElement> g_primitiveRenderQueue;

	std::unordered_map<Scripting::EntityId, Math::Matrix4> g_modelMatrices;
	
	// If capacity is under recent max for X number of frames, shrink to fit next smallest maximum
	static int g_shrinkToFitCounter = 0;
	
	void
	Submit(Graphics::Model const& a_model, Scripting::Entity& a_entity)
	{
		for (auto const& mesh : a_model.GetMeshes())
		{
			auto const& primitives = mesh.GetPrimitives();
			
			//g_primitiveRenderQueue.insert(queueEnd, primitives.begin(), primitives.end());
			g_primitiveRenderQueue.reserve(g_primitiveRenderQueue.size() + primitives.size());

			for (auto const& primitive : primitives)
			{
				RenderQueueElement element = { primitive, a_entity.GetEntityId() };
				g_primitiveRenderQueue.emplace_back(std::move(element));
			}
		}

		g_modelMatrices[a_entity.GetEntityId()] = a_entity.Transform()->GetTransformationMatrix();
	}

	static
	bool
	PrimitiveBackToFrontSort(RenderQueueElement const& a_lhs, RenderQueueElement const& a_rhs)
	{
		auto const& lhs = a_lhs.primitive;
		auto const& rhs = a_rhs.primitive;
		
		// Depth is front-to-back 1-0
		if (lhs.GetDepth() < 0 || lhs.GetDepth() > 1)
		{
			return false;
		}

		if (rhs.GetDepth() < 0 || rhs.GetDepth() > 1)
		{
			return true;
		}
		
		return lhs.GetDepth() > rhs.GetDepth();
	}
	
	/**
	 * \brief Flush the renderer's command buffer and draw to the screen.
	 * \remark This function needs to be manually declared to use, as
	 *         this function is only intended to be called by the system at
	 *         the end of each frame
	 */
	NEXT_CORE_EXPORT
	void
	Flush()
	{
		// We use the painter's algorithm (https://en.wikipedia.org/wiki/Painter%27s_algorithm)
		// This is because there is no depth buffer in NextAPI, and so this is a simple solution, albeit with
		// some artifacting.
		// In the future we could probably find a more efficient drawing algorithm (BSPs like doom does?)
		std::sort(g_primitiveRenderQueue.begin(), g_primitiveRenderQueue.end(), PrimitiveBackToFrontSort);
		
		for (auto& [primitive, entityId] : g_primitiveRenderQueue)
		{
			float depth = primitive.GetDepth();
			if (depth < 0 || depth > 1)
			{
				break;
			}
			
			primitive.OnRender();
		}

		// Reallocation / capacity change not guaranteed according to
		// https://www.cplusplus.com/reference/vector/vector/capacity/
		// When does it happen? Can we avoid this?
		g_primitiveRenderQueue.clear();
		g_modelMatrices.clear();
	}
}
