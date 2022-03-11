#include "Scenes.h"

#include <MinimalInclude.h>

#include "RotateOverTime.h"
#include "SimpleFpsCamera.h"

using namespace Next;

StartingScene(SceneA);

ReflectRegister(SceneA);
ReflectRegister(SceneB);

class ChangeSceneComponent : public Next::Behaviour
{
	ComponentDeclare(ChangeSceneComponent, Behaviour)

public:
	void OnUpdate() override
	{
		if (Input::GetKeyDown(KeyCode::H))
		{
			printf("H\n");
			SceneManager::ChangeScene<SceneA>();
		}
		if (Input::GetKeyDown(KeyCode::J))
		{
			printf("J\n");
			SceneManager::ChangeScene<SceneB>();
		}
	}
};

void
SceneA::OnSceneCreate()
{
	Model* suzanne = Model::Create("complex/suzanne.obj");

	Entity dirLight = Entity::Create("DirLight");
	auto   light    = dirLight.AddComponent<Light>();
	light->type     = LightType::Directional;
	//light->ambientColor = { 0.2f, 0.2f, 0.2f };
	light->diffuseColor = { 1, 1, 1 };
	dirLight.Transform()->SetRotation({ -35, -45, 0 });

	Entity mainCamera = Entity::Create("MainCamera");
	mainCamera.AddComponent<SimpleFpsCamera>();
	mainCamera.AddComponent<ChangeSceneComponent>();

	mainCamera.Transform()->SetPosition({ 10, 10, -10 });
	mainCamera.Transform()->SetRotation({ -35, -45, 0 });

	std::vector<Entity> transforms;

	for (int i = 0; i < 5; i++)
	{
		float angle = 360.f * i / 10.f;

		float x = 4 * std::cos(angle);
		float y = 4 * std::sin(angle);

		if (i == 0)
			x = y = 0;

		Entity entity = Entity::Create("Monkey " + std::to_string(i + 1));

		auto* innerModelRenderer  = entity.AddComponent<ModelRenderer>();
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

void
SceneB::OnSceneCreate()
{
	Model* suzanne = Model::Create("complex/suzanne.obj");
	
	Entity mainCamera = Entity::Create("MainCamera");
	mainCamera.AddComponent<SimpleFpsCamera>();
	mainCamera.AddComponent<ChangeSceneComponent>();
	
	Entity monke = Entity::Create("Monke");
	auto modelRenderer = monke.AddComponent<ModelRenderer>();
	modelRenderer->model = suzanne;
	monke.Transform()->SetPosition({ 0, 0, 5 });
	monke.Transform()->SetRotation({ 0, 180, 0 });
}
