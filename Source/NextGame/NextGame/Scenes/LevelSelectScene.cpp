#include "LevelSelectScene.h"

#include "FlatGroundScene.h"
#include "MultiPlanetScene.h"
#include "SinglePlanetScene.h"

#include "Scripts/Character/Player/PlayerPersistentData.h"
#include "Scripts/Character/Player/PlayerShip.h"
#include "Scripts/LevelChange/PlanetLevelSelector.h"
#include "Scripts/Objects/SpinInPlace.h"

ReflectRegister(LevelSelectScene);

using namespace Next;

void
LevelSelectScene::OnSceneCreate()
{	
	if (PlayerPersistentData::health <= 0)
	{
		PlayerPersistentData::ResetData();
	}
	
	//SceneManager::LoadScene<FlatGroundScene>();
	
	Entity playerEntity = Entity::Create("Player");
	playerEntity.AddComponent<PlayerShip>();
	playerEntity.Transform()->SetPosition({ 0, 0, 0 });

	{
		Entity earth = Entity::Create("Earth");
		auto earthPlanet = earth.AddComponent<PlanetLevelSelector>();
		earthPlanet->Init("level/select/earth.obj", 10, 90, FlatGroundScene::GetStaticType());
		earth.Transform()->SetPosition({ 0, 0, 50 });
	}
	{
		Entity copper     = Entity::Create("Copper");
		auto copperPlanet = copper.AddComponent<PlanetLevelSelector>();
		copperPlanet->Init("level/select/copper.obj", 15, 45, SinglePlanetScene::GetStaticType());
		copper.Transform()->SetPosition({ -30, 0, -40 });
	}
	{
		// TODO: Easter egg??
		Entity mars     = Entity::Create("Mars");
		auto marsPlanet = mars.AddComponent<PlanetLevelSelector>();
		marsPlanet->Init("level/select/mars.obj", 3, 145, MultiPlanetScene::GetStaticType());
		mars.Transform()->SetPosition({ 40, 0, -10 });
	}

	// For performance reasons
	#if defined(NEXT_RELEASE)
	for (int i = 0; i < 200; i++)
	{
		Entity star = Entity::Create("Star" + i);
		star.AddComponent<ModelRenderer>()->model = Model::Create("objects/star.obj");

		const float boxRadius = 75;

		float x = Random::Value() * (boxRadius * 2) - boxRadius;
		float y = Random::Value() * (boxRadius * 2) - boxRadius;
		float z = Random::Value() * (boxRadius * 2) - boxRadius;

		float scale = Random::Value() * 0.5f + 0.1f;
		
		star.Transform()->SetPosition({ x, y, z });
		star.Transform()->SetLocalScale(Vector3(scale));

		star.AddComponent<SpinInPlace>()->spinSpeed = Random::Value() * 145 + 45;
	}
	#endif
}
