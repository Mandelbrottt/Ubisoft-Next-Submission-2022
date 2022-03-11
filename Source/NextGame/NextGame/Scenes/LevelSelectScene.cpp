#include "LevelSelectScene.h"

#include "Scripts/Player/PlayerShip.h"

ReflectRegister(LevelSelectScene);

using namespace Next;

void
LevelSelectScene::OnSceneCreate()
{
	Entity playerEntity = Entity::Create("Player");
	playerEntity.AddComponent<PlayerShip>();

	Entity groundEntity = Entity::Create("Ground");
	auto groundRenderer = groundEntity.AddComponent<ModelRenderer>();
	groundRenderer->model = Model::Create("level/env/grass.obj");
	groundEntity.Transform()->SetLocalPosition({ 0, -3, 0 });
	groundEntity.Transform()->SetLocalScale({ 3, 3, 3 });
}
