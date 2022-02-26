#include "pch.h"

#include "Init.h"

#include "Components/LineRenderer.h"
#include "Components/ModelRenderer.h"

#include "Math/Transformations.h"

#include "Rendering/Renderer.h"

#include "Scripting/Entity.h"

void
Init()
{
	Application_Init();
}

// Declare System Updates manually so as to not expose it unnecessarily
namespace Next
{
	namespace Time
	{
		extern
		void
		Update(float a_deltaTime);
	}

	namespace Input
	{
		extern
		void
		Update();
	}
}

void
Update(float a_deltaTime)
{
	// Convert deltaTime into seconds because NextAPI uses milliseconds
	float timeInSeconds = a_deltaTime / 1000.f;
	Next::Time::Update(timeInSeconds);

	Next::Input::Update();

	Next::Entity::Update();
}

void
Render()
{
	using namespace Next;
	
	auto& entityReps = Entity::s_entityRepresentations;

	// Perspective Matrix
	float fov    = 90;
	float aspect = 16.f / 9.f;

	auto perspective = Matrix::Perspective(fov, aspect, 0.1f, 1000.f);
	
	Renderer::PrepareScene({}, perspective);

	for (auto& [id, rep] : entityReps)
	{
		for (auto& element : rep.components)
		{
			if (element.id == Reflection::GetTypeId<LineRenderer>())
			{
				LineRenderer* lineRenderer = static_cast<LineRenderer*>(element.component);
				
				//lineRenderer->OnRender();
			} else if (element.id == Reflection::GetTypeId<ModelRenderer>())
			{
				ModelRenderer* modelRenderer = static_cast<ModelRenderer*>(element.component);

				Renderer::Submit(modelRenderer, modelRenderer->Transform());
			}
		}
	}
	
	Renderer::Flush();
}

void
Shutdown()
{
	// TODO: cleanup entities
}
