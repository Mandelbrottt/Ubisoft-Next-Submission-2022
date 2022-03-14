#include "FlatGroundScene.h"

#include "LevelSelectScene.h"

#include "Scripts/Character/Enemy/TurretFireController.h"
#include "Scripts/Character/Player/PlayerPersistentData.h"
#include "Scripts/Character/Player/PlayerShip.h"
#include "Scripts/LevelChange/DistanceLevelChangeLogic.h"
#include "Scripts/LevelChange/LevelChangeManager.h"
#include "Scripts/Objects/FuelPickup.h"

ReflectRegister(FlatGroundScene);

using namespace Next;

void
FlatGroundScene::OnSceneCreate()
{
	// In case the user is starting in this scene instead of the menu scene
	if (PlayerPersistentData::health <= 0)
	{
		PlayerPersistentData::ResetData();
	}

	// Special logic if the planet is flat
	PlayerShipController::flatPlanet = true;

	// Create the player at the top of the map facing downwards
	Entity playerEntity = Entity::Create("Player");
	playerEntity.AddComponent<PlayerShip>();
	PlayerShip::GetPlayerControllerEntity().Transform()->SetPosition({ 0, 20, 0 });
	PlayerShip::GetPlayerControllerEntity().Transform()->SetLocalRotation({ -70, 0, 0 });

	// Add the logic for switching back to the level select scene
	Entity levelChange = Entity::Create("LevelChangeLogicManager");
	auto logic = levelChange.AddComponent<DistanceLevelChangeLogic>();
	logic->distanceToLeave = 150;

	auto manager = levelChange.AddComponent<LevelChangeManager>();
	manager->Init(logic, LevelSelectScene::GetStaticType());
	
	float size = 20;

	float envSize = size * 10;
	float objSize = size * 6;

	#if !defined(NEXT_RELEASE)
	envSize = size * 5;
	objSize = size * 3;
	#endif
	
	CreateEnvironment(envSize);
	CreateObjects(objSize);
}

void
FlatGroundScene::OnSceneDestroy()
{
	PlayerShipController::flatPlanet = false;
}

void
FlatGroundScene::CreateEnvironment(float a_size)
{
	// Add sky sphere
	Entity sky                = Entity::Create("Sky");
	auto   skyModel           = sky.AddComponent<ModelRenderer>();
	skyModel->model           = Model::Create("level/env/sky.obj");
	skyModel->receiveLighting = false;
	sky.Transform()->SetLocalScale(Vector3(150));

	// Add directional light
	Entity dirLightEntity = Entity::Create("DirLight");
	auto   light          = dirLightEntity.AddComponent<Light>();
	light->type           = LightType::Directional;
	light->Transform()->SetLocalRotation({ -70, 0, 0 });
	light->ambientColor = { 0.5f, 0.5f, 0.5f };
	light->diffuseColor = { 4, 5, 4 };

	float groundHeight          = 0;
	PlayerShipController::min_y = groundHeight + 1;
	
	Model* ground = Model::Create("level/env/grass.obj");

	float numX = a_size / 20;
	float numY = a_size / 20;

	// Create a set of tiled ground planes in a square around the origin
	for (int i = 0; i < numX; i++)
	{
		for (int j = 0; j < numY; j++)
		{
			Entity groundEntity   = Entity::Create("Ground");
			auto   groundRenderer = groundEntity.AddComponent<ModelRenderer>();
			groundRenderer->model = ground;

			float x = static_cast<float>(i - numX / 2);
			float y = static_cast<float>(j - numY / 2);

			groundEntity.Transform()->SetLocalPosition(
				{ a_size / (numX) * x, groundHeight, a_size / (numY) * y }
			);
		}
	}
}

void
FlatGroundScene::CreateObjects(float a_size)
{
	// Randomly generate turrets
	for (int i = 0; i < 16; i++)
	{
		Entity turretEntity = Entity::Create("Turret " + std::to_string(i));
		turretEntity.AddComponent<TurretFireController>();

		float x = (Random::Value() * a_size) - (a_size / 2);
		float y = (Random::Value() * a_size) - (a_size / 2);
		turretEntity.Transform()->SetPosition({ x, 0.5f, y });
	}

	// Randomly generate fuel crates
	for (int i = 0; i < 10; i++)
	{
		Entity fuelEntity = Entity::Create("Fuel " + std::to_string(i));
		fuelEntity.AddComponent<FuelPickup>();

		float x = (Random::Value() * a_size) - (a_size / 2);
		float y = (Random::Value() * a_size) - (a_size / 2);
		fuelEntity.Transform()->SetPosition({ x, 0.5f, y });
	}
}
