#include "FlatGroundScene.h"

#include "Scripts/Character/Enemy/TurretFireController.h"
#include "Scripts/Character/Player/PlayerShip.h"
#include "Scripts/Objects/FuelPickup.h"

ReflectRegister(FlatGroundScene);

using namespace Next;

void
FlatGroundScene::OnSceneCreate()
{
	Entity dirLightEntity = Entity::Create("DirLight");
	auto   light          = dirLightEntity.AddComponent<Light>();
	light->type = LightType::Directional;
	light->Transform()->SetLocalRotation({ -70, 0, 0 });
	light->ambientColor   = { 0.5f, 0.5f, 0.5f };
	light->diffuseColor   = { 4, 5, 4 };

	Entity playerEntity = Entity::Create("Player");
	playerEntity.AddComponent<PlayerShip>();
	playerEntity.Transform()->SetPosition({ 0, 5, 0 });

	Model* ground = Model::Create("level/env/grass.obj");

	float size = 20;
	size *= 4;

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Entity groundEntity   = Entity::Create("Ground");
			auto   groundRenderer = groundEntity.AddComponent<ModelRenderer>();
			groundRenderer->model = ground;

			float x = static_cast<float>(i - 5 / 2);
			float y = static_cast<float>(j - 5 / 2);

			groundEntity.Transform()->SetLocalPosition({ 20 * x, 0, 20 * y });
		}
	}

	for (int i = 0; i < 10; i++)
	{
		Entity turretEntity = Entity::Create("Turret " + std::to_string(i));
		turretEntity.AddComponent<TurretFireController>();

		float x = (Random::Value() * size) - (size / 2);
		float y = (Random::Value() * size) - (size / 2);
		turretEntity.Transform()->SetPosition({ x, 0.5f, y });
	}
	
	for (int i = 0; i < 10; i++)
	{
		Entity fuelEntity = Entity::Create("Fuel " + std::to_string(i));
		fuelEntity.AddComponent<FuelPickup>();
		
		float x = (Random::Value() * size) - (size / 2);
		float y = (Random::Value() * size) - (size / 2);
		fuelEntity.Transform()->SetPosition({ x, 0.5f, y });
	}
}
