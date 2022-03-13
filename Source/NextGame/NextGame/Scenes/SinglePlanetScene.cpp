#include "SinglePlanetScene.h"

#include "LevelSelectScene.h"

#include "Scripts/Character/Player/PlayerPersistentData.h"
#include "Scripts/Character/Player/PlayerShipController.h"
#include "Scripts/LevelChange/DistanceLevelChangeLogic.h"
#include "Scripts/LevelChange/LevelChangeManager.h"
#include "Scripts/Objects/GravitySource.h"
#include "Scripts/Objects/SpinInPlace.h"

ReflectRegister(SinglePlanetScene);

using namespace Next;

void
SinglePlanetScene::OnSceneCreate()
{
	if (PlayerPersistentData::health <= 0)
	{
		PlayerPersistentData::ResetData();
	}

	Entity playerEntity = Entity::Create("Player");
	playerEntity.AddComponent<PlayerShip>();
	PlayerShip::GetPlayerControllerEntity().Transform()->SetPosition({ 0, 100, 0 });
	PlayerShip::GetPlayerControllerEntity().Transform()->SetLocalRotation({ -70, 0, 0 });

	float planetRadius = 50;

	Entity levelChange = Entity::Create("LevelChangeLogicManager");
	auto logic = levelChange.AddComponent<DistanceLevelChangeLogic>();
	logic->distanceToLeave = planetRadius * 5;

	auto manager = levelChange.AddComponent<LevelChangeManager>();
	manager->Init(logic, LevelSelectScene::GetStaticType());
	
	PlayerShipController::flatPlanet = false;
	PlayerShipController::min_y = planetRadius + 1;

	CreateEnvironment(planetRadius);

	CreateObjects(planetRadius);
}

void
SinglePlanetScene::CreateEnvironment(float a_radius)
{
	Entity dirLightEntity = Entity::Create("DirLight");
	auto   light          = dirLightEntity.AddComponent<Light>();
	light->type           = LightType::Directional;
	light->Transform()->SetLocalRotation({ -70, 0, 0 });
	light->ambientColor = { 0.5f, 0.5f, 0.5f };
	light->diffuseColor = { 4, 5, 4 };

	{
		Entity copper = Entity::Create("Copper");

		auto copperModelRenderer = copper.AddComponent<ModelRenderer>();
		copperModelRenderer->model = Model::Create("level/env/copperlevel.obj");
		copper.Transform()->SetLocalScale(Vector3(a_radius));

		auto gravitySource = copper.AddComponent<GravitySource>();
		gravitySource->gravityStrength = 9.81f * a_radius;
	}
	
	// For performance reasons
	#if defined(NEXT_RELEASE)
	for (int i = 0; i < 200; i++)
	{
		Entity star               = Entity::Create("Star" + i);
		auto   renderer           = star.AddComponent<ModelRenderer>();
		renderer->model           = Model::Create("objects/star.obj");
		renderer->receiveLighting = false;
		
		float r = Random::Value() * (a_radius * 3) + 2 * a_radius;
		
		float polar = Random::Value() * 2 * Math::PI;
		float alpha = Random::Value() * Math::PI - Math::PI / 2;
		
		float x = r * std::sin(polar) * std::cos(alpha);
		float y = r * std::sin(polar) * std::sin(alpha);
		float z = r * std::cos(polar);

		float scale = Random::Value() * 0.5f + 0.1f;
		
		star.Transform()->SetPosition({ x, y, z });
		star.Transform()->SetLocalScale(Vector3(scale));

		star.AddComponent<SpinInPlace>()->spinSpeed = Random::Value() * 145 + 45;
	}
	#endif
}

void
SinglePlanetScene::CreateObjects(float a_radius) {}
