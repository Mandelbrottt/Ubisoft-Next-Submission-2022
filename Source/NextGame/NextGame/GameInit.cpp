#include <MinimalInclude.h>

#include <Application/Init.h>

#include "SimpleFpsCamera.h"
#include "RotateOverTime.h"

// TODO: Write wrapper printing to screen

using namespace Next;

class LightSpin : public Behaviour
{
	ReflectDeclare(LightSpin, Behaviour)

public:
	void OnCreate() override
	{
		m_light = AddComponent<Light>();
		m_light->type = LightType::Spot;
		m_light->diffuseColor = { 0.8, 0.8, 0.8 };

		auto cube = AddComponent<ModelRenderer>();
		cube->model = Model::Create("cube/cube.obj");
	}
	
	void OnUpdate() override
	{
		float x = 8 * std::cos(Time::ElapsedTime());
		float y = 8 * std::sin(Time::ElapsedTime());

		Transform()->SetPosition({ x, 0, y });
	}

private:
	Light* m_light;

	ReflectMembers(
		ReflectField(m_light)
	)
};

ReflectRegister(LightSpin);

void
Application_Init()
{
	Model* suzanne = Model::Create("complex/suzanne.obj");

	Entity dirLight = Entity::Create("DirLight");
	auto light = dirLight.AddComponent<Light>();
	light->type = LightType::Directional;
	light->ambientColor = { 0.2f, 0.2f, 0.2f };
	light->diffuseColor = { 0.2f, 0.2f, 0.2f };
	dirLight.Transform()->SetRotation({ -35, -45, 0 });
	
	Entity mainCamera = Entity::Create("MainCamera");
	mainCamera.AddComponent<SimpleFpsCamera>();
	
	mainCamera.Transform()->SetPosition({ 10, 10, -10 });
	mainCamera.Transform()->SetRotation({ -35, -45, 0 });

	Entity pointLight = Entity::Create("Point Light");
	pointLight.AddComponent<LightSpin>();
	
	std::vector<Entity> transforms;
	
	for (int i = 0; i < 5; i++)
	{
		float angle = 360.f * i / 10.f;

		float x = 4 * std::cos(angle);
		float y = 4 * std::sin(angle);

		if (i == 0) x = y = 0;

		Entity entity = Entity::Create("Monkey " + std::to_string(i + 1));

		auto* innerModelRenderer = entity.AddComponent<ModelRenderer>();
		innerModelRenderer->model = suzanne;

		entity.AddComponent<RotateOverTime>()->Init(i);

		auto* transform = entity.Transform();
		
		transform->SetPosition({ x, y, 0 });

		if (i != 0)
		{
			transform->SetParent(transforms[i - 1].Transform());
		}

		transforms.push_back(entity);
	}
}
