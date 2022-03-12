#include "FlatGroundScene.h"

#include "Scripts/Enemy/TurretFireController.h"
#include "Scripts/Player/PlayerShip.h"

ReflectRegister(FlatGroundScene);

using namespace Next;

void
FlatGroundScene::OnSceneCreate()
{
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
			Entity groundEntity = Entity::Create("Ground");
			auto groundRenderer = groundEntity.AddComponent<ModelRenderer>();
			groundRenderer->model = ground;

			float x = static_cast<float>(i - 5 / 2);
			float y = static_cast<float>(j - 5 / 2);
			
			groundEntity.Transform()->SetLocalPosition({ 20 * x, 0, 20 * y });
		}
	}

	for (int i = 0; i < 8; i++)
	{
		Entity turretEntity = Entity::Create("Turret " + std::to_string(i));
		turretEntity.AddComponent<TurretFireController>();
		auto turretRenderer = turretEntity.AddComponent<ModelRenderer>();
		turretRenderer->model = Model::Create("objects/turret.obj");

		float x = (Random::Value() * size) - (size / 2);
		float y = (Random::Value() * size) - (size / 2);
		turretEntity.Transform()->SetPosition({ x, 0, y });
	}
	
}
