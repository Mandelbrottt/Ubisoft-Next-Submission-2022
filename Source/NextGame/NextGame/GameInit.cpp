#include <MinimalInclude.h>

#include <Application/Init.h>

#include "SimpleFpsCamera.h"
#include "RotateOverTime.h"

// TODO: Write wrapper printing to screen

using namespace Next;

void
Application_Init()
{
	Model* suzanne = Model::Create("complex/suzanne.obj");

	Entity mainCamera = Entity::Create("MainCamera");
	mainCamera.AddComponent<SimpleFpsCamera>();
	
	mainCamera.Transform()->SetPosition({ 10, 10, -10 });
	mainCamera.Transform()->SetRotation({ -35, -45, 0 });
	
	std::vector<Entity> transforms;
	
	for (int i = 0; i < 30; i++)
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
