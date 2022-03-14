#include "MultiPlanetScene.h"

#include "LevelSelectScene.h"

#include "Scripts/Character/Player/PlayerShip.h"
#include "Scripts/Character/Player/PlayerShipController.h"
#include "Scripts/LevelChange/DistanceLevelChangeLogic.h"
#include "Scripts/LevelChange/LevelChangeManager.h"
#include "Scripts/Objects/CollidableSphereTag.h"
#include "Scripts/Objects/GravitySource.h"
#include "Scripts/Objects/SpinInPlace.h"

ReflectRegister(MultiPlanetScene);

using namespace Next;

void
MultiPlanetScene::OnSceneCreate()
{
	Entity dirLightEntity = Entity::Create("DirLight");
	auto   light          = dirLightEntity.AddComponent<Light>();
	light->type           = LightType::Directional;
	light->Transform()->SetLocalRotation({ -70, 0, 0 });
	light->ambientColor = { 0.5f, 0.5f, 0.5f };
	light->diffuseColor = { 4, 5, 4 };

	Entity playerEntity = Entity::Create("Player");
	playerEntity.AddComponent<PlayerShip>();
	PlayerShip::GetPlayerControllerEntity().Transform()->SetPosition({ 0, 100, 0 });
	PlayerShip::GetPlayerControllerEntity().Transform()->SetLocalRotation({ -70, 0, 0 });
	
	// Add the logic for switching back to the level select scene
	Entity levelChange     = Entity::Create("LevelChangeLogicManager");
	auto   logic           = levelChange.AddComponent<DistanceLevelChangeLogic>();
	logic->distanceToLeave = 300;

	auto manager = levelChange.AddComponent<LevelChangeManager>();
	manager->Init(logic, LevelSelectScene::GetStaticType());
	
	CreateEnvironment();
}

void
MultiPlanetScene::CreateEnvironment()
{
	CreatePlanet("level/env/earthlevel.obj", Vector3(-100, 100, 50), 50, 0, 0);
	CreatePlanet("level/env/copperlevel.obj", Vector3(150, 75, 30), 70, 0, 0);
	CreatePlanet("level/env/marslevel.obj", Vector3(100, -50, -200), 20, 0, 0);
	//CreatePlanet("level/env/earthlevel.obj", Vector3(-70, 0, 0), 50, 0, 0);
	
	// For performance reasons
	#if defined(NEXT_RELEASE)
	for (int i = 0; i < 300; i++)
	{
		Entity star               = Entity::Create("Star" + i);
		auto   renderer           = star.AddComponent<ModelRenderer>();
		renderer->model           = Model::Create("objects/star.obj");
		renderer->receiveLighting = false;
		
		float x = Random::Value() * 600 - 300;
		float y = Random::Value() * 600 - 300;
		float z = Random::Value() * 600 - 300;

		float scale = Random::Value() * 0.5f + 0.1f;
		
		star.Transform()->SetPosition({ x, y, z });
		star.Transform()->SetLocalScale(Vector3(scale));

		star.AddComponent<SpinInPlace>()->spinSpeed = Random::Value() * 145 + 45;
	}
	#endif
}

void
MultiPlanetScene::CreatePlanet(
	std::string_view const& a_filename,
	Vector3 a_position,
	float a_radius,
	float a_numTurrets,
	float a_numPickups
)
{
	// Create a planet that sucks in the player and collides with them
	
	Entity planet = Entity::Create("Planet");

	auto renderer   = planet.AddComponent<ModelRenderer>();
	renderer->model = Model::Create(a_filename);
	planet.Transform()->SetLocalScale(Vector3(a_radius));
	planet.Transform()->SetLocalPosition(a_position);

	auto gravitySource             = planet.AddComponent<GravitySource>();
	gravitySource->gravityStrength = 9.81f * a_radius;

	auto collider    = planet.AddComponent<SphereCollider>();
	collider->radius = a_radius;

	planet.AddComponent<CollidableSphereTag>();
}
