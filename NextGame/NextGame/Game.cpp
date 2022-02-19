#include "Game.h"

#include <MinimalInclude.h>
#include <algorithm>

#include <Components/LineRenderer.h>

#include <Graphics/Model.h>

#include <Math/Transformations.h>

#include <NextAPI/app.h>

#include "CrazyLineThing.h"
#include "Cube.h"

// TODO: Decouple Sound, Write wrapper for drawing and printing to screen

using namespace NextCore;

std::vector<Scripting::Entity> g_entities;

Graphics::Model g_model;

void
GameInit()
{
	g_entities.reserve(100);

	g_model.LoadFromFile(Application::ResourcePath() + "cube/cube.obj");
	
	{
		//g_entities.emplace_back();
		//Scripting::Entity& player = g_entities.back();
		//player.AddComponent(Player::GetType());
		//player.AddComponent(CrazyLineThing::GetType());
	}
	{
		g_entities.emplace_back();
		Scripting::Entity& cube = g_entities.back();
		cube.AddComponent(Cube::GetType());
	}
}

void
GameUpdate()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	for (auto& entity : g_entities)
	{
		entity.OnUpdate();
	}
}

void
GameRender()
{	
	for (auto& entity : g_entities)
	{
		int lineRendererCount;
		auto* lineRenderers = entity.GetComponents<Component::LineRenderer>(&lineRendererCount);
		
		if (lineRenderers)
		{
			for (int i = 0; i < lineRendererCount; i++)
			{
				lineRenderers[i]->OnRender();
			}
		}
	}

	using namespace Math;

	// Model Matrix
	auto* transform = g_entities[0].GetComponent<Component::Transform>();
	auto& scale     = transform->Scale();
	auto& rotation  = transform->Rotation();
	auto& position  = transform->Position();

	auto model = Matrix4::Identity();
	model *= Scale(scale);
	model *= RotateZ(rotation.z);
	model *= RotateY(rotation.y);
	model *= RotateX(rotation.x);
	model *= Translate(position);

	// View Matrix

	// Perspective Matrix
	float fov    = 90;
	float aspect = 16.f / 9.f;

	auto perspective = Perspective(fov, aspect, 0.1f, 1000.f);

	for (auto& mesh : g_model.GetMeshes())
	{
		auto const&                 primitives       = mesh.GetPrimitives();
		const Graphics::Primitive** sortedPrimitives = new const Graphics::Primitive*[primitives.size()];
		for (int i = 0; i < primitives.size(); i++)
		{
			sortedPrimitives[i] = &primitives[i];
			const_cast<Graphics::Primitive&>(primitives[i]).OnRenderPrepare(model, {}, perspective);
		}

		auto predicate = [](const Graphics::Primitive* a_lhs, const Graphics::Primitive* a_rhs)
		{
			// Depth is front-to-back 1-0
			return a_lhs->GetDepth() > a_rhs->GetDepth();
		};

		std::sort(sortedPrimitives, sortedPrimitives + primitives.size(), predicate);

		for (int i = 0; i < primitives.size(); i++)
		{
			const_cast<Graphics::Primitive*>(sortedPrimitives[i])->OnRender();
		}
	}

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");
}

void
GameShutdown()
{

}