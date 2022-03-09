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
	
	float fov  = 90;
	float near = 0.1f;
	float far  = 1000.0f;

	Vector3 cameraPosition { 0 };
	Vector3 cameraForward { 0, 0, 1 };
	Matrix4 viewMatrix = Matrix4::Identity();

	CubeMap skybox;

	static std::vector<Camera*> cameras;
	Entity::GetAllComponents(cameras);

	if (!cameras.empty())
	{
		auto* camera = cameras.front();

		fov  = camera->GetFov();
		near = camera->GetNearClippingPlane();
		far  = camera->GetFarClippingPlane();

		auto* transform = camera->Transform();

		cameraPosition = transform->GetPosition();
		cameraForward = transform->Forward();

		viewMatrix = transform->GetTransformationMatrix();
		viewMatrix = Matrix::ViewInverse(viewMatrix);

		skybox = camera->GetSkybox();
	}
	
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

	// Submit all of the Lights in the scene
	static std::vector<Light*> lights;
	Entity::GetAllComponents(lights);

	for (auto* light : lights)
	{
		Renderer::Submit(light, light->Transform());
	}

	// Submit all of the ModelRenderers in the scene
	static std::vector<ModelRenderer*> modelRenderers;
	Entity::GetAllComponents(modelRenderers);

	for (auto* modelRenderer : modelRenderers)
	{
		Renderer::Submit(modelRenderer, modelRenderer->Transform());
	}

	Renderer::Flush();
}

void
Shutdown()
{
	// TODO: cleanup entities
}
