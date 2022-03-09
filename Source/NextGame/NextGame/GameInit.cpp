#include <MinimalInclude.h>

#include <Application/Init.h>

#include "SimpleFpsCamera.h"
#include "RotateOverTime.h"

using namespace Next;

class LightSpin : public Behaviour
{
	ReflectDeclare(LightSpin, Behaviour)

public:
	void OnCreate() override
	{
		m_light = AddComponent<Light>();
		m_light->type = LightType::Point;
		m_light->diffuseColor = { 5, 0, 0 };

		auto cube = AddComponent<ModelRenderer>();
		cube->model = Model::Create("cube/cube.obj");
	}
	
	void OnUpdate() override
	{
		float theta = Time::ElapsedTime();
		
		float x = 8 * std::cos(theta);
		float y = 8 * std::sin(theta);

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
	//light->ambientColor = { 0.2f, 0.2f, 0.2f };
	light->diffuseColor = { 1, 1, 1 };
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

		entity.AddComponent<RotateOverTime>()->Init(i * 3);

		auto* transform = entity.Transform();
		
		transform->SetPosition({ x, y, 0 });

		if (i != 0)
		{
			transform->SetParent(transforms[i - 1].Transform());
		}

		transforms.push_back(entity);
	}
}
