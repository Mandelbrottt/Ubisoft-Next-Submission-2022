#include "FlatGroundScene.h"

#include "Scripts/Player/PlayerShip.h"

ReflectRegister(FlatGroundScene);

using namespace Next;

void
FlatGroundScene::OnSceneCreate()
{
	Entity playerEntity = Entity::Create("Player");
	playerEntity.AddComponent<PlayerShip>();

	Model* ground = Model::Create("level/env/grass.obj");
	
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Entity groundEntity = Entity::Create("Ground");
			auto groundRenderer = groundEntity.AddComponent<ModelRenderer>();
			groundRenderer->model = ground;

			float x = static_cast<float>(i - 5 / 2);
			float y = static_cast<float>(j - 5 / 2);
			
			groundEntity.Transform()->SetLocalPosition({ 20 * x, -3, 20 * y });
		}
	}

}
