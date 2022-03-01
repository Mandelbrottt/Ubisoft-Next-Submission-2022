#include "pch.h"

#include "Application.h"
#include "Init.h"

#include "Components/Camera.h"
#include "Components/LineRenderer.h"
#include "Components/ModelRenderer.h"
#include "Components/Transform.h"

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

	float fov  = 90;
	float near = 0.1f;
	float far  = 1000.0f;

	Vector3 cameraPosition { 0 };
	Vector3 cameraForward { 0, 0, 1 };
	Matrix4 viewMatrix = Matrix4::Identity();

	CubeMap skybox;

	for (auto& [entityId, rep] : entityReps)
	{
		for (auto& [typeId, component] : rep.components)
		{
			if (typeId != Reflection::GetTypeId<Camera>())
			{
				continue;
			}

			auto* camera = static_cast<Camera*>(component);

			fov  = camera->GetFov();
			near = camera->GetNearClippingPlane();
			far  = camera->GetFarClippingPlane();

			auto* transform = camera->Transform();

			cameraPosition = transform->GetPosition();
			cameraForward = transform->Forward();

			viewMatrix = transform->GetTransformationMatrix();
			viewMatrix = Matrix::ViewInverse(viewMatrix);

			skybox = camera->GetSkybox();

			goto Render_Main_Camera_Found;
		}
	}

Render_Main_Camera_Found:

	float aspect = Application::ScreenHeight() / Application::ScreenWidth();

	// Perspective Matrix
	auto perspective = Matrix::Perspective(fov, aspect, near, far);

	Renderer::PrepareSceneDescriptor descriptor = {
		cameraPosition,
		cameraForward,
		viewMatrix,
		perspective,
		std::move(skybox)
	};
	
	PrepareScene(descriptor);

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
